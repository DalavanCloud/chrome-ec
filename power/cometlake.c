/* Copyright 2019 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* Cometlake chipset power control module for Chrome EC */

#include "cometlake.h"
#include "chipset.h"
#include "console.h"
#include "gpio.h"
#include "intel_x86.h"
#include "power.h"
#include "power_button.h"
#include "task.h"
#include "timer.h"

/* Console output macros */
#define CPRINTS(format, args...) cprints(CC_CHIPSET, format, ## args)

static int forcing_shutdown;  /* Forced shutdown in progress? */

void chipset_force_shutdown(enum chipset_shutdown_reason reason)
{
	int timeout_ms = 50;

	CPRINTS("%s(%d)", __func__, reason);
	report_ap_reset(reason);

	/* Turn off A (except PP5000_A) rails*/
	gpio_set_level(GPIO_EN_A_RAILS, 0);

	/* Turn off PP5000_A rail */
	gpio_set_level(GPIO_EN_PP5000_A, 0);

	/* Need to wait a min of 10 msec before check for power good */
	msleep(10);

	/*
	 * TODO(b/122264541): Replace this wait with
	 * power_wait_signals_timeout()
	 */
	/* Now wait for PP5000_A and RSMRST_L to go low */
	while ((gpio_get_level(GPIO_PP5000_A_PG_OD) ||
		power_has_signals(IN_PGOOD_ALL_CORE)) && (timeout_ms > 0)) {
		msleep(1);
		timeout_ms--;
	};

	if (!timeout_ms)
		CPRINTS("PP5000_A rail still up!  Assuming G3.");
}

void chipset_handle_espi_reset_assert(void)
{
	/*
	 * If eSPI_Reset# pin is asserted without SLP_SUS# being asserted, then
	 * it means that there is an unexpected power loss (global reset
	 * event). In this case, check if shutdown was being forced by pressing
	 * power button. If yes, release power button.
	 */
	if ((power_get_signals() & IN_PGOOD_ALL_CORE) && forcing_shutdown) {
		power_button_pch_release();
		forcing_shutdown = 0;
	}
}

enum power_state chipset_force_g3(void)
{
	chipset_force_shutdown(CHIPSET_SHUTDOWN_G3);

	return POWER_G3;
}

/* Called by APL power state machine when transitioning from G3 to S5 */
void chipset_pre_init_callback(void)
{
	/*
	 * TODO (b/122265772): Need to use CONFIG_POWER_PP5000_CONTROL, but want
	 * to do that after some refactoring so that more than 1 signal can be
	 * tracked if necessary.
	 */

	/* Enable 5.0V and 3.3V rails, and wait for Power Good */
	/* Turn on PP5000_A rail */
	gpio_set_level(GPIO_EN_PP5000_A, 1);
	/* Turn on A (except PP5000_A) rails*/
	gpio_set_level(GPIO_EN_A_RAILS, 1);

	/* Ensure that PP5000_A rail is stable */
	while (!gpio_get_level(GPIO_PP5000_A_PG_OD))
		;

}

enum power_state power_handle_state(enum power_state state)
{

	int all_sys_pwrgd_in;
	int all_sys_pwrgd_out;

	common_intel_x86_handle_rsmrst(state);

	switch (state) {

	case POWER_S5:
		if (forcing_shutdown) {
			power_button_pch_release();
			forcing_shutdown = 0;
		}
		/* If RSMRST_L is asserted, we're no longer in S5. */
		if (!power_has_signals(IN_PGOOD_ALL_CORE))
			return POWER_S5G3;
		break;

	case POWER_S0:
		/*
		 * Check value of PG_EC_ALL_SYS_PWRGD to see if EC_PCH_SYS_PWROK
		 * needs to be changed. If it's low->high transition, requires a
		 * 2msec delay.
		 */
		all_sys_pwrgd_in = gpio_get_level(GPIO_PG_EC_ALL_SYS_PWRGD);
		all_sys_pwrgd_out = gpio_get_level(GPIO_EC_PCH_SYS_PWROK);

		if (all_sys_pwrgd_in != all_sys_pwrgd_out) {
			if (all_sys_pwrgd_in)
				msleep(2);
			gpio_set_level(GPIO_EC_PCH_SYS_PWROK, all_sys_pwrgd_in);
		}
		break;

	default:
		break;
	}

	return common_intel_x86_power_handle_state(state);
}
