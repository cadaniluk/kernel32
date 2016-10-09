define(`BOOTSYS_SEG', `0x7c0')
define(`BOOTSYS_OFF', `0x200')
define(`BOOTSYS_SIZE', `0x1')
define(`BOOTSYS_C', `0x0')
define(`BOOTSYS_H', `0x0')
define(`BOOTSYS_S', `0x1')
define(`FLOPPY_DRV', `0x0')
/* TODO: optimize the whole code! not by using subtle and hard-to-
 * detect micro-optimizations but using calls to functions,
 * for example! the apm functions, for example, can all be wrapped into
 * call functions and thus we can save valuable bootloader space. */

/* TODO: add real mode support! add protected mode 16-bit support! */

/* The bootloader is loaded to physical address `0x7c00'. It then loads the
 * remaining boot system to `0x400' (right after the IVT), effectively
 * overriding the BDA (interrupts are disabled to prevent BDA changes).

.code16

.global boot_start

.data

.text
boot_start:
	/* NOTE: this part must be 8086-compatible, so the checking process
	 * below can be performed on any x86 CPU! */

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
	mov $0x2 | BOOTSYS_SIZE, %ax
	mov (BOOTSYS_C << 8) | BOOTSYS_S, %cx
	mov (BOOTSYS_H << 8) | FLOPPY_DRV, %dx 
	int $0x13
	jnc 1f
	cmp $0x2, %bp
	je floppy_err
	inc %bp
	xor %ah, %ah
	int $0x13
	jmp 1b
1:

/* TODO: the bootloader is small but not that small that there is no more space
 * available in it for more code than currently written. so you can add more
 * to this loader! but do not let it get all too messy! */


/* TODO: verify at preprocessing time that bootsys_seg:bootsys_off is aligned
 * on word oundaries! */
ljmp $BOOTSYS_SEG, $BOOTSYS_OFF

/* TODO: change that to something more sophisticated later! */
no_32b:
/* TODO: move floppy_err someplace else. */
floppy_err:
	cli
	hlt
