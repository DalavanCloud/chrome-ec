/* Copyright 2018 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * O2 Micro OZ554 LED driver.
 */

#include "console.h"
#include "gpio.h"
#include "hooks.h"
#include "i2c.h"
#include "task.h"
#include "timer.h"

#define CPRINTS(format, args...) cprints(CC_I2C, format, ## args)
#define CPRINTF(format, args...) cprintf(CC_I2C, format, ## args)

#define I2C_ADDR_OZ554		0x62
#define OZ554_DATA_SIZE		6

struct oz554_value {
	uint8_t offset;
	uint8_t data;
};

/*
 * OZ554ALN asserts the interrupt when it's ready for writing settings, which
 * are cleared when it's turned off. We enable the interrupt on HOOK_INIT and
 * keep it enabled in S0/S3/S5.
 *
 * It's assumed the device doesn't have a lid and OZ554ALN is powered only in
 * S0. For clamshell devices, different interrupt & power control scheme may be
 * needed.
 */

/* This ordering is suggested by vendor. */
static const struct oz554_value order[] = {
	/*
	 * Reigster 0x01: Operation frequency control
	 * Frequency selection: 300(KHz)
	 * Short circuit protection: 8(V)
	 */
	{.offset = 1, .data = 0x43},
	/*
	 * Reigster 0x02: LED current amplitude control
	 * ISET Resistor: 10.2(Kohm)
	 * Maximum LED current: 1636/10.2 = 160.4(mA)
	 * Setting LED current: 65(mA)
	 */
	{.offset = 2, .data = 0x65},
	/*
	 * Reigster 0x03: LED backlight Status
	 * Status function: Read only
	 */
	{.offset = 3, .data = 0x00},
	/*
	 * Reigster 0x04: LED current control with SMBus
	 * SMBus PWM function: None Use
	 */
	{.offset = 4, .data = 0x00},
	/*
	 * Reigster 0x05: OVP, OCP control
	 * Over Current Protection: 0.5(V)
	 * Panel LED Voltage(Max): 47.8(V)
	 * OVP setting: 54(V)
	 */
	{.offset = 5, .data = 0x97},
	/*
	 * Reigster 0x00: Dimming mode and string ON/OFF control
	 * String Selection: 4(Number)
	 * Interface Selection: 1
	 * Brightness mode: 3
	 */
	{.offset = 0, .data = 0xF2},
};
BUILD_ASSERT(ARRAY_SIZE(order) == OZ554_DATA_SIZE);

static void set_oz554_reg(void)
{
	int i;

	for (i = 0; i < OZ554_DATA_SIZE; ++i) {
		int rv = i2c_write8(I2C_PORT_BACKLIGHT, I2C_ADDR_OZ554,
				    order[i].offset, order[i].data);
		if (rv) {
			CPRINTS("Write OZ554 register %d failed rv=%d" , i, rv);
			return;
		}
	}
	CPRINTS("Wrote OZ554 settings");
}

static void backlight_enable_deferred(void)
{
	if (gpio_get_level(GPIO_PANEL_BACKLIGHT_EN))
		set_oz554_reg();
}
DECLARE_DEFERRED(backlight_enable_deferred);

void backlight_enable_interrupt(enum gpio_signal signal)
{
	hook_call_deferred(&backlight_enable_deferred_data, 30 * MSEC);
}

static void init_oz554(void)
{
	gpio_enable_interrupt(GPIO_PANEL_BACKLIGHT_EN);
}
DECLARE_HOOK(HOOK_INIT, init_oz554, HOOK_PRIO_DEFAULT);
