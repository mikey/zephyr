/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <toolchain.h>
#include <kernel_structs.h>

/* exports */
GTEXT(__initialize)
GTEXT(__reset)

/* imports */
GTEXT(_PrepC)

/* Load an immediate 64-bit value into a register */
#define LOAD_IMM64(r, e)			\
	lis	r,(e)@highest;			\
	ori	r,r,(e)@higher;			\
	rldicr	r,r, 32, 31;			\
	oris	r,r, (e)@h;			\
	ori	r,r, (e)@l;

#if CONFIG_INCLUDE_RESET_VECTOR
SECTION_FUNC(reset, __reset)
	b __initialize
#endif /* CONFIG_INCLUDE_RESET_VECTOR */

/*
 * Remainder of asm-land initialization code before we can jump into
 * the C domain
 */
SECTION_FUNC(TEXT, __initialize)
#ifdef CONFIG_INIT_STACKS
	/* Pre-populate all bytes in _interrupt_stack with 0xAA */
	LOAD_IMM64(%r1, _interrupt_stack)
	LOAD_IMM64(%r0, CONFIG_ISR_STACK_SIZE/8)
	LOAD_IMM64(%r2, 0xaaaaaaaaaaaaaaaa)

	mtctr	%r0

1:	std	%r2,0(%r1)
	addi	%r1,%r1,8
	bdnz	1b
#endif
	/* set HID0 HILE so we take IRQs in LE */
	mfspr	%r1, 0x3f0
	LOAD_IMM64(%r2, 0x0800000000000000)
	or	%r1, %r2, %r1
	mtspr	0x3f0, %r1

	LOAD_IMM64(%r1, first_rfid)
	mtsrr0	%r1
	LOAD_IMM64(%r1, 0x9000000000000003)
	mtsrr1	%r1
	rfid

first_rfid:
	LOAD_IMM64(%r1, _interrupt_stack) // ?? FIXME which stack ??
	LOAD_IMM64(%r0, CONFIG_ISR_STACK_SIZE-0x100)
	add	%r1,%r1,%r0

	LOAD_IMM64(%r12, _PrepC)
	mtctr	%r12,
	bctrl
	b .
