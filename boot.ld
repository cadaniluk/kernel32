OUTPUT(boot.out)
ENTRY(boot_start)

SECTIONS {
	. = 0x500;
	.text_util : { *(.text_util) }
	.data_util : { *(.data_util) }
	. = 0x7c00;
	.text : { *(.text) }
	.data : { *(.data) }
	.end : AT(0x7dfe) { SHORT(0xaa55); }
}
