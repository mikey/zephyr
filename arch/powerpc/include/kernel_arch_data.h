/*
 * Copyright (c) 2016 Jean-Paul Etienne <fractalclone@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Private kernel definitions
 *
 * This file contains private kernel structures definitions and various
 * other definitions for the PowerPC processor architecture.
 */

#ifndef ZEPHYR_ARCH_POWERPC_INCLUDE_KERNEL_ARCH_DATA_H_
#define ZEPHYR_ARCH_POWERPC_INCLUDE_KERNEL_ARCH_DATA_H_

#include <toolchain.h>
#include <linker/sections.h>
#include <arch/cpu.h>
#include <kernel_arch_thread.h>

/* Can we reduce this to 8 assuming we aren't using VMX/VSX? */
#define STACK_ALIGN_SIZE	8

#ifndef _ASMLANGUAGE
#include <kernel.h>
#include <zephyr/types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern K_THREAD_STACK_DEFINE(_interrupt_stack, CONFIG_ISR_STACK_SIZE);

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_ARCH_POWERPC_INCLUDE_KERNEL_ARCH_DATA_H_ */
