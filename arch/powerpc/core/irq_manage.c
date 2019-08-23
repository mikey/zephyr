/*
 * Copyright (c) 2016 Jean-Paul Etienne <fractalclone@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <toolchain.h>
#include <kernel_structs.h>

void z_arch_irq_enable(unsigned int irq)
{
	unsigned int key = irq_lock();

	/* FIXME: enable/disable these when we have an IRQ controller */

	irq_unlock(key);
}

void z_arch_irq_disable(unsigned int irq)
{
	unsigned int key = irq_lock();

	/* FIXME: enable/disable these when we have an IRQ controller */

	irq_unlock(key);
}


void z_irq_priority_set(unsigned int irq, unsigned int prio, u32_t flags)
{
	ARG_UNUSED(flags);


	unsigned int key = irq_lock();

	/* FIXME: actually do something here */

	irq_unlock(key);
}

/*
 * @brief Spurious interrupt handler
 *
 * Installed in all dynamic interrupt slots at boot time. Throws an error if
 * called.
 *
 * @return N/A
 */

void z_irq_spurious(void *unused)
{
	ARG_UNUSED(unused);
	z_fatal_error(K_ERR_SPURIOUS_IRQ, NULL);
}
