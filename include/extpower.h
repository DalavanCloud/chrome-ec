/* Copyright (c) 2013 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* External power detection API for Chrome EC */

#ifndef __CROS_EC_EXTPOWER_H
#define __CROS_EC_EXTPOWER_H

#include "common.h"

/**
 * Return non-zero if external power is present.
 */
int extpower_is_present(void);

/**
 * Interrupt handler for external power GPIOs.
 *
 * @param signal	Signal which triggered the interrupt.
 */
void extpower_interrupt(enum gpio_signal signal);

/**
 * Return true if inrush current might happen, thus, battery learning mode
 * needs to be enabled.
 */
int extpower_is_inrush_protection_needed(void);

#endif  /* __CROS_EC_EXTPOWER_H */
