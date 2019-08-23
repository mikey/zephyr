/*
 * Copyright (c) 2016 Jean-Paul Etienne <fractalclone@gmail.com>
 * Contributors: 2018 Antmicro <www.antmicro.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief POWERPC specific kernel interface header
 * This header contains the POWERPC specific kernel interface.  It is
 * included by the generic kernel interface header (arch/cpu.h)
 */

#ifndef ZEPHYR_INCLUDE_ARCH_POWERPC_ARCH_H_
#define ZEPHYR_INCLUDE_ARCH_POWERPC_ARCH_H_

#include "exp.h"
#include <arch/common/sys_io.h>
#include <arch/common/ffs.h>

#include <irq.h>
#include <sw_isr_table.h>
#include <generated_dts_board.h>

/* stacks, for POWERPC architecture stack should be 16byte-aligned */
#define STACK_ALIGN  16

#ifdef CONFIG_64BIT
#define RV_OP_LOADREG ld
#define RV_OP_STOREREG std
#define RV_REGSIZE 8
#define RV_REGSHIFT 3
#else
#define RV_OP_LOADREG lw
#define RV_OP_STOREREG sw
#define RV_REGSIZE 4
#define RV_REGSHIFT 2
#endif

/*
 * The 64bit PowerPC ABIs allows up to 512 bytes to be written below
 * the stack pointer. Whenever we take an exception and reuse the
 * existing stack, we need to be careful not to touch this space.
 */
#define USER_REDZONE_SIZE    512

/* A minimal ppc64le stack frame */
#define STACK_FRAME_C_MINIMAL   64

#ifndef _ASMLANGUAGE
#include <sys/util.h>

#ifdef __cplusplus
extern "C" {
#endif

#define STACK_ROUND_UP(x) ROUND_UP(x, STACK_ALIGN)
#define STACK_ROUND_DOWN(x) ROUND_DOWN(x, STACK_ALIGN)

/* macros convert value of its argument to a string */
#define DO_TOSTR(s) #s
#define TOSTR(s) DO_TOSTR(s)

/* concatenate the values of the arguments into one */
#define DO_CONCAT(x, y) x ## y
#define CONCAT(x, y) DO_CONCAT(x, y)

/*
 * SOC-specific function to get the IRQ number generating the interrupt.
 * __soc_get_irq returns a bitfield of pending IRQs.
 */
extern u32_t __soc_get_irq(void);

void z_arch_irq_enable(unsigned int irq);
void z_arch_irq_disable(unsigned int irq);
int z_arch_irq_is_enabled(unsigned int irq);
void z_irq_spurious(void *unused);

extern uint64_t arch_lock;
extern unsigned int arch_lock_key;

static inline uint64_t __cmpxchg64(uint64_t *mem, uint64_t old, uint64_t new)
{
	uint64_t prev;

	__asm__ volatile(
		"# __cmpxchg64		\n"
		"1:	ldarx	%0,0,%2	\n"
		"	cmpd	%0,%3	\n"
		"	bne-	2f	\n"
		"	stdcx.	%4,0,%2	\n"
		"	bne-	1b	\n"
		"2:			\n"

		: "=&r"(prev), "+m"(*mem)
		: "r"(mem), "r"(old), "r"(new)
		: "cr0");

	return prev;
}

/* FIXME need some memory barriers for SMP */
static inline void spin_lock(uint64_t *lock)
{
	while (1) {
		while (*lock) {}

		if (__cmpxchg64(lock, 0, 1) == 0)
			return;
	}
}

static inline void spin_unlock(uint64_t *lock)
{
	*lock = 0;
}

static ALWAYS_INLINE unsigned int z_arch_irq_lock(void)
{
	unsigned long msr;
	unsigned int key = 0;

        __asm__ __volatile__("mfmsr %0" : "=r" (msr));
	if (msr & 0x0000000000008000)
		key = 1; /* IRQs are on */

	/* Turn IRQs off */
	msr = 0x9000000000000003;
	__asm__ __volatile__("mtmsrd %0,1" : : "r" (msr));
	return key;
}

static ALWAYS_INLINE void z_arch_irq_unlock(unsigned int key)
{
	unsigned long msr = 0x9000000000008003;

	if (key == 1)
		/* Turn IRQs on */
		__asm__ __volatile__("mtmsrd %0,1" : : "r" (msr)); // IRQs on
}

static ALWAYS_INLINE bool z_arch_irq_unlocked(unsigned int key)
{
	return key;
}

static ALWAYS_INLINE void z_arch_nop(void)
{
	__asm__ volatile("nop");
}

extern u32_t z_timer_cycle_get_32(void);
#define z_arch_k_cycle_get_32()	z_timer_cycle_get_32()

#define Z_ARCH_IRQ_CONNECT(irq_p, priority_p, isr_p, isr_param_p, flags_p) \
({ \
	Z_ISR_DECLARE(irq_p, 0, isr_p, isr_param_p); \
	irq_p; \
})

#ifdef __cplusplus
}
#endif

#endif /*_ASMLANGUAGE */

#endif
