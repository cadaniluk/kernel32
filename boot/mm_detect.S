/*
 * This is one of the few (if not the only) header file, where I got major ideas
 * from the Linux source code. The code is entirely self-written, like EVERY 
 * other line of the project, but some BIOS inconsistencies can only be found
 * out through a lot of testing, for example. I do not have the resources for
 * that; Linux has.
 */

include(`mm_detect.h')

.global mm_detect_e820

SEC_RW_DATA(`.bootsys_data')

SEC_TEXT(`.bootsys_text')

/*
 * Input:
 * `es:di' - buffer for memory map
 *
 * Output:
 * `si' - number of entries in memory map
 *
 * TODO:
 * At least one known BIOS expects the buffer to be always the same, so we
 * load each entry into the buffer and copy it to the map.
 */
mm_detect_e820:
	push %eax
	push %ecx
	push %edx
	push %ebx
	push %si
	push %di

	xor %ebx, %ebx
	mov $E820_SMAP, %edx
	xor %si, %si
1:
	/* We can only have so many entries. */
	cmp $E820_MAX_ENTRIES, %si
	je 3f

	mov $0xe820, %eax
	mov $E820_ENTRY_LEN, %ecx
	int $0x15

	/*
	 * Some BIOSes terminating with a set carry flag fail to set "SMAP"
	 * properly, so we check for the flag first.
	 */
	jc 3f

	 /*
	  * Some BIOSes stop setting "SMAP" during the loop for unknown reason.
	  * We just fail in that case.
	  */
	cmp $E820_SMAP, %eax
	jne 2f

	test %ebx, %ebx /* if BIOS returns with %ebx = 0x0 */
	jz 3f

	add $E820_ENTRY_LEN, %di /* point to next entry */

	inc %si
	jmp 1b

2:
	xor %si, %si
3:
	pop %di
	pop %si
	pop %ebx
	pop %edx
	pop %ecx
	pop %eax
	ret
