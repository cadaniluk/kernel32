include(`boot.h')

OUTPUT(boot.out)
ENTRY(boot_start)

MEMORY {
	boot : org = BOOT_TEXT, len = BOOT_SIZE
	bootsys : org = BOOTSYS_TEXT % 0x10, len = BOOTSYS_LEN
}


SECTIONS {
	.text : { *(.text) } > boot
	.data : { *(.data) } > boot
	.boot_sig : AT(BOOT_TEXT + BOOT_SIZE - 0x2) { SHORT(0xaa55); } > boot

	.bootsys_text : AT(BOOT_TEXT + BOOT_SIZE) { *(.bootsys_text) } > bootsys
	.bootsys_data : AT(LOADADDR(.bootsys_text) + SIZEOF(.bootsys_text)) {
	*(.bootsys_data) } > bootsys
}

/* TODO: is the at command for bootsys_data really necessary? */
