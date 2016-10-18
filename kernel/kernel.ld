OUTPUT(kernel.out)
ENTRY(kernel_start)

SECTIONS {
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) }
}
