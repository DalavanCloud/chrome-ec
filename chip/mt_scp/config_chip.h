/* Copyright 2018 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CONFIG_CHIP_H
#define __CROS_EC_CONFIG_CHIP_H

#include "core/cortex-m/config_core.h"

/* Interval between HOOK_TICK notifications */
#define HOOK_TICK_INTERVAL_MS 500
#define HOOK_TICK_INTERVAL    (HOOK_TICK_INTERVAL_MS * MSEC)

/* Default to UART 2 (AP UART) for EC console */
#define CONFIG_UART_CONSOLE 2

/* Number of IRQ vectors */
#define CONFIG_IRQ_COUNT 56

/*
 * Number of EINT can be 0 ~ 160. Change this to conditional macro
 * on adding other variants.
 */
#define MAX_NUM_EINT 8
#define MAX_EINT_PORT (MAX_NUM_EINT / 32)

/* RW only, no flash
 * +-------------------- 0x0
 * | ptr to stack_top  0x0
 * | ptr to reset func 0x04
 * |-------------------- 0x08
 * | free shared space with AP
 * +-------------------- 0x005B0
 * | IPI shared buffer with AP (288 + 8) * 2
 * +-------------------- 0x00800
 * | scp.img, exception vectors starting location.
 * +-------------------- 0x7B800
 * | free shared space with AP 2KB
 * +-------------------- 0x7C000
 * | 8KB I-CACHE
 * +-------------------- 0x7E000
 * | 8KB D-CACHE
 * +-------------------- 0x80000
 */
#undef  CONFIG_FW_INCLUDE_RO
#define CONFIG_RAM_BASE 0x00800
#define CONFIG_RAM_SIZE 0x7B000
#define CONFIG_RO_MEM_OFF 0
#define CONFIG_RO_SIZE 0
#define CONFIG_RW_MEM_OFF 0
#define CONFIG_RW_SIZE 0x40000 /* 256KB */
#define CONFIG_EC_WRITABLE_STORAGE_OFF 0
#define CONFIG_EC_PROTECTED_STORAGE_OFF 0
#define CONFIG_RO_STORAGE_OFF 0
#define CONFIG_RW_STORAGE_OFF 0
#define CONFIG_PROGRAM_MEMORY_BASE 0
#define CONFIG_MAPPED_STORAGE_BASE 0
#define CONFIG_CHIP_MEMORY_REGIONS

/* Unsupported features/commands */
#undef CONFIG_CMD_FLASHINFO
#undef CONFIG_CMD_POWER_AP
#undef CONFIG_FLASH
#undef CONFIG_FLASH_PHYSICAL
#undef CONFIG_FMAP
#undef CONFIG_HIBERNATE

/* Task stack size */
#define CONFIG_STACK_SIZE 1024
#define IDLE_TASK_STACK_SIZE 256
#define SMALLER_TASK_STACK_SIZE 384
#define TASK_STACK_SIZE 488
#define LARGER_TASK_STACK_SIZE 640
#define VENTI_TASK_STACK_SIZE 768

/* IPI */
#define CONFIG_IPC_SHARED_OBJ_BUF_SIZE 288
#define CONFIG_IPC_SHARED_OBJ_ADDR                                             \
	(CONFIG_RAM_BASE -                                                     \
	 (CONFIG_IPC_SHARED_OBJ_BUF_SIZE + 2 * sizeof(int32_t)) * 2)

#define CONFIG_CHIP_PRE_INIT

#define GPIO_PIN(num) ((num) / 32), ((num) % 32)
#define GPIO_PIN_MASK(p, m) .port = (p), .mask = (m)

#endif /* __CROS_EC_CONFIG_CHIP_H */
