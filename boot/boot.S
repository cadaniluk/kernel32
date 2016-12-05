include(`include/cmos.h')

/* TODO: super ugly! rectify that! */
include(`../kernel/m4/kernel.m4')

/* TODO: i dont like how boot_start is subtracted by 2 here. its intended to
 * serve word-alignment, but thats way too hardcoded. find something more
 * flexible to achieve the same thing. */
/* `ss' is not simply set to `0x0' here to indicate an underflow of the stack
 * with the overflow flag (if `sp' underflows). */
define(`BOOT_SS', `0x50') /* right after BDA */
define(`BOOT_SP', `(BOOT_ORG - BOOT_SS * 0x10)')

/* TODO: optimize the whole code! not by using subtle and hard-to-
 * detect micro-optimizations but using calls to functions,
 * for example! the apm functions, for example, can all be wrapped into
 * call functions and thus we can save valuable bootloader space. */

define(`VID_SEG', `0xb800')
define(`NUM_COLS', `80')
define(`VID_ATTR', `0xf')

define(`BDA_EBDA_BASE', `0x40e') /* EBDA address = base_address:0x0 */

.global boot_start

.data
verify_cpu: .asciz "Verifying CPU..."
cpu32: .asciz "32-bit-compatible... ready to go!"
not_cpu32: .asciz "Not 32-bit-compatible... kernel cannot start up."
press_key: .asciz "Press a key to reboot..."

kernel_too_large_msg: .asciz "The kernel is too large."

curr_line: .byte 0x0
sec_boot_addr:
.word 0x0
.word 0x0 /* initialized later on */

.text
boot_start:
	/*
	 * We do not need interrupts anywhere in the bootloader, so we
	 * disable them right away.
	 */
	cli

	/* Set up a basic stack. */
	mov $BOOT_SS, %ax /* Stack segment points to right after the BDA */
	mov %ax, %ss
	mov $BOOT_SP, %sp /* word-aligned, so must be even */

	/* NOTE: this part must be 8086-compatible, so the checking process
	 * below can be performed on any x86 CPU! */

	/* Enable video mode `0x3'. */
	mov $0x3, %ax
	int $0x10

	/* Zero out `ds'. */
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
	 * TODO: Determine storage medium and checking validity of sector number
	 * according to those values.
	 */
	/* TODO: access cmos here! */

	/* Get EBDA start address and check if kernel size is OK. */

	mov BDA_EBDA_BASE, %ax /* get start seg addr of EBDA */
	sub $0x1000 + BOOTSYS_SECTORS << 0x5, %ax
	/*
	 * `0x1000': point to 64 KiB before EBDA
	 * `BOOTSYS_SECTORS << 0x5': make sure that loading the
	 * sectors does not cross 64 KiB boundaries because that makes
	 * the BIOS routine err
	 */

	mov %ax, sec_boot_addr + 0x2 /* for far jmp to secondary bootloader */
	mov %ax, %dx
	sub $KERNEL_ORG >> 0x4, %dx /* get paragraphs available for kernel */
	cmp $KERNEL_SECTORS << 0x5, %dx /* shift does sectors -> paragraphs */
	jb kernel_too_large
	/* `ax' still contains the start segment address. */
	
	/* Now, load the Real Mode part of the kernel, effectively, a
	 * second-stage bootloader. Retry three times because the read
	 * command does not have a time-out. */
	xor %bp, %bp
1:
	mov sec_boot_addr + 0x2, %ax /* EBDA seg addr (off is `0x0') */
	mov %ax, %es
	xor %bx, %bx
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

ljmp *sec_boot_addr

/* TODO: change that to something more sophisticated later! */
no_32b:
	mov $not_cpu32, %si
	call puts

	jmp reboot

floppy_err:
	/* TODO: add better user info here! */
	jmp reboot

kernel_too_large:
	mov $kernel_too_large, %si
	call puts

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
