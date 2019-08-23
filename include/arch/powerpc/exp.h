/*
 * Copyright (c) 2019 Michael Neuling <mikey@neuling.org>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief PowerPC public exception handling
 *
 * PowerPC-specific kernel exception handling interface.
 */

#ifndef ZEPHYR_INCLUDE_ARCH_POWERPC_EXP_H_
#define ZEPHYR_INCLUDE_ARCH_POWERPC_EXP_H_

#ifndef _ASMLANGUAGE
#include <zephyr/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct __esf {
	ulong_t backptr;
	ulong_t nia;
	ulong_t srr1;
	ulong_t lr;
	ulong_t ctr;
	ulong_t cr;

	ulong_t r0;
	ulong_t r1;
	ulong_t r2;
	ulong_t r3;
	ulong_t r4;
	ulong_t r5;
	ulong_t r6;
	ulong_t r7;
	ulong_t r8;
	ulong_t r9;
	ulong_t r10;
	ulong_t r11;
	ulong_t r12;
	ulong_t r13;
	ulong_t r14;
	ulong_t r15;
	ulong_t r16;
	ulong_t r17;
	ulong_t r18;
	ulong_t r19;
	ulong_t r20;
	ulong_t r21;
	ulong_t r22;
	ulong_t r23;
	ulong_t r24;
	ulong_t r25;
	ulong_t r26;
	ulong_t r27;
	ulong_t r28;
	ulong_t r29;
	ulong_t r30;
	ulong_t r31;
};

typedef struct __esf z_arch_esf_t;

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_ARCH_POWERPC_EXP_H_ */
