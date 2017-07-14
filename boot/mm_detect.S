#include <mm_detect.h>
#include <addrs.h>

.code16

/*
 * This is one of the few (if not the only) header file, where I got major ideas
 * from the Linux source code. The code is entirely self-written, like EVERY 
 * other line of the project, but some BIOS inconsistencies can only be found
 * out through a lot of testing, for example. I do not have the resources for
 * that; Linux has.
 */

.global init_e820_map

.data
/* TODO: allocate on heap or similar! */
entry: .skip 0x14
entry_dest: .int KP_E820_MAP

.text
/*
 * Returns the number of entries in `%ax'.
 */
init_e820_map:
	xor %ebx, %ebx
	mov $E820_SMAP, %edx

	/*
	 * `%ebp' instead of `%bp' is used here to enable using 32-bit addressing.
	 * Since the boot system is always within the first 64 KiB of memory,
	 * `%ebp == %bp' is guaranteed.
	 */
	mov $E820_MAX_ENTRIES, %ebp

1:
	mov $0xe820, %eax
	mov $E820_ENTRY_SIZE, %ecx
	mov $entry, %di
	int $0x15

	/*
	 * Some BIOSes terminating with a set carry flag but fail to set "SMAP"
	 * properly, so we check for the flag first.
	 */
	jc success

	/*
	 * Some BIOSes stop setting "SMAP" during the loop for unknown reasons.
	 * We just fail in that case.
	 */
	cmp $E820_SMAP, %eax
	jne failure

	/* Terminate normally: `%ebx' is zero. */
	test %ebx, %ebx
	jz success

	movzx %di, %esi
	mov entry_dest, %edi
	mov $0x5, %ecx	/* assumes entry size to be 20 bytes */
	rep movsl
	mov %edi, entry_dest

	dec %ebp
	jnz 1b

success:
	mov $E820_MAX_ENTRIES, %ax
	sub %bp, %ax
	ret

failure:
	xor %ax, %ax
	ret
