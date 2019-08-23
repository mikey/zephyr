/*
 * Copyright (c) 2019 Anton Blanchard <anton@linux.ibm.com>
 * Copyright (c) 2019 Michael Neuling <mikey@neuling.org>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Thread context switching
 *
 * This module implements the routines necessary for thread context switching
 * on PowerPC CPUs.
 */

#include <kernel_structs.h>
#include <kernel_arch_data.h>
#include <offsets_short.h>
#include <offsets.h>
#include <toolchain.h>
#include <arch/cpu.h>

GTEXT(z_arch_switch)
GTEXT(z_arch_switch_return)

SECTION_FUNC(TEXT, z_arch_switch)

#ifdef CONFIG_EXECUTION_BENCHMARKING
#warning Need to implement
#endif
	/*
	 * Saving the entire state is overkill for a cooperative context
	 * switch, but we have a single restore path for cooperative and
	 * preemptive context switches (z_arch_switch_return) and it
	 * restores everything.
	 */
	stdu	%r1,-(__z_arch_esf_t_SIZEOF+USER_REDZONE_SIZE)(%r1)
	std	%r0,  __z_arch_esf_t_r0_OFFSET(%r1)
	std	%r1,  __z_arch_esf_t_r1_OFFSET(%r1)
	std	%r2,  __z_arch_esf_t_r2_OFFSET(%r1)
	std	%r3,  __z_arch_esf_t_r3_OFFSET(%r1)
	std	%r4,  __z_arch_esf_t_r4_OFFSET(%r1)
	std	%r5,  __z_arch_esf_t_r5_OFFSET(%r1)
	std	%r6,  __z_arch_esf_t_r6_OFFSET(%r1)
	std	%r7,  __z_arch_esf_t_r7_OFFSET(%r1)
	std	%r8,  __z_arch_esf_t_r8_OFFSET(%r1)
	std	%r9,  __z_arch_esf_t_r9_OFFSET(%r1)
	std	%r10, __z_arch_esf_t_r10_OFFSET(%r1)
	std	%r11, __z_arch_esf_t_r11_OFFSET(%r1)
	std	%r12, __z_arch_esf_t_r12_OFFSET(%r1)
	std	%r13, __z_arch_esf_t_r13_OFFSET(%r1)
	std	%r14, __z_arch_esf_t_r14_OFFSET(%r1)
	std	%r15, __z_arch_esf_t_r15_OFFSET(%r1)
	std	%r16, __z_arch_esf_t_r16_OFFSET(%r1)
	std	%r17, __z_arch_esf_t_r17_OFFSET(%r1)
	std	%r18, __z_arch_esf_t_r18_OFFSET(%r1)
	std	%r19, __z_arch_esf_t_r19_OFFSET(%r1)
	std	%r20, __z_arch_esf_t_r20_OFFSET(%r1)
	std	%r21, __z_arch_esf_t_r21_OFFSET(%r1)
	std	%r22, __z_arch_esf_t_r22_OFFSET(%r1)
	std	%r23, __z_arch_esf_t_r23_OFFSET(%r1)
	std	%r24, __z_arch_esf_t_r24_OFFSET(%r1)
	std	%r25, __z_arch_esf_t_r25_OFFSET(%r1)
	std	%r26, __z_arch_esf_t_r26_OFFSET(%r1)
	std	%r27, __z_arch_esf_t_r27_OFFSET(%r1)
	std	%r28, __z_arch_esf_t_r28_OFFSET(%r1)
	std	%r29, __z_arch_esf_t_r29_OFFSET(%r1)
	std	%r30, __z_arch_esf_t_r30_OFFSET(%r1)
	std	%r31, __z_arch_esf_t_r31_OFFSET(%r1)
	mflr	%r0
	std	%r0, __z_arch_esf_t_nia_OFFSET(%r1)
	std	%r0, __z_arch_esf_t_lr_OFFSET(%r1)
	mfctr	%r0
	std	%r0, __z_arch_esf_t_ctr_OFFSET(%r1)
	mfcr	%r0
	std	%r0, __z_arch_esf_t_cr_OFFSET(%r1)
	mfmsr	%r0
	std	%r0, __z_arch_esf_t_srr1_OFFSET(%r1)

	/* Save previous stack pointer to ->switch_handle */
	std	%r1,0(%r4)

	/* Next stack pointer */
	mr	%r1,%r3

	b	z_arch_switch_return
