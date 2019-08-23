/*
 * Copyright (c) 2019 Michael Neuling <mikey@neuling.org>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <kernel.h>
#include <arch/cpu.h>
#include <device.h>
#include <irq.h>
#include <drivers/timer/system_timer.h>

static u32_t accumulated_cycle_count;

/* FIXME: we only have 1 interrupt for now */
#define TIMER_IRQ_NUM 0

static void write_decrementer(u64_t val)
{
	__asm__ __volatile__("mtdec %0" : : "r" (val));
}

static void powerpc_timebase_irq_handler(void *device)
{
	ARG_UNUSED(device);
	int key = irq_lock();

	write_decrementer(sys_clock_hw_cycles_per_tick());

	/* FIXME: This is going to overflow pretty quickly??!? */
	accumulated_cycle_count += sys_clock_hw_cycles_per_tick();

	z_clock_announce(1); /* this does the scheduling */

	irq_unlock(key);
}

u32_t z_clock_elapsed(void)
{
	return 0;
}

u32_t z_timer_cycle_get_32(void)
{
	/* FIXME this is unused currently */
	return accumulated_cycle_count;
}

int z_clock_driver_init(struct device *device)
{
	ARG_UNUSED(device);

	IRQ_CONNECT(TIMER_IRQ_NUM, 0, powerpc_timebase_irq_handler, NULL, 0);
	irq_enable(TIMER_IRQ_NUM);

	write_decrementer(sys_clock_hw_cycles_per_tick());

	return 0;
}
