include(`include/cmos.h')

define(`BOOTSYS_C', `0x0')
define(`BOOTSYS_H', `0x0')
define(`BOOTSYS_S', `0x2')
define(`FLOPPY_DRV', `0x0')
/* TODO: find correlation between bootsys_sectors and bootsys_size!
 * maybe just change bootsys_size to something smaller still < 64 kib, so
 * we can load as much sectors */
define(`BOOTSYS_SECTORS', `0x3')

/* TODO: i dont like how boot_start is subtracted by 2 here. its intended to
 * serve word-alignment, but thats way too hardcoded. find something more
 * flexible to achieve the same thing. */
define(`BOOT_SS', `0x50') /* right after BDA */
define(`BOOT_SP', `(BOOT_ORG - 0x2 - BOOT_SS * 0x10)')

/* TODO: optimize the whole code! not by using subtle and hard-to-
 * detect micro-optimizations but using calls to functions,
 * for example! the apm functions, for example, can all be wrapped into
 * call functions and thus we can save valuable bootloader space. */

define(`VID_SEG', `0xb800')
define(`NUM_COLS', `80')
define(`VID_ATTR', `0xf')

.global boot_start

.data
verify_cpu: .asciz "Verifying CPU..."
cpu32: .asciz "32-bit-compatible... ready to go!"
not_cpu32: .asciz "Not 32-bit-compatible... kernel cannot start up."
press_key: .asciz "Press a key to reboot..."

curr_line: .byte 0x0

.text
boot_start:
	/* Set up a basic stack. */
	mov $BOOT_SS, %ax /* Stack segment points to right after the BDA */
	mov %ax, %ss
	mov $BOOT_SP, %sp /* word-aligned, so must be even */

	/* NOTE: this part must be 8086-compatible, so the checking process
	 * below can be performed on any x86 CPU! */

	/* Enable video mode `0x3'. */
	mov $0x3, %ax
	int $0x10

	xor %ax, %ax
	mov %ax, %ds
	mov $verify_cpu, %si
	call puts

	/* TODO: check for intel 64 as well and take appropriate action! */
	/*
	 * First, make sure we are actually running a 32-bit CPU. We do this
	 * by verifying that the MSB of the `flags' register is always clear
	 * and bits 12 through 14 are arbitrarily modifiable.
	 */
	pushf
	pop %ax
	xor $0x7000, %ax
	mov %ax, %dx
	and $0x7fff, %dx
	or $0x8000, %ax
	push %ax
	popf
	pushf
	pop %ax
	cmp %ax, %dx
	jne no_32b

	mov $cpu32, %si
	call puts

	/*
	 * Determine storage medium and checking validity of sector number
	 * according to those values.
	 */

	/* TODO: access cmos here! */

	
	/* Now, load the Real Mode part of the kernel, effectively, a
	 * second-stage bootloader. Retry three times because the read
	 * command does not have a time-out. */
	xor %bp, %bp
1:
	mov $BOOTSYS_SEG, %ax
	mov %ax, %es
	xor %bx, %bx
/* TODO: add rounding up here for bootsys_size! */
	mov $0x200 | BOOTSYS_SECTORS, %ax
	mov $(BOOTSYS_C << 0x8) | BOOTSYS_S, %cx
	mov $(BOOTSYS_H << 0x8) | FLOPPY_DRV, %dx 
	int $0x13
	jnc 1f
	cmp $0x2, %bp
	je floppy_err
	inc %bp
	xor %ah, %ah
	int $0x13
	jmp 1b
1:

/* TODO: verify at preprocessing time that bootsys_seg:bootsys_off is aligned
 * on word oundaries! */
ljmp $BOOTSYS_SEG, $0x0

/* TODO: change that to something more sophisticated later! */
no_32b:
	mov $not_cpu32, %si
	call puts

	jmp reboot

floppy_err:
	jmp reboot

/* `ds:si' - string to print */
/* Currently only goes to the next line... add video driver support and
 * use the video driver then! */
puts:
	push %ax
	push %es

	mov $NUM_COLS * 0x2, %al /* `* 0x2' for char + attr */
	mulb curr_line
	mov %ax, %di
	incb curr_line

	mov $VID_SEG, %ax
	mov %ax, %es
	mov $VID_ATTR, %ah
1:
	lodsb
	test %al, %al
	jz 2f
	stosw
	jmp 1b
2:
	pop %es
	pop %ax

	ret

reboot:
	mov $press_key, %si
	call puts

	/* TODO: add key reading function here! */

	ljmp $0xf000, $0xfff0
