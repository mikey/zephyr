# SPDX-License-Identifier: Apache-2.0

set(EMU_PLATFORM qemu)

set(QEMU_binary_suffix powerpc64le)
set(QEMU_CPU_TYPE_${ARCH} powerpc64le)

set(QEMU_FLAGS_${ARCH}
  -nographic
  -machine powerpc64le
  )

board_set_debugger_ifnset(qemu)
