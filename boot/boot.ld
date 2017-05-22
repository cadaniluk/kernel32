#include <addrs.h>

ENTRY(boot_start)

MEMORY {
	boot : org = BOOT_ORG, len = BOOT_LEN
	bootsys : org = BOOTSYS_ORG, len = BOOTSYS_LEN
}


SECTIONS {
	.text : { boot.o(.text) } > boot
	.data : { boot.o(.data) } > boot
	.boot_sig : AT(BOOT_ORG + BOOT_LEN - 0x2) { SHORT(0xaa55); } > boot

	.bootsys_text : AT(BOOT_ORG + BOOT_LEN) { *(.text.bootsys_start) *(.text) }
	> bootsys

	.bootsys_data : { *(.data) } > bootsys
}
