/*
 * Copyright (c) 2019 Anton Blanchard <anton@linux.ibm.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <kernel.h>
#include <ksched.h>
#include <kernel_structs.h>
#include <offsets.h>

#define STACK_POISON 0xBADC0FFEE0DDF00D

void z_arch_new_thread(struct k_thread *thread, k_thread_stack_t *stack,
		 size_t stack_size, k_thread_entry_t thread_func,
		 void *arg1, void *arg2, void *arg3,
		 int priority, unsigned int options)
{
	char *stack_memory = Z_THREAD_STACK_BUFFER(stack);
	Z_ASSERT_VALID_PRIO(priority, thread_func);

	unsigned long stack_init;
	struct __esf *s;

	z_new_thread_init(thread, stack_memory, stack_size, priority, options);

	/*
	 * We need a minimal stack frame for the called function to write its
	 * LR into.
	 */
	stack_init = STACK_ROUND_DOWN(stack_memory + stack_size -
				      STACK_FRAME_C_MINIMAL);

	/*
	 * Poison the back chain pointer and the LR so it's obvious if
	 * z_thread_entry() returns.
	 */
	*(unsigned long *)stack_init = STACK_POISON;

	/* Now create the stack frame that z_arch_switch will use. */
	stack_init -= sizeof(struct __esf) + USER_REDZONE_SIZE;

	s = (struct __esf *)stack_init;
	memset(s, 0, sizeof(struct __esf));
	s->backptr = stack_init + sizeof(struct __esf) + USER_REDZONE_SIZE;
	s->nia = (ulong_t)z_thread_entry;
	s->srr1 = (ulong_t)0x9000000000008003UL;
	s->r3 = (ulong_t)thread_func;
	s->r4 = (ulong_t)arg1;
	s->r5 = (ulong_t)arg2;
	s->r6 = (ulong_t)arg3;

	/* Store the stack pointer away for z_arch_switch. */
	thread->switch_handle = (void *)stack_init;

	return;
}
