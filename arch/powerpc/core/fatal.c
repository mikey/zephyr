/*
 * Copyright (c) 2016 Jean-Paul Etienne <fractalclone@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <kernel.h>
#include <kernel_structs.h>
#include <inttypes.h>
#include <logging/log.h>

LOG_MODULE_DECLARE(os);

FUNC_NORETURN void z_powerpc_fatal_error(unsigned int reason,
				       const z_arch_esf_t *esf)
{
	LOG_ERR("z_powerpc_fatal_error");
	while (1) ;
}
