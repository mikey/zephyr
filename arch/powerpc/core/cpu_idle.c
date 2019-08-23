/*
 * Copyright (c) 2019 Michael Neuling <mikey@neuling.org>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <irq.h>

/**
 *
 * @brief Power save idle routine
 *
 * This function will be called by the kernel idle loop or possibly within
 * an implementation of _sys_power_save_idle in the kernel when the
 * '_sys_power_save_flag' variable is non-zero.
 *
 * @return N/A
 */
void __weak z_arch_cpu_idle(void)
{
	int dec;

	z_arch_irq_unlock(1);

	/* Work around PowerNV QEMU decrementer bug */
	__asm__ __volatile__("mfdec %0" : "=r" (dec));
	if (dec < 0)
		__asm__ __volatile__("mtdec %0" : : "r" (1));
}

/**
 *
 * @brief Atomically re-enable interrupts and enter low power mode
 *
 * INTERNAL
 * The requirements for k_cpu_atomic_idle() are as follows:
 * 1) The enablement of interrupts and entering a low-power mode needs to be
 *    atomic, i.e. there should be no period of time where interrupts are
 *    enabled before the processor enters a low-power mode.  See the comments
 *    in k_lifo_get(), for example, of the race condition that
 *    occurs if this requirement is not met.
 *
 * 2) After waking up from the low-power mode, the interrupt lockout state
 *    must be restored as indicated in the 'imask' input parameter.
 *
 * @return N/A
 */
void __weak z_arch_cpu_atomic_idle(unsigned int key)
{
	z_arch_irq_unlock(key);
}
