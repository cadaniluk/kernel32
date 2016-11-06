include(`include/mm_detect.h')

.global mm_detect

SEC_BSS(`.bootsys_bss')
mm_map:
.space E820_ENTRY_LEN * E820_MAX_ENTRIES

SEC_TEXT(`.bootsys_text')
/* returns a pointer to the memory map */
mm_detect:
	mov $BOOTSYS_SEG, %ax
	mov %ax, %es
	mov $mm_map, %di
	xor %ebx, %ebx
	mov $E820_SMAP, %edx
	xor %si, %si /* cx used, so this is our counter register */
1:
	cmp $E820_MAX_ENTRIES, %si
	je 2f
	/* TODO: add some indicator in the return value that the max entry
	 * number has been reached. */

	mov $0xe820, %eax
	mov $E820_ENTRY_LEN, %ecx
	int $0x15

	jc 2f
	test %ebx, %ebx
	jz 2f
	jmp 1b
2:
	mov $mm_map, %ax
	ret

/* TODO: warn if e820_entry_len * e820_max_entries > register size */

