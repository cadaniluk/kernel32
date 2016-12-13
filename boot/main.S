include(`i8259A.h')
include(`mm_detect.h')
/* TODO: super ugly! fix that! */
include(`../kernel/m4/kernel.m4')

define(`USER_INT_START', `0x20') /* first usable interrupt */
define(`i8259A_MASTER', `0x20')
define(`i8259A_SLAVE', `0xa0')

.global bootsys_start

/* TODO: maybe add some additional logging messages! */

SEC_R_DATA(`.bootsys_data')
.balign 8
tmp_gdt:
.word 0x0, 0x0, 0x0, 0x0
.word 0xffff, 0x0, 0x9800, 0xcf
.word 0xffff, 0x0, 0x9200, 0xcf
.set tmp_gdt_len, . - tmp_gdt

/* TODO: choose alignment to have address mod 4 = 2 */
.balign 2
tmp_gdtr:
.word tmp_gdt_len /* null, code, and data */
.int 0x0 /* to be initialized */

SEC_TEXT(`.bootsys_text')
bootsys_start:
	mov %cs, %ax
	mov %ax, %ds

	/* TODO: enable the A20 address line. */

	call mm_detect

	/* Load the kernel now. */

	xor %bp, %bp
1:
	mov $KERNEL_ORG >> 0x4, %ax
	mov %ax, %es
	mov $KERNEL_ORG & 0xf, %bx
	mov $0x200 | KERNEL_SECTORS, %ax
	mov $(LBA_TO_C(HEADS, SECTORS, KERNEL_LBA) << 0x8) | dnl
	LBA_TO_S(HEADS, SECTORS, KERNEL_LBA), %cx
	mov $(LBA_TO_H(HEADS, SECTORS, KERNEL_LBA) << 0x8) | FLOPPY_DRV, %dx
	int $0x13
	jnc 1f
	cmp $0x2, %bp
	je floppy_err
	inc %bp
	xor %ah, %ah
	int $0x13
	jmp 1b
1:

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

	/* Compute linear address of GDT to be loaded into the GDTR register. */
	xor %eax, %eax
	mov %cs, %ax
	shl $0x4, %eax
	add $tmp_gdt, %eax
	mov %eax, tmp_gdtr + 0x2

	/* TODO: lgdtl or lgdtw? maybe ask a question on SO on that... */
	lgdtl tmp_gdtr

	smsw %ax
	or $0x1, %ax
	lmsw %ax

	/* TODO: use named constant for selector here */
	ljmp $0x8, $KERNEL_ORG

floppy_err:
	/* TODO: add proper error display using the floppy err table.
	 * later, use the custom 16-bit floppy driver (if ever). */
	cli
	hlt
