ENTRY(kernel_start)

MEMORY {
	kernel : org = KERNEL_ORG, len = KERNEL_SIZE
}

SECTIONS {
    .text : { *(.text) } > kernel
    .data : { *(.data) } > kernel
    .bss : { *(.bss) } > kernel
}
