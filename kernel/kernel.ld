#include <addrs.h>

ENTRY(kernel_start)

MEMORY {
	kernel : org = KERNEL_ORG, len = KERNEL_LEN
}

SECTIONS {
    .text : { *(.text.kernel_start) *(.text) } > kernel
    .data : { *(.data) } > kernel
}
