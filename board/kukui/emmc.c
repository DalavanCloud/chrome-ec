/* Copyright 2018 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * Transfer bootblock over SPI by emulating eMMC "Alternative Boot operation"
 * (section 6.3.4 of eMMC 5.0 specification, JESD84-B50).
 *
 * eMMC boot operation looks a lot like SPI: CMD is unidirectional MOSI, DAT is
 * unidirectional MISO. CLK is driven by the master. However, there is no
 * chip-select, and the clock is active for a long time before any command is
 * sent on the CMD line. From SPI perspective, this looks like a lot of '1'
 * are being sent from the master.
 *
 * To catch the commands, we setup DMA to write the data into a circular buffer
 * (in_msg), and monitor for a falling edge on CMD (emmc_cmd_interrupt). Once
 * an interrupt is received, we scan the circular buffer, in reverse, to
 * be as fast as possible and minimize chances of missing the command.
 *
 * We then figure out the bit-wise command alignment, decode it, and, upon
 * receiving BOOT_INITIATION command, setup DMA to respond with the data on the
 * DAT line. The data in bootblock_data.h is preprocessed to include necessary
 * eMMC headers: acknowledge boot mode, start of block, CRC, end of block, etc.
 * The host can only slow down transfer by stopping the clock, which is
 * compatible with SPI.
 *
 * In some cases (e.g. if the BootROM expects data over 8 lanes instead of 1),
 * the BootROM will quickly interrupt the transfer with an IDLE command. In this
 * case we interrupt the transfer, and the BootROM will try again.
 */

#include "clock.h"
#include "console.h"
#include "dma.h"
#include "endian.h"
#include "gpio.h"
#include "task.h"
#include "timer.h"

#include "bootblock_data.h"

#define CPRINTS(format, args...) cprints(CC_SPI, format, ## args)
#define CPRINTF(format, args...) cprintf(CC_SPI, format, ## args)

#if EMMC_SPI_PORT == 1
#define STM32_SPI_EMMC_REGS STM32_SPI1_REGS
#define STM32_DMAC_SPI_EMMC_TX STM32_DMAC_SPI1_TX
#define STM32_DMAC_SPI_EMMC_RX STM32_DMAC_SPI1_RX
#elif EMMC_SPI_PORT == 2
#define STM32_SPI_EMMC_REGS STM32_SPI2_REGS
#define STM32_DMAC_SPI_EMMC_TX STM32_DMAC_SPI2_TX
#define STM32_DMAC_SPI_EMMC_RX STM32_DMAC_SPI2_RX
#else
#error "Please define EMMC_SPI_PORT in board.h."
#endif

/* 1024 bytes circular buffer is enough for ~0.6ms @ 13Mhz. */
#define SPI_RX_BUF_BYTES 1024
#define SPI_RX_BUF_WORDS (SPI_RX_BUF_BYTES/4)
static uint32_t in_msg[SPI_RX_BUF_WORDS];

/* Macros to advance in the circular buffer. */
#define RX_BUF_NEXT_32(i) (((i) + 1) & (SPI_RX_BUF_WORDS - 1))
#define RX_BUF_DEC_32(i, j) (((i) - (j)) & (SPI_RX_BUF_WORDS - 1))
#define RX_BUF_PREV_32(i) RX_BUF_DEC_32((i), 1)

enum emmc_cmd {
	EMMC_ERROR = -1,
	EMMC_IDLE = 0,
	EMMC_PRE_IDLE,
	EMMC_BOOT,
};

static const struct dma_option dma_tx_option = {
	STM32_DMAC_SPI_EMMC_TX, (void *)&STM32_SPI_EMMC_REGS->dr,
	STM32_DMA_CCR_MSIZE_8_BIT | STM32_DMA_CCR_PSIZE_8_BIT
};

/* Circular RX buffer */
static const struct dma_option dma_rx_option = {
	STM32_DMAC_SPI_EMMC_RX, (void *)&STM32_SPI_EMMC_REGS->dr,
	STM32_DMA_CCR_MSIZE_8_BIT | STM32_DMA_CCR_PSIZE_8_BIT |
	STM32_DMA_CCR_CIRC
};

/* Setup DMA to transfer bootblock. */
static void bootblock_transfer(void)
{
	dma_chan_t *txdma = dma_get_channel(STM32_DMAC_SPI_EMMC_TX);

	dma_prepare_tx(&dma_tx_option, sizeof(bootblock_raw_data),
		       bootblock_raw_data);
	dma_go(txdma);

	CPRINTS("transfer");
}

/* Abort an ongoing transfer. */
static void bootblock_stop(void)
{
	dma_disable(STM32_DMAC_SPI_EMMC_TX);

	/*
	 * Wait a bit to for DMA to stop writing (we can't really wait for the
	 * buffer to get empty, as the bus may not be clocked anymore).
	 */
	udelay(100);

	/* Then flush SPI FIFO, and make sure DAT line stays idle (high). */
	STM32_SPI_EMMC_REGS->dr = 0xff;
	STM32_SPI_EMMC_REGS->dr = 0xff;
	STM32_SPI_EMMC_REGS->dr = 0xff;
	STM32_SPI_EMMC_REGS->dr = 0xff;
}

static enum emmc_cmd emmc_parse_command(int index)
{
	int32_t shift0;
	uint32_t data[3];

	if (in_msg[index] == 0xffffffff)
		return EMMC_ERROR;

	data[0] = htobe32(in_msg[index]);
	index = RX_BUF_NEXT_32(index);
	data[1] = htobe32(in_msg[index]);
	index = RX_BUF_NEXT_32(index);
	data[2] = htobe32(in_msg[index]);

	/* Figure out alignment (cmd starts with 01) */

	/* Number of leading ones. */
	shift0 = __builtin_clz(~data[0]);

	data[0] = (data[0] << shift0) | (data[1] >> (32-shift0));
	data[1] = (data[1] << shift0) | (data[2] >> (32-shift0));

	if (data[0] == 0x40000000 && data[1] == 0x0095ffff) {
		/* 400000000095 GO_IDLE_STATE */
		CPRINTS("goIdle");
		return EMMC_IDLE;
	}

	if (data[0] == 0x40f0f0f0 && data[1] == 0xf0fdffff) {
		/* 40f0f0f0f0fd GO_PRE_IDLE_STATE */
		CPRINTS("goPreIdle");
		return EMMC_PRE_IDLE;
	}

	if (data[0] == 0x40ffffff && data[1] == 0xfae5ffff) {
		/* 40fffffffae5 BOOT_INITIATION */
		CPRINTS("bootInit");
		return EMMC_BOOT;
	}

	CPRINTS("eMMC error");
	return EMMC_ERROR;
}


/*
 * Wake the EMMC task when there is a falling edge on the CMD line, so that we
 * can capture the command.
 */
void emmc_cmd_interrupt(enum gpio_signal signal)
{
	task_wake(TASK_ID_EMMC);
	CPRINTF("i");
}

static void emmc_init_spi(void)
{
#if EMMC_SPI_PORT == 1
	/* Reset SPI */
	STM32_RCC_APB2RSTR |= STM32_RCC_PB2_SPI1;
	STM32_RCC_APB2RSTR &= ~STM32_RCC_PB2_SPI1;

	/* Enable clocks to SPI module */
	STM32_RCC_APB2ENR |= STM32_RCC_PB2_SPI1;
#elif EMMC_SPI_PORT == 2
	/* Reset SPI */
	STM32_RCC_APB1RSTR |= STM32_RCC_PB1_SPI2;
	STM32_RCC_APB1RSTR &= ~STM32_RCC_PB1_SPI2;

	/* Enable clocks to SPI module */
	STM32_RCC_APB1ENR |= STM32_RCC_PB1_SPI2;
#else
#error "Please define EMMC_SPI_PORT in board.h."
#endif
	clock_wait_bus_cycles(BUS_APB, 1);
	gpio_config_module(MODULE_SPI, 1);

	STM32_SPI_EMMC_REGS->cr2 =
		STM32_SPI_CR2_FRXTH | STM32_SPI_CR2_DATASIZE(8) |
		STM32_SPI_CR2_RXDMAEN | STM32_SPI_CR2_TXDMAEN;

	/* Manual CS, disable. */
	STM32_SPI_EMMC_REGS->cr1 = STM32_SPI_CR1_SSM | STM32_SPI_CR1_SSI;

	/* Flush SPI FIFO, and make sure DAT line stays idle (high). */
	STM32_SPI_EMMC_REGS->dr = 0xff;
	STM32_SPI_EMMC_REGS->dr = 0xff;
	STM32_SPI_EMMC_REGS->dr = 0xff;
	STM32_SPI_EMMC_REGS->dr = 0xff;

	/* Enable the SPI peripheral */
	STM32_SPI_EMMC_REGS->cr1 |= STM32_SPI_CR1_SPE;
}

void emmc_task(void *u)
{
	int dma_pos, i;
	dma_chan_t *rxdma;
	enum emmc_cmd cmd;
	/* Are we currently transmitting data? */
	int tx = 0;

	emmc_init_spi();

	gpio_enable_interrupt(GPIO_EMMC_CMD);

	/* Start receiving in circular buffer in_msg. */
	rxdma = dma_get_channel(STM32_DMAC_SPI_EMMC_RX);
	dma_start_rx(&dma_rx_option, sizeof(in_msg), in_msg);

	/* Enable internal chip select. */
	STM32_SPI_EMMC_REGS->cr1 &= ~STM32_SPI_CR1_SSI;

	while (1) {
		/*
		 * TODO(b:110907438): After the bootblock has been transferred
		 * and AP has booted, disable SPI controller and interrupt.
		 */

		/* Wait for a command */
		task_wait_event(-1);

		dma_pos = dma_bytes_done(rxdma, sizeof(in_msg)) / 4;
		i = RX_BUF_PREV_32(dma_pos);

		/*
		 * By now, bus should be idle again (it takes <10us to transmit
		 * a command, less than is needed to process interrupt and wake
		 * this task).
		 */
		if (in_msg[i] != 0xffffffff) {
			CPRINTF("?");
			/* TODO(b:110907438): We should probably just retry. */
			continue;
		}

		/*
		 * Find a command, looking from the end of the buffer to make
		 * it faster.
		 */
		while (i != dma_pos && in_msg[i] == 0xffffffff)
			i = RX_BUF_PREV_32(i);

		/*
		 * We missed the command? That should not happen if we process
		 * the buffer quickly enough (and the interrupt was real).
		 */
		if (i == dma_pos) {
			CPRINTF("!");
			continue;
		}

		/*
		 * We found the end of the command, now find the beginning
		 * (commands are 6-byte long so the starting point is either 2
		 * or 1 word before the end of the command).
		 */
		i = RX_BUF_DEC_32(i, 2);
		if (in_msg[i] == 0xffffffff)
			i = RX_BUF_NEXT_32(i);

		cmd = emmc_parse_command(i);

		if (!tx) {
			/*
			 * When not transferring, host will send GO_IDLE_STATE,
			 * GO_PRE_IDLE_STATE, then BOOT_INITIATION commands. But
			 * all we really care about is the BOOT_INITIATION
			 * command: start the transfer.
			 */
			if (cmd == EMMC_BOOT) {
				tx = 1;
				bootblock_transfer();
			}
		} else {
			/*
			 * Host sends GO_IDLE_STATE to abort the transfer (e.g.
			 * when an incorrect number of lanes is used) and when
			 * the transfer is complete.
			 * Also react to GO_PRE_IDLE_STATE in case we missed
			 * GO_IDLE_STATE command.
			 */
			if (cmd == EMMC_IDLE || cmd == EMMC_PRE_IDLE) {
				bootblock_stop();
				tx = 0;
			}
		}
	}
}