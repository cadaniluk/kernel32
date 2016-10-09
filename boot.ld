OUTPUT(boot.out)
ENTRY(boot_start)

SECTIONS {
	. = 0x7c00;
	.text : { *(.text) }
	.data : { *(.data) }
	.end : AT(0x7dfe) { SHORT(0xaa55); }
}
