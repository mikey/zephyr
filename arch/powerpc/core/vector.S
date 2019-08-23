/*
 * Copyright (c) 2019 Michael Neuling <mikey@neuling.org>
 * Copyright (c) 2019 Anton Blanchard <anton@linux.ibm.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <toolchain.h>
#include <linker/sections.h>
#include <kernel_structs.h>
#include <offsets_short.h>
#include <offsets.h>
#include <arch/cpu.h>


/* Load an immediate 64-bit value into a register */
#define LOAD_IMM64(r, e)			\
	lis	r,(e)@highest;			\
	ori	r,r,(e)@higher;			\
	rldicr	r,r, 32, 31;			\
	oris	r,r, (e)@h;			\
	ori	r,r, (e)@l;

/* exports */
GTEXT(__start)
GTEXT(z_arch_switch_return)

/* imports */
GTEXT(__initialize)
GDATA(_sw_isr_table)

#define FIXUP_ENDIAN						   \
	tdi   0,0,0x48;	  /* Reverse endian of b . + 8		*/ \
	b     191f;	  /* Skip trampoline if endian is good	*/ \
	.long 0xa600607d; /* mfmsr r11				*/ \
	.long 0x01006b69; /* xori r11,r11,1			*/ \
	.long 0x05009f42; /* bcl 20,31,$+4			*/ \
	.long 0xa602487d; /* mflr r10				*/ \
	.long 0x14004a39; /* addi r10,r10,20			*/ \
	.long 0xa64b5a7d; /* mthsrr0 r10			*/ \
	.long 0xa64b7b7d; /* mthsrr1 r11			*/ \
	.long 0x2402004c; /* hrfid				*/ \
191:

SECTION_FUNC(vectors, __start)
	/* Microwatt entry point */
	li	%r3, 0
	b	__initialize

	/* QEMU entry point */
	.= 0x10
	FIXUP_ENDIAN
	li	%r3, 1
	b	__initialize

#define EXCEPTION(nr)		\
	.= nr			;\
	b	.

	/* More exception stubs */
	EXCEPTION(0x300)
	EXCEPTION(0x380)
	EXCEPTION(0x400)
	EXCEPTION(0x480)
	EXCEPTION(0x500)
	EXCEPTION(0x600)
	EXCEPTION(0x700)
	EXCEPTION(0x800)

	.= 0x900
	b	__decrementer

	EXCEPTION(0x980)
	EXCEPTION(0xa00)
	EXCEPTION(0xb00)
	EXCEPTION(0xc00)
	EXCEPTION(0xd00)
	EXCEPTION(0xe00)
	EXCEPTION(0xe20)
	EXCEPTION(0xe40)
	EXCEPTION(0xe60)
	EXCEPTION(0xe80)
	EXCEPTION(0xf00)
	EXCEPTION(0xf20)
	EXCEPTION(0xf40)
	EXCEPTION(0xf60)
	EXCEPTION(0xf80)
	EXCEPTION(0x1000)
	EXCEPTION(0x1100)
	EXCEPTION(0x1200)
	EXCEPTION(0x1300)
	EXCEPTION(0x1400)
	EXCEPTION(0x1500)
	EXCEPTION(0x1600)

SECTION_FUNC(vectors, __decrementer)
	/* Create a stack frame */
	stdu	%r1,-(__z_arch_esf_t_SIZEOF+USER_REDZONE_SIZE)(%r1)
	/* do basic save */
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
	mfsrr0	%r3
	std	%r3, __z_arch_esf_t_nia_OFFSET(%r1)
	mflr	%r3
	std	%r3, __z_arch_esf_t_lr_OFFSET(%r1)
	mfctr	%r3
	std	%r3, __z_arch_esf_t_ctr_OFFSET(%r1)
	mfcr	%r3
	std	%r3, __z_arch_esf_t_cr_OFFSET(%r1)
	mfsrr1	%r3
	std	%r3, __z_arch_esf_t_srr1_OFFSET(%r1)
	/* all "userspace" registers are now saved */

	/* Create a stack frame for c code */
	stdu	%r1,-STACK_FRAME_C_MINIMAL(%r1)


	/* switch to interrupt stack */
	/* FIXME don't do this for now */
//	LOAD_IMM64(%r1, _interrupt_stack)
//	LOAD_IMM64(%r0, CONFIG_ISR_STACK_SIZE-0x100)
//	add	%r1,%r1,%r0
//	std	%r31, 0(%r1)
saver1:
	/* load kernel ptr */
	LOAD_IMM64(%r5, _kernel)
	/* Decrement _kernel.nested variable */
	lwz	%r8, _kernel_offset_to_nested(%r5)
	addi	%r8, %r8, 1
	stw	%r8, _kernel_offset_to_nested(%r5)


	/* check interrupt table */
	/* get the interrupt number */
	li	%r4, 0	/* FIXME!! This is the timebase so only 1 interrupt */
	LOAD_IMM64(%r5, _sw_isr_table)
	sldi	%r4, %r4, (RV_REGSHIFT + 1)
	add	%r6, %r5, %r4

	ld	%r3, 0(%r5) /* load parm1 */
	ld	%r6, RV_REGSIZE(%r5) /* load func ptr */
	mtctr	%r6
	bctrl	/* call ISR */
	nop

	addi	%r3,%r1,STACK_FRAME_C_MINIMAL
	bl	z_get_next_switch_handle

	/* Restore curent thread stack pointer */
	ld	%r1, 0(%r1)

	/* New stack pointer */
	mr	%r1,%r3

	/* Increment _kernel.nested variable */
	LOAD_IMM64(%r5, _kernel)
	lwz	%r8, _kernel_offset_to_nested(%r5)
	addi	%r8, %r8, -1
	stw	%r8, _kernel_offset_to_nested(%r5)

SECTION_FUNC(vectors, z_arch_switch_return)

	ld	%r0,  __z_arch_esf_t_r0_OFFSET(%r1)
//	ld	%r1,  __z_arch_esf_t_r1_OFFSET(%r1) // do this at rfid
	ld	%r2,  __z_arch_esf_t_r2_OFFSET(%r1)
//	ld	%r3,  __z_arch_esf_t_r3_OFFSET(%r1) // do this at rfid
	ld	%r4,  __z_arch_esf_t_r4_OFFSET(%r1)
	ld	%r5,  __z_arch_esf_t_r5_OFFSET(%r1)
	ld	%r6,  __z_arch_esf_t_r6_OFFSET(%r1)
	ld	%r7,  __z_arch_esf_t_r7_OFFSET(%r1)
	ld	%r8,  __z_arch_esf_t_r8_OFFSET(%r1)
	ld	%r9,  __z_arch_esf_t_r9_OFFSET(%r1)
	ld	%r10, __z_arch_esf_t_r10_OFFSET(%r1)
	ld	%r11, __z_arch_esf_t_r11_OFFSET(%r1)
	ld	%r12, __z_arch_esf_t_r12_OFFSET(%r1)
	ld	%r13, __z_arch_esf_t_r13_OFFSET(%r1)
	ld	%r14, __z_arch_esf_t_r14_OFFSET(%r1)
	ld	%r15, __z_arch_esf_t_r15_OFFSET(%r1)
	ld	%r16, __z_arch_esf_t_r16_OFFSET(%r1)
	ld	%r17, __z_arch_esf_t_r17_OFFSET(%r1)
	ld	%r18, __z_arch_esf_t_r18_OFFSET(%r1)
	ld	%r19, __z_arch_esf_t_r19_OFFSET(%r1)
	ld	%r20, __z_arch_esf_t_r20_OFFSET(%r1)
	ld	%r21, __z_arch_esf_t_r21_OFFSET(%r1)
	ld	%r22, __z_arch_esf_t_r22_OFFSET(%r1)
	ld	%r23, __z_arch_esf_t_r23_OFFSET(%r1)
	ld	%r24, __z_arch_esf_t_r24_OFFSET(%r1)
	ld	%r25, __z_arch_esf_t_r25_OFFSET(%r1)
	ld	%r26, __z_arch_esf_t_r26_OFFSET(%r1)
	ld	%r27, __z_arch_esf_t_r27_OFFSET(%r1)
	ld	%r28, __z_arch_esf_t_r28_OFFSET(%r1)
	ld	%r29, __z_arch_esf_t_r29_OFFSET(%r1)
	ld	%r30, __z_arch_esf_t_r30_OFFSET(%r1)
	ld	%r31, __z_arch_esf_t_r31_OFFSET(%r1)

	ld	%r3, __z_arch_esf_t_lr_OFFSET(%r1)
	mtlr	%r3
	ld	%r3, __z_arch_esf_t_ctr_OFFSET(%r1)
	mtctr	%r3
	ld	%r3, __z_arch_esf_t_cr_OFFSET(%r1)
	mtcr	%r3
	ld	%r3, __z_arch_esf_t_srr1_OFFSET(%r1)
	mtsrr1	%r3
	ld	%r3, __z_arch_esf_t_nia_OFFSET(%r1)
	mtsrr0	%r3

	/* restore %r3 */
	ld	%r3,  __z_arch_esf_t_r3_OFFSET(%r1)

	/* do final fixup r1 */
	ld	%r1, 0(%r1)
decrementer_exit:
	rfid

// z_irq_spurious is getting optimised away between the first and second link
// stage, resulting in the location of the isr functions moving. Refer to it
// here so it doesn't get removed by the linker.
.llong z_irq_spurious
