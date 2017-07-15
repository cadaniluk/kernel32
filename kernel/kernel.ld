#include <addrs.h>

ENTRY(kernel_start)

/*
 * TODO: while this works, it cannot be a long-term solution: the bss section
 * can only start after the kernel indicated by KERNEL_LEN, NO MATTER
 * IF THAT MEMORY IS OCCUPIED AT ALL, however. you should find something more
 * flexible that changes dynamically, without fiddling around with macros and
 * constants!
 */
MEMORY {
	kernel : org = KERNEL_ORG, len = KERNEL_LEN
    bss : org = KERNEL_ORG + KERNEL_LEN, len = 0x80000 - (KERNEL_ORG + \
    KERNEL_LEN)
}

SECTIONS {
    .text : { *(.text.kernel_start) *(.text) } > kernel
    .data : { *(.data) } > kernel
    .bss : { *(.bss) } > bss
}
