ENTRY(boot_start)

MEMORY {
	boot : org = BOOT_ORG, len = BOOT_LEN
	bootsys : org = 0x0, len = BOOTSYS_LEN /* org has only the offset */
}


SECTIONS {
	.text : { *(.text) } > boot
	.data : { *(.data) } > boot
	.boot_sig : AT(BOOT_ORG + BOOT_LEN - 0x2) { SHORT(0xaa55); } > boot

	.bootsys_text : AT(BOOT_ORG + BOOT_LEN) { *(.bootsys_text) } > bootsys
	.bootsys_data : AT(LOADADDR(.bootsys_text) + SIZEOF(.bootsys_text)) {
	*(.bootsys_data) } > bootsys
	.bootsys_bss : AT(LOADADDR(.bootsys_data) + SIZEOF(.bootsys_data)) {
	*(.bootsys_bss) } > bootsys
}

/* TODO: is the at command for bootsys_data and _bss really necessary? */

