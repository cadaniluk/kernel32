OUTPUT(boot.out)
ENTRY(boot_start)

SECTIONS {
	. = 0x500;
	.bootsys_text : { *(.bootsys_text) }
	.bootsys_data : { *(.bootsys_data) }
	. = 0x7c00;
	.text : { *(.text) }
	.data : { *(.data) }
	.end : AT(0x7dfe) { SHORT(0xaa55); }
}
