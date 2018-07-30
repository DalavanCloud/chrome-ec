/* -*- mode:c -*-
 *
 * Copyright 2018 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * Declare symbolic names for all the GPIOs that we care about.
 * Note: Those with interrupt handlers must be declared first.
 */

/* Interrupts */
GPIO_INT(SPI1_NSS,          PIN(A, 15), GPIO_INT_BOTH,
         spi_event)
GPIO_INT(USB_C0_PD_INT_ODL, PIN(B, 1), GPIO_INT_FALLING | GPIO_PULL_UP,
         tcpc_alert_event)
GPIO_INT(VOLUME_UP_L,       PIN(B, 10), GPIO_INT_BOTH | GPIO_PULL_UP,
         button_interrupt) /* EC_VOLUP_BTN_ODL */
GPIO_INT(VOLUME_DOWN_L,     PIN(B, 11), GPIO_INT_BOTH | GPIO_PULL_UP,
         button_interrupt) /* EC_VOLDN_BTN_ODL */
GPIO_INT(POWER_BUTTON_L,    PIN(A, 0),  GPIO_INT_BOTH | GPIO_PULL_UP,
         power_button_interrupt) /* EC_PWR_BTN_ODL */

GPIO_INT(AP_IN_SLEEP_L,     PIN(C, 12), GPIO_INT_BOTH | GPIO_PULL_DOWN,
         power_signal_interrupt)
GPIO_INT(PMIC_EC_RESETB,    PIN(B, 7),  GPIO_INT_BOTH | GPIO_PULL_DOWN,
         power_signal_interrupt)
GPIO_INT(WARM_RESET_REQ,    PIN(A, 3),  GPIO_INT_RISING | GPIO_PULL_DOWN,
         warm_reset_request_interrupt)
GPIO_INT(AP_EC_WATCHDOG_L,  PIN(C, 2),  GPIO_INT_FALLING,
         ap_watchdog_interrupt)

GPIO_INT(ACCEL_INT_ODL,     PIN(A, 4), GPIO_INT_FALLING | GPIO_SEL_1P8V | GPIO_PULL_UP,
         bmi160_interrupt)
GPIO_INT(SYNC_INT,          PIN(A, 5),  GPIO_INT_RISING | GPIO_PULL_DOWN,
         sync_interrupt)
GPIO_INT(CHARGER_INT_ODL,   PIN(C, 13), GPIO_INPUT | GPIO_PULL_UP,
         rt946x_interrupt)
#ifdef SECTION_IS_RO
GPIO_INT(EMMC_CMD,          PIN(A, 14), GPIO_INT_FALLING,
         emmc_cmd_interrupt)
#endif

/* Voltage rails control pins */
GPIO(PP3300_S0_EN,         PIN(B, 6),  GPIO_OUT_LOW)
GPIO(PP1800_S3_EN,         PIN(C, 7),  GPIO_OUT_LOW)
GPIO(PP3300_S3_EN,         PIN(A, 8),  GPIO_OUT_LOW)

/* Reset pins */
GPIO(AP_SYS_RST_L,         PIN(C, 11), GPIO_ODR_LOW)
GPIO(PMIC_WATCHDOG_L,      PIN(C, 3),  GPIO_OUT_LOW)
GPIO(PMIC_EN_ODL,          PIN(C, 10), GPIO_ODR_HIGH)
GPIO(PMIC_FORCE_RESET,     PIN(A, 2),  GPIO_OUT_LOW)
GPIO(MT6370_RST_L,         PIN(F, 0),  GPIO_OUT_LOW)

/*
 * I2C pins should be configured as inputs until I2C module is
 * initialized. This will avoid driving the lines unintentionally.
 */
GPIO(I2C1_SCL,             PIN(B, 8),  GPIO_INPUT)
GPIO(I2C1_SDA,             PIN(B, 9),  GPIO_INPUT)
GPIO(I2C2_SCL,             PIN(A, 11), GPIO_INPUT)
GPIO(I2C2_SDA,             PIN(A, 12), GPIO_INPUT)

/* Analog pins */
GPIO(BOARD_ID,             PIN(C, 0),  GPIO_ANALOG)

/* Other input pins */
GPIO(WP_L,                 PIN(C, 8),  GPIO_INPUT) /* EC_FLASH_WP_ODL */
GPIO(CCD_MODE_ODL,         PIN(C, 9),  GPIO_INPUT)
GPIO(BOOT0,                PIN(F, 11), GPIO_INPUT)

/* Other output pins */
GPIO(ENTERING_RW,          PIN(C, 6),  GPIO_ODR_HIGH) /* EC_ENTERING_RW_ODL */
GPIO(EC_INT_L,             PIN(B, 12), GPIO_ODR_HIGH) /* EC_AP_INT_ODL */
GPIO(EC_BOARD_ID_EN_L,     PIN(C, 15), GPIO_ODR_HIGH) /* EC_BOARD_ID_EN_ODL */
GPIO(USB_C0_DISCHARGE,     PIN(B, 0),  GPIO_OUT_LOW)
GPIO(USB_C0_DP_OE_L,       PIN(C, 5),  GPIO_OUT_HIGH)
GPIO(USB_C0_DP_POLARITY,   PIN(C, 14), GPIO_OUT_LOW)
GPIO(USB_C0_HPD_OD,        PIN(F, 1),  GPIO_OUT_LOW)
GPIO(OTG_EN,               PIN(A, 6),  GPIO_OUT_LOW)
GPIO(BOOTBLOCK_MUX_OE,     PIN(C, 4),  GPIO_ODR_HIGH)
GPIO(BOOTBLOCK_EN_L,       PIN(C, 1),  GPIO_ODR_HIGH)

/* USART1: PA9/PA10 */
ALTERNATE(PIN_MASK(A, 0x0600), 1, MODULE_UART,       0)
/* I2C MASTER: PB8/9 */
ALTERNATE(PIN_MASK(B, 0x0300), 1, MODULE_I2C,        GPIO_ODR_HIGH | GPIO_PULL_UP)
/* I2C MASTER: PA11/12 */
ALTERNATE(PIN_MASK(B, 0x1800), 5, MODULE_I2C,        GPIO_ODR_HIGH | GPIO_PULL_UP)
/* SPI SLAVE: PB3/4/5 */
ALTERNATE(PIN_MASK(B, 0x0038), 0, MODULE_SPI,        0)
#ifdef SECTION_IS_RO
/* SPI SLAVE: PB13/14/15 */
ALTERNATE(PIN_MASK(B, 0xE000), 0, MODULE_SPI,        0)
#endif
/* SPI SLAVE CS: PA15 */
ALTERNATE(PIN_MASK(A, 0x8000), 0, MODULE_SPI,        0)