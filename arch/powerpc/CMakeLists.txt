# SPDX-License-Identifier: Apache-2.0

add_subdirectory(core)

set_property(GLOBAL PROPERTY PROPERTY_OUTPUT_FORMAT elf64-powerpcle)

zephyr_cc_option(-msoft-float)
zephyr_cc_option(-mno-string)
zephyr_cc_option(-mno-multiple)
zephyr_cc_option(-mno-vsx)
zephyr_cc_option(-mno-altivec)
zephyr_cc_option(-mlittle-endian)
zephyr_cc_option(-mstrict-align)
zephyr_cc_option(-fno-stack-protector)
