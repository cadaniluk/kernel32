#include <addrs.h>
#include <sec_boot_params.h>
#include <drivers/cmos.h>
#include <drivers/vga.h>

.code16

/*
 * TODO: add macro constant SECTOR_SIZE = 512 and replace all its former
 * occurences as a magic constant. if the sector size turns out to not be fixed
 * (it can be 512 or 4096 or whatever bytes, depending on the disk used),
 * find something to determine that at runtime and use that value. replace all
 * occurences of SECTOR_SIZE with that dynamically determined value.
 */

/*
 * `ss' is not simply set to `0x0' here to indicate an underflow of the stack
 * with the overflow flag (if `sp' underflows).
 */
#define BOOT_SS 0x50 /* right after BDA */
#define BOOT_SP (BOOT_ORG - BOOT_SS * 0x10)

#define BDA_EBDA_BASE 0x40e /* EBDA address = base_address:0x0 */

.global boot_start

.data
.balign 2

/* Stores the current cursor location. */
cursor_loc:
.word 0x0

verify_cpu_msg: .asciz "Verifying CPU"
cpu32_msg: .asciz "32-bit compatible"
not_cpu32_msg: .asciz "32-bit incompatible"

check_space_msg: .asciz "Checking if space suffices"
too_little_space_msg: .asciz "Space too small"
space_sufficient_msg: .asciz "Space suffices"

disk_err_msg: .ascii "Disk OP fail (code 0x"
disk_code: .asciz "cc)"

sec_boot_jmp_msg: .asciz "Jumping to secondary bootloader"
press_key_msg: .asciz "Press a key to reboot"

.text
boot_start:
	/* Initialize current line to zero. The values of memory locations
	 * are indeterminate on reset. */
	movb $0x0, SBP_CURR_LINE

	/* Obtain drive number we are booting from. */
	mov %dl, SBP_DRV_NUM

	/*
	 * Set the IOS bit of the Miscellaneous Output Register of the CRTC.
	 * Instead of retrieving it and using that retrieved value we set it
	 * explicitly, so we know the addresses already at compile time. I cannot
	 * see any penalty in setting it manually.
	 */
	mov $VGA_MISC_OUT_RD, %dx
	in %dx, %al
#if VGA_IOS_VALUE == 0
	and $~0x1 << BIT_POS(VGA_IOS), %al
#else
	or $0x1 << BIT_POS(VGA_IOS), %al
#endif
	mov $VGA_MISC_OUT_WR, %dx
	out %al, %dx

	/*
	 * We do not need interrupts anywhere in the bootloader, so we
	 * disable them right away.
	 */
	cli

	/*
	 * Upward direction is the default and does not change throughou the
	 * whole booting process.
	 */
	cld

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

	mov $verify_cpu_msg, %si
	mov $DEF_ATTR, %bl
	call puts
	
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

	mov $cpu32_msg, %si
	mov $OK_ATTR, %bl
	call puts

	/* Get EBDA start address and check if kernel size is OK. */

	mov $check_space_msg, %si
	mov $DEF_ATTR, %bl
	call puts

	/* TODO: BDA 0x40e or int $0x12? is any of them unreliable in some
	 * situations/on some systems? */
	/*
	 * Get the address of the BDA shifted to the right by 0x4 bits. It seems
	 * that the EBDA is always aligned to paragraph (16-byte, damn Real Mode)
	 * boundaries.
	 */
	movzwl BDA_EBDA_BASE, %eax
	shl $0x4, %eax

	cmp $KERNEL_ORG + KERNEL_LEN, %eax	/* does everything fit? */
	jbe too_little_space

	mov $space_sufficient_msg, %si
	mov $OK_ATTR, %bl
	call puts

	/*
	 * Now, load the Real Mode part of the kernel, effectively, a
	 * second-stage bootloader. Retry three times because the read
	 * command does not have a timeout and the external device could be slower
	 * than the CPU.
	 */
	/* TODO: ensure that the load doesnt cross a 64kib boundary */
	xor %bp, %bp
	mov %bp, %es
	mov $BOOTSYS_ORG, %bx
	mov $(LBA_TO_C(HEADS, SECTORS, BOOTSYS_LBA) << 0x8) | dnl
	LBA_TO_S(HEADS, SECTORS, BOOTSYS_LBA), %cx
	mov $(LBA_TO_H(HEADS, SECTORS, BOOTSYS_LBA) << 0x8), %dh
	mov SBP_DRV_NUM, %dl

1:
	mov $0x200 | BOOTSYS_SECTORS, %ax
	int $0x13
	jnc 1f
	cmp $0x2, %bp
	je disk_err
	inc %bp
	xor %ah, %ah
	int $0x13
	jmp 1b

1:
	mov $sec_boot_jmp_msg, %si
	mov $DEF_ATTR, %bl
	call puts

	ljmp $0x0, $BOOTSYS_ORG

/* TODO: change that to something more sophisticated later! */
no_32b:
	mov $not_cpu32_msg, %si
	mov $ERR_ATTR, %bl
	call puts

	jmp reboot

disk_err:
	/*
	 * The disk error table is too large for the primary bootloader, so
	 * we print the error code and rely on the user to find out the meaning.
	 */
	xor %bx, %bx
	mov %bx, %es
	mov $disk_code, %di
	call putx8
	mov $disk_err_msg, %si
	mov $ERR_ATTR, %bl
	call puts

	jmp reboot

too_little_space:
	mov $too_little_space_msg, %si
	mov $ERR_ATTR, %bl
	call puts

	jmp reboot

/*
 * `ds:si' - string to print
 * `bl' - character attribute
 * Very basic - only prints a string and goes to the next line then.
 */
puts:
	push %ax
	push %si
	push %di
	push %es

	mov $NUM_COLS * 2, %al /* `* 2' for char + attr */
	mulb SBP_CURR_LINE
	mov %ax, %di
	incb SBP_CURR_LINE

	mov $VID_SEG, %ax
	mov %ax, %es
	mov %bl, %ah

1:
	lodsb
	test %al, %al
	jz 2f
	stosw
	jmp 1b

2:
	/* Write back the location to main memory. */
	shr %di
	mov %di, cursor_loc

	/*
	 * Update the cursor location. Note that x86 is little-endian, so we need to
	 * load the single bytes in reverse into the CRTC registers.
	 */
	mov $VGA_CRTC_ADDR(VGA_IOS_VALUE), %dx
	mov $VGA_CLOC_LO, %al
	out %al, %dx
	mov $VGA_CRTC_DATA(VGA_IOS_VALUE), %dx
	mov cursor_loc, %al
	out %al, %dx

	mov $VGA_CRTC_ADDR(VGA_IOS_VALUE), %dx
	mov $VGA_CLOC_HI, %al
	out %al, %dx
	mov $VGA_CRTC_DATA(VGA_IOS_VALUE), %dx
	mov cursor_loc + 0x1, %al
	out %al, %dx

	pop %es
	pop %di
	pop %si
	pop %ax

	ret

/*
 * `ah' - number to print (disk routines return the error code in `ah
 * `ds:si' - 16-bit memory location to write the result to
 * Converts an eight-bit hexadecimal number to ASCII and writes it to a 16-bit
 * memory location.
 */
putx8:
	push %ax
	push %cx
	push %di

	/*
	 * The conversion to hexadecimal ASCII is basically the same as
	 * n += (n <= 9 ? 30 : 51);
	 */
	shr $0x4, %ax
	shr $0x4, %al
	mov $0x2, %cx

1:
	xchg %al, %ah
	cmp $0x9, %al
	jae 2f
	add $0x30, %al	/* to digit */
	jmp 3f

2:
	add $0x57, %al	/* to letter */

3:
	dec %cx
	jnz 1b

	xchg %al, %ah
	stosw

	pop %di
	pop %cx
	pop %ax
	ret

reboot:
	mov $press_key_msg, %si
	mov $DEF_ATTR, %bl
	call puts

	/* Read a keystroke. */
	xor %ah, %ah
	int $0x16

	ljmp $0xf000, $0xfff0
