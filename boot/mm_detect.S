include(`include/mm_detect.h')

.global mm_detect

SEC_BSS(`.bootsys_bss')
mm_map:
.space E820_ENTRY_LEN * E820_MAX_ENTRIES

SEC_TEXT(`.bootsys_text')
/*
 * ax - error code
 * bx - pointer to the memory map
 */
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

	mov $0xe820, %eax
	mov $E820_ENTRY_LEN, %ecx
	int $0x15

	jc 3f
	test %ebx, %ebx
	jz 3f
	jmp 1b
2:
	mov $E820_TOO_MANY, %ax
	jmp 4f
3:
	xor %ax, %ax
4:
	mov $mm_map, %bx
	ret

/* TODO: warn if e820_entry_len * e820_max_entries > register size */

/* TODO: read arch/x86/boot/memory.c and do something equivalent to that.
 * some bioses stop setting edx to SMAP during the loop i think or something
 * similar... check that! */
