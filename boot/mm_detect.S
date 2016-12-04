include(`include/mm_detect.h')

.global mm_detect
.global mm_map

SEC_BSS(`.bootsys_bss')
mm_map:
.space E820_ENTRY_LEN * E820_MAX_ENTRIES

SEC_TEXT(`.bootsys_text')
/*
 * ax - error code
 * bx - pointer to the memory map
 * si - length of memory map
 * TODO: maybe return the length of the memory map in cx to preserve the
 * purpose of the counter register cx!
 */
mm_detect:
	mov %ds, %ax
	mov %ax, %es
	mov $mm_map, %di
	xor %ebx, %ebx
	mov $E820_SMAP, %edx
	xor %si, %si /* cx already in use, so this is our counter register */
1:
	cmp $E820_MAX_ENTRIES, %si /* if no. entries maxes out */
	je 3f

	mov $0xe820, %eax
	mov $E820_ENTRY_LEN, %ecx
	int $0x15 /* obtain entry */

	jc 4f /* some BIOSes terminate with cf = 0 */
	cmp $E820_SMAP, %eax
	jne 2f /* if SMAP value not set properly */

	test %ebx, %ebx
	jz 4f /* ebx = 0x0 -> terminate entry list */

	add $E820_ENTRY_LEN, %di
	inc %si
	jmp 1b
2:
	mov $E820_BAD_SMAP, %ax
	ret
3:
	mov $E820_TOO_MANY, %ax
	ret
4:
	xor %ax, %ax
5:
	mov $mm_map, %bx
	ret

/* TODO: warn if e820_entry_len * e820_max_entries > register size */
