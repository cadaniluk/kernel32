OUTPUT(boot.out)
ENTRY(boot_start)

/*
 * TODO: find a way to have common variables for linker scripts and C & asm.
 * the best way would be to just use m4. figure out how to do that!
 */

bootsys_size = 0x10000;
limit = 0x90000;

MEMORY {
	boot : org = 0x7c00, len = 0x200
	bootsys : org = limit - bootsys_size, len = limit
}


SECTIONS {
	.text : { *(.text) } > boot
	.data : { *(.data) } > boot
	.end : AT(0x7dfe) { SHORT(0xaa55); } > boot

	.bootsys_text : { *(.bootsys_text) } > bootsys
	.bootsys_data : { *(.bootsys_data) } > bootsys
}
