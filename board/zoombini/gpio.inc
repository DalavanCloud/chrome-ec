/* -*- mode:c -*-
 *
 * Copyright 2017 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* Declare symbolic names for all the GPIOs that we care about.
 * Note: Those with interrupt handlers must be declared first. */

/* Zoombini GPIOs */
#ifdef BOARD_ZOOMBINI
GPIO_INT(USB_C0_PD_INT_L, PIN(6, 1), GPIO_INT_FALLING, tcpc_alert_event)
GPIO_INT(USB_C1_PD_INT_L, PIN(F, 5), GPIO_INT_FALLING, tcpc_alert_event)
GPIO_INT(USB_C2_PD_INT_L, PIN(9, 5), GPIO_INT_FALLING, tcpc_alert_event)

GPIO_INT(USB_C0_PPC_INT_L, PIN(0, 3), GPIO_INT_FALLING, ppc_interrupt)
GPIO_INT(USB_C1_PPC_INT_L, PIN(0, 4), GPIO_INT_FALLING, ppc_interrupt)
GPIO_INT(USB_C2_PPC_INT_L, PIN(4, 0), GPIO_INT_FALLING, ppc_interrupt)

GPIO_INT(POWER_BUTTON_L,  PIN(0, 1), GPIO_INT_BOTH | GPIO_PULL_UP, power_button_interrupt)
GPIO_INT(WP_L,            PIN(A, 1), GPIO_INT_BOTH, switch_interrupt)
GPIO_INT(LID_OPEN,        PIN(D, 2), GPIO_INT_BOTH | GPIO_HIB_WAKE_HIGH, lid_interrupt)
GPIO_INT(AC_PRESENT,      PIN(0, 0), GPIO_INT_BOTH | GPIO_HIB_WAKE_HIGH, extpower_interrupt)

/* Power Signals. */
GPIO_INT(PCH_SLP_S0_L,    PIN(A, 4), GPIO_INT_BOTH, power_signal_interrupt)
#ifndef CONFIG_ESPI_VW_SIGNALS
GPIO_INT(PCH_SLP_S3_L,    PIN(A, 6), GPIO_INT_BOTH, power_signal_interrupt)
GPIO_INT(PCH_SLP_S4_L,    PIN(A, 3), GPIO_INT_BOTH, power_signal_interrupt)
#endif /* defined(CONFIG_ESPI_VW_SIGNALS) */
GPIO_INT(PCH_SLP_SUS_L,   PIN(D, 4), GPIO_INT_BOTH, power_signal_interrupt)
/* TODO(aaboagye): Internal PU may be needed later on... */
GPIO_INT(RSMRST_L_PGOOD,  PIN(E, 2), GPIO_INT_BOTH, power_signal_interrupt)
GPIO_INT(PMIC_DPWROK,     PIN(F, 4), GPIO_INT_BOTH, power_signal_interrupt)
GPIO_INT(PMIC_INT_L,      PIN(D, 5), GPIO_INT_FALLING | GPIO_PULL_UP, power_signal_interrupt)
GPIO_INT(PP5000_PGOOD,    PIN(C, 0), GPIO_INT_BOTH, power_signal_interrupt)
GPIO_INT(VOLUME_UP_L,     PIN(7, 5), GPIO_INT_BOTH | GPIO_PULL_UP, button_interrupt)
GPIO_INT(VOLUME_DOWN_L,   PIN(4, 1), GPIO_INT_BOTH | GPIO_PULL_UP, button_interrupt)

/* Power Enables. */
GPIO(EN_PP3300_DSW,        PIN(6, 0), GPIO_OUT_LOW)
GPIO(EN_PP5000,            PIN(7, 3), GPIO_OUT_LOW)

GPIO(ENTERING_RW,         PIN(E, 1), GPIO_OUT_LOW)
GPIO(CCD_MODE_ODL,        PIN(E, 3), GPIO_INPUT)  /* Case Closed Debug mode. */
GPIO(KB_BL_EN,            PIN(8, 6), GPIO_OUT_LOW)
GPIO(PMIC_EN,             PIN(D, 7), GPIO_ODR_LOW)
GPIO(ENABLE_BACKLIGHT,    PIN(D, 3), GPIO_ODR_LOW) /* EC_BL_EN_OD */

GPIO(PCH_DSW_PWROK,    PIN(3, 7), GPIO_OUT_LOW)  /* EC_PCH_DSW_PWROK */
GPIO(PCH_RSMRST_L,     PIN(C, 2), GPIO_OUT_LOW)  /* EC_PCH_RSMRST_L */
GPIO(PCH_RTCRST,       PIN(7, 6), GPIO_OUT_LOW)  /* EC_PCH_RTCRST */
GPIO(EN_TOP_SWAP,      PIN(B, 7), GPIO_INPUT)    /* GPP_B14_STRAP */
GPIO(PCH_PWRBTN_L,     PIN(C, 1), GPIO_ODR_HIGH) /* EC_PCH_PWR_BTN_ODL */
GPIO(PCH_WAKE_L,       PIN(7, 4), GPIO_ODR_HIGH) /* EC_PCH_WAKE_ODL */
GPIO(CPU_PROCHOT,      PIN(3, 4), GPIO_INPUT)    /* PCH_PROCHOT_ODL */
GPIO(SYS_RESET_L,      PIN(0, 2), GPIO_ODR_HIGH) /* SYS_RST_ODL */

GPIO(I2C0_SCL,   PIN(B, 5), GPIO_INPUT)
GPIO(I2C0_SDA,   PIN(B, 4), GPIO_INPUT)
GPIO(I2C1_SCL,   PIN(9, 0), GPIO_INPUT)
GPIO(I2C1_SDA,   PIN(8, 7), GPIO_INPUT)
GPIO(I2C2_SCL,   PIN(9, 2), GPIO_INPUT)
GPIO(I2C2_SDA,   PIN(9, 1), GPIO_INPUT)
GPIO(I2C3_SCL,   PIN(D, 1), GPIO_INPUT)
GPIO(I2C3_SDA,   PIN(D, 0), GPIO_INPUT)
GPIO(I2C4_SCL,   PIN(F, 3), GPIO_INPUT)
GPIO(I2C4_SDA,   PIN(F, 2), GPIO_INPUT)
GPIO(I2C5_SCL,   PIN(3, 3), GPIO_INPUT)
GPIO(I2C5_SDA,   PIN(3, 6), GPIO_INPUT)
GPIO(I2C7_SCL,   PIN(B, 3), GPIO_INPUT)
GPIO(I2C7_SDA,   PIN(B, 2), GPIO_INPUT)

GPIO(BAT_PRESENT_L, PIN(E, 5), GPIO_INPUT)
GPIO(USB_PD_RST_L,  PIN(F, 1), GPIO_ODR_HIGH)

GPIO(USB_A_5V_EN,         PIN(A, 0), GPIO_ODR_LOW)
GPIO(USB_A_HIGH_POWER_EN, PIN(A, 2), GPIO_ODR_LOW)

GPIO(USB_C0_BC12_VBUS_ON_L, PIN(8, 2), GPIO_ODR_HIGH)
GPIO(USB_C1_BC12_VBUS_ON_L, PIN(B, 1), GPIO_ODR_HIGH)
GPIO(USB_C2_BC12_VBUS_ON_L, PIN(E, 0), GPIO_ODR_HIGH)
GPIO(USB_C0_BC12_CHG_DET, PIN(6, 2), GPIO_INPUT)
GPIO(USB_C1_BC12_CHG_DET, PIN(8, 3), GPIO_INPUT)
GPIO(USB_C2_BC12_CHG_DET, PIN(E, 4), GPIO_INPUT)
GPIO(USB_C0_OC_L,         PIN(6, 7), GPIO_ODR_HIGH)
GPIO(USB_C1_OC_L,         PIN(7, 0), GPIO_ODR_HIGH)
GPIO(USB_C2_OC_L,         PIN(6, 3), GPIO_ODR_HIGH)

GPIO(BOARD_VERSION1, PIN(9, 6), GPIO_INPUT)
GPIO(BOARD_VERSION2, PIN(9, 3), GPIO_INPUT)
GPIO(BOARD_VERSION3, PIN(F, 0), GPIO_INPUT)

/* UART Pins */
ALTERNATE(PIN_MASK(6, 0x30), 0, MODULE_UART, GPIO_PULL_UP)
/* I2C Ports */
ALTERNATE(PIN_MASK(B, 0x30), 1, MODULE_I2C, 0) /* I2C0 */
ALTERNATE(PIN_MASK(9, 0x06), 1, MODULE_I2C, 0) /* I2C2 */
ALTERNATE(PIN_MASK(9, 0x01), 1, MODULE_I2C, 0) /* I2C1 SDA */
ALTERNATE(PIN_MASK(8, 0x80), 1, MODULE_I2C, 0) /* I2C1 SCL */
ALTERNATE(PIN_MASK(D, 0x03), 1, MODULE_I2C, 0) /* I2C3 */
ALTERNATE(PIN_MASK(3, 0x48), 1, MODULE_I2C, 0) /* I2C5 */
ALTERNATE(PIN_MASK(B, 0x0C), 1, MODULE_I2C, 0) /* I2C7 */
ALTERNATE(PIN_MASK(F, 0x0C), 1, MODULE_I2C, 0) /* I2C4 */

/* Keyboard Pins */
/* KSI_00-01 */
ALTERNATE(PIN_MASK(3, 0x03), 0, MODULE_KEYBOARD_SCAN, GPIO_INPUT)
/* KSI_02-07 */
ALTERNATE(PIN_MASK(2, 0xFC), 0, MODULE_KEYBOARD_SCAN, GPIO_INPUT)
/* KSO_00-01 */
ALTERNATE(PIN_MASK(2, 0x03), 0, MODULE_KEYBOARD_SCAN, GPIO_ODR_HIGH)
/* KSO_02 inverted */
GPIO(KBD_KSO2, PIN(1, 7), GPIO_OUT_LOW)
/* KSO_03-09 */
ALTERNATE(PIN_MASK(1, 0x7F), 0, MODULE_KEYBOARD_SCAN, GPIO_ODR_HIGH)
/* KSO_10-12 */
ALTERNATE(PIN_MASK(0, 0xE0), 0, MODULE_KEYBOARD_SCAN, GPIO_ODR_HIGH)

/* PWMs. */
ALTERNATE(PIN_MASK(C, 0x18), 1, MODULE_PWM, 0) /* LED 1 & 2 */
ALTERNATE(PIN_MASK(8, 0x01), 1, MODULE_PWM, 0) /* KB Backlight */

/* ADC */
ALTERNATE(PIN_MASK(4, 0x30), 1, MODULE_ADC, 0) /* ADC0-1 */

/* Power Switch Logic (PSL) inputs */
ALTERNATE(PIN_MASK(0, 0x03), 1, MODULE_PMU, 0) /* GPIO00, GPIO01 */
ALTERNATE(PIN_MASK(D, 0x04), 1, MODULE_PMU, 0) /* GPIOD2 */

/* Meowth GPIOs */
#else

GPIO_INT(USB_C0_PD_INT_L, PIN(6, 1), GPIO_INT_FALLING, usb_c_interrupt)
GPIO_INT(USB_C1_PD_INT_L, PIN(F, 5), GPIO_INT_FALLING, usb_c_interrupt)

GPIO_INT(POWER_BUTTON_L,  PIN(0, 1), GPIO_INT_BOTH | GPIO_PULL_UP, power_button_interrupt)
GPIO_INT(WP_L,            PIN(A, 1), GPIO_INT_BOTH, switch_interrupt)
GPIO_INT(LID_OPEN,        PIN(D, 2), GPIO_INT_BOTH | GPIO_HIB_WAKE_HIGH, lid_interrupt)
GPIO_INT(AC_PRESENT,      PIN(0, 0), GPIO_INT_BOTH | GPIO_HIB_WAKE_HIGH, extpower_interrupt)

/* Power Signals. */
GPIO_INT(PCH_SLP_S0_L,    PIN(A, 4), GPIO_INT_BOTH, power_signal_interrupt)
GPIO_INT(PCH_SLP_S3_L,    PIN(A, 6), GPIO_INT_BOTH, power_signal_interrupt)
GPIO_INT(PCH_SLP_S4_L,    PIN(A, 3), GPIO_INT_BOTH, power_signal_interrupt)
GPIO_INT(PCH_SLP_SUS_L,   PIN(D, 4), GPIO_INT_BOTH, power_signal_interrupt)
/* TODO(aaboagye): Internal PU may be needed later on... */
GPIO_INT(RSMRST_L_PGOOD,  PIN(E, 2), GPIO_INT_BOTH, power_signal_interrupt)
GPIO_INT(PMIC_DPWROK,     PIN(F, 4), GPIO_INT_BOTH, power_signal_interrupt)
GPIO_INT(PMIC_INT_L,      PIN(D, 5), GPIO_INT_FALLING | GPIO_PULL_UP, power_signal_interrupt)
GPIO_INT(VOLUME_UP_L,     PIN(7, 5), GPIO_INT_BOTH | GPIO_PULL_UP, button_interrupt)
GPIO_INT(VOLUME_DOWN_L,   PIN(6, 3), GPIO_INT_BOTH | GPIO_PULL_UP, button_interrupt)

/* Power Enables. */
GPIO(EN_PP3300_DSW,        PIN(8, 2), GPIO_OUT_LOW)
GPIO(EN_PP5000,            PIN(7, 3), GPIO_OUT_LOW)
GPIO(EN_PP1800_U,          PIN(A, 0), GPIO_OUT_LOW)
GPIO(EN_PP3300_WLAN,       PIN(8, 3), GPIO_OUT_HIGH)

GPIO(ENTERING_RW,         PIN(E, 1), GPIO_OUT_LOW)
GPIO(CCD_MODE_ODL,        PIN(E, 3), GPIO_INPUT)  /* Case Closed Debug mode. */
GPIO(PMIC_EN,             PIN(8, 6), GPIO_ODR_LOW)
GPIO(USB_C0_OC_L,         PIN(6, 7), GPIO_ODR_HIGH)
GPIO(USB_C1_OC_L,         PIN(7, 0), GPIO_ODR_HIGH)
GPIO(ENABLE_BACKLIGHT,    PIN(D, 3), GPIO_ODR_LOW) /* EC_BL_EN_OD */

GPIO(PCH_DSW_PWROK,    PIN(3, 7), GPIO_OUT_LOW)  /* EC_PCH_DSW_PWROK */
GPIO(PCH_RSMRST_L,     PIN(C, 2), GPIO_OUT_LOW)  /* EC_PCH_RSMRST_L */
GPIO(PCH_RTCRST,       PIN(7, 6), GPIO_OUT_LOW)  /* EC_PCH_RTCRST */
GPIO(PCH_PWRBTN_L,     PIN(C, 1), GPIO_ODR_HIGH) /* EC_PCH_PWR_BTN_ODL */
GPIO(PCH_WAKE_L,       PIN(7, 4), GPIO_ODR_HIGH) /* EC_PCH_WAKE_ODL */
GPIO(CPU_PROCHOT,      PIN(3, 4), GPIO_INPUT)    /* PCH_PROCHOT_ODL */
GPIO(SYS_RESET_L,      PIN(0, 2), GPIO_ODR_HIGH) /* SYS_RST_ODL */

GPIO(I2C0_SCL,   PIN(B, 5), GPIO_INPUT)
GPIO(I2C0_SDA,   PIN(B, 4), GPIO_INPUT)
GPIO(I2C1_SCL,   PIN(9, 0), GPIO_INPUT)
GPIO(I2C1_SDA,   PIN(8, 7), GPIO_INPUT)
GPIO(I2C3_SCL,   PIN(D, 1), GPIO_INPUT)
GPIO(I2C3_SDA,   PIN(D, 0), GPIO_INPUT)
GPIO(I2C4_SCL,   PIN(F, 3), GPIO_INPUT)
GPIO(I2C4_SDA,   PIN(F, 2), GPIO_INPUT)
GPIO(I2C5_SCL,   PIN(3, 3), GPIO_INPUT)
GPIO(I2C5_SDA,   PIN(3, 6), GPIO_INPUT)
GPIO(I2C7_SCL,   PIN(B, 3), GPIO_INPUT)
GPIO(I2C7_SDA,   PIN(B, 2), GPIO_INPUT)

GPIO(BAT_PRESENT_L, PIN(E, 5), GPIO_INPUT)
GPIO(USB_PD_RST_L,  PIN(6, 2), GPIO_ODR_HIGH)

GPIO(BOARD_VERSION1, PIN(9, 6), GPIO_INPUT)
GPIO(BOARD_VERSION2, PIN(9, 3), GPIO_INPUT)
GPIO(BOARD_VERSION3, PIN(A, 2), GPIO_INPUT)

/* UART Pins */
ALTERNATE(PIN_MASK(6, 0x30), 0, MODULE_UART, GPIO_PULL_UP)
/* I2C Ports */
ALTERNATE(PIN_MASK(B, 0x30), 1, MODULE_I2C, 0) /* I2C0 */
ALTERNATE(PIN_MASK(9, 0x01), 1, MODULE_I2C, 0) /* I2C1 SDA */
ALTERNATE(PIN_MASK(8, 0x80), 1, MODULE_I2C, 0) /* I2C1 SCL */
ALTERNATE(PIN_MASK(D, 0x03), 1, MODULE_I2C, 0) /* I2C3 */
ALTERNATE(PIN_MASK(3, 0x48), 1, MODULE_I2C, 0) /* I2C5 */
ALTERNATE(PIN_MASK(B, 0x0C), 1, MODULE_I2C, 0) /* I2C7 */
ALTERNATE(PIN_MASK(F, 0x0C), 1, MODULE_I2C, 0) /* I2C4 */

/* PWMs. */
ALTERNATE(PIN_MASK(C, 0x18), 1, MODULE_PWM, 0) /* DB0 Green/Blue LED (M) */
ALTERNATE(PIN_MASK(8, 0x01), 1, MODULE_PWM, 0) /* DB0 Red LED (M) */
ALTERNATE(PIN_MASK(6, 0x01), 1, MODULE_PWM, 0) /* DB1 Red LED (M) */
ALTERNATE(PIN_MASK(B, 0x80), 1, MODULE_PWM, 0) /* DB1 Green LED (M) */
ALTERNATE(PIN_MASK(C, 0x01), 1, MODULE_PWM, 0) /* DB1 Blue LED (M) */

/* ADC */
ALTERNATE(PIN_MASK(4, 0x32), 1, MODULE_ADC, 0) /* ADC0-1,4 */
ALTERNATE(PIN_MASK(F, 0x02), 1, MODULE_ADC, 0) /* ADC8 */

/* Power Switch Logic (PSL) inputs */
ALTERNATE(PIN_MASK(0, 0x03), 1, MODULE_PMU, 0) /* GPIO00, GPIO01 */
ALTERNATE(PIN_MASK(D, 0x04), 1, MODULE_PMU, 0) /* GPIOD2 */

#endif /* defined(BOARD_ZOOMBINI) */