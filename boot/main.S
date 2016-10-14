define(`BOOTSYS_SEG', `0x7e0')
define(`BOOTSYS_OFF', `0x0')
define(`BOOTSYS_SIZE', `0x1') /* in 512-byte sectors */
define(`BOOTSYS_C', `0x0')
define(`BOOTSYS_H', `0x0')
define(`BOOTSYS_S', `0x2')
define(`FLOPPY_DRV', `0x0')

/* TODO: optimize the whole code! not by using subtle and hard-to-
 * detect micro-optimizations but using calls to functions,
 * for example! the apm functions, for example, can all be wrapped into
 * call functions and thus we can save valuable bootloader space. */

define(`VID_SEG', `0xb800')
define(`NUM_COLS', `80')
define(`VID_ATTR', `0xf')

/*
 * The bootloader is loaded to physical address `0x7c00'. It does some stuff
 * and loads the 16-bit boot system to `0x7e00'. Assuming the memory until
 * `0x90000' is free (the EBDA starts somewhere around `0x9fc00', but it is
 * not standardized, AFAIK), the boot system has over 512 KiB space. That
 * must suffice. If it does not, we can still move the bootloader to `0x500'
 * right after IVT and BDA to give us an additional `0x7700' bytes. If that
 * does not suffice, we can activate Unreal Mode for theoretical 4 GiB if
 * the machine provides those. And if that does not suffice... well,
 * then I'm just too dumb to write 16-bit code smaller than 4 GiB.
 */

.code16

.global boot_start

.data
verify_cpu: .asciz "Verifying CPU..."
cpu32: .asciz "32-bit-compatible... ready to go!"
not_cpu32: .asciz "Not 32-bit-compatible... kernel cannot start up."

curr_line: .byte 0x0

.text
boot_start:
	/* Set up a basic stack. */
	mov $0x50, %ax /* Stack segment points to right after the BDA */
	mov %ax, %ss
	mov $0x76fe, %sp /* word-aligned, so must be even */

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
	/* First, make sure we are actually running a 32-bit CPU. We do this
	 * by verifying that the MSB of the `flags' register is always clear
	 * and bits 12 through 14 are arbitrarily modifiable. */
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

	
before:
	/* Now, load the Real Mode part of the kernel, effectively, a
	 * second-stage bootloader. Retry three times, as recommended by
	 * RBIL. */
	xor %bp, %bp
1:
/* TODO: define BOOTSYS constants sopmewhere and FLOPPY_DRV! */
	mov $BOOTSYS_SEG, %ax
	mov %ax, %es
	mov $BOOTSYS_OFF, %bx
/* TODO: add some checking so that not too much sectors are loaded even though
 * there isn't enough space! also check (at preprocessing time) that the sector
 * number is not impossibly high! */
	mov $0x200 | BOOTSYS_SIZE, %ax
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

lbl:
/* TODO: verify at preprocessing time that bootsys_seg:bootsys_off is aligned
 * on word oundaries! */
ljmp $BOOTSYS_SEG, $BOOTSYS_OFF

/* TODO: change that to something more sophisticated later! */
no_32b:
	mov $not_cpu32, %si
	call puts

	cli
	hlt

floppy_err:
	cli
	hlt

/* `ds:si' - string to print */
/* Currently only goes to the next line... add video driver support and
 * use the video driver then! */
puts:
	push %ax
	push %es

	mov $NUM_COLS, %al
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
