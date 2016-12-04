include(`include/i8259A.h')
include(`include/mm_detect.h')
/* TODO: super ugly! fix that! */
include(`../kernel/m4/kernel.m4')

define(`USER_INT_START', `0x20') /* first usable interrupt */
define(`i8259A_MASTER', `0x20')
define(`i8259A_SLAVE', `0xa0')

.global bootsys_start

/* TODO: maybe add some additional logging messages! */

SEC_R_DATA(`.bootsys_data')

SEC_TEXT(`.bootsys_text')
bootsys_start:
	/*
	 * Remap IRQs. Interrupts have been disabled in the
	 * bootloader already.
	 */

	mov i8259A_ICW1($i8259A_IC4), %al
	out %al, i8259A_ICW1_ADDR($i8259A_MASTER)
	out %al, i8259A_ICW1_ADDR($i8259A_SLAVE)

	mov i8259A_ICW2($USER_INT_START), %al
	out %al, i8259A_ICW2_ADDR($i8259A_MASTER)
	mov i8259A_ICW2($USER_INT_START + 8), %al
	out %al, i8259A_ICW2_ADDR($i8259A_SLAVE)

	mov i8259A_ICW3($0x4), %al
	out %al, i8259A_ICW3_ADDR($i8259A_MASTER)
	mov i8259A_ICW3($0x2), %al
	out %al, i8259A_ICW3_ADDR($i8259A_SLAVE)

	mov i8259A_ICW4($i8259A_uPM & i8259A_x86), %al
	out %al, i8259A_ICW4_ADDR($i8259A_MASTER)
	out %al, i8259A_ICW4_ADDR($i8259A_SLAVE)

	call mm_detect

	/* The kernel is loaded now. */

	xor %bp, %bp
1:
	mov $KERNEL_ORG >> 0x4, %ax
	mov %ax, %es
/* TODO: add rounding up here for kernel_len! */
	mov $KERNEL_ORG & 0xf, %bx
	mov $0x200 | KERNEL_SECTORS, %ax
	mov $(KERNEL_C << 0x8) | KERNEL_S, %cx
	mov $(KERNEL_H << 0x8) | FLOPPY_DRV, %dx
	int $0x13
	jnc 1f
	cmp $0x2, %bp
	je floppy_err
	inc %bp
	xor %ah, %ah
	int $0x13
	jmp 1b
1:
	/* TODO: go to protected mode here! */
	cli
	hlt

floppy_err:
	/* TODO: add proper error display using the floppy err table.
	 * later, use the custom 16-bit floppy driver (if ever). */
	cli
	hlt
