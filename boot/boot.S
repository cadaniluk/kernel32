include(`boot.h')

define(`BOOTSYS_SIZE', `0x1') /* in 512-byte sectors */
define(`BOOTSYS_C', `0x0')
define(`BOOTSYS_H', `0x0')
define(`BOOTSYS_S', `0x2')
define(`FLOPPY_DRV', `0x0')

define(`BOOT_SS', `0x50') /* right after BDA */
define(`BOOT_SP', `0x76fe') /* word-aligned, right before bootloader */

/* TODO: optimize the whole code! not by using subtle and hard-to-
 * detect micro-optimizations but using calls to functions,
 * for example! the apm functions, for example, can all be wrapped into
 * call functions and thus we can save valuable bootloader space. */

define(`VID_SEG', `0xb800')
define(`NUM_COLS', `80')
define(`VID_ATTR', `0xf')

/*
 * The bootloader is loaded to physical address `0x7c00'. It does some stuff
 * and loads the 16-bit boot system to the highest address possible, right under
 * `0x90000', assuming the memory until `0x90000' is free (the EBDA starts
 * somewhere around `0x9fc00', but it is not standardized, AFAIK). This way, the
 * boot system has about 512 KiB free memory, which simply must suffice.
 * Technically, we can do crazy stuff such as switching to Unreal Mode, which
 * gives us a maximum of 4 GiB, but that should not be necessary.
 * The boot system loads the kernel in turn to address `0x0'. It has free
 * space until the start address of the boot system. Should this prove
 * insufficient, we can either split the kernel in two parts, below and over
 * 1 MiB, or do it as Linux does, i.e., load the whole kernel over 1 MiB. This
 * is actually the cleanest way I think, but I do not think the kernel is
 * getting that big, so we can load the kernel in the first MiB and user
 * programs above.
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

	
	/* Now, load the Real Mode part of the kernel, effectively, a
	 * second-stage bootloader. Retry three times, as recommended by
	 * RBIL. */
	xor %bp, %bp
1:
	mov $BOOTSYS_TEXT / 0x10, %ax
	mov %ax, %es
	mov $BOOTSYS_TEXT % 0x10, %bx
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

/* TODO: verify at preprocessing time that bootsys_seg:bootsys_off is aligned
 * on word oundaries! */
ljmp $BOOTSYS_TEXT / 0x10, $BOOTSYS_TEXT % 0x10

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
