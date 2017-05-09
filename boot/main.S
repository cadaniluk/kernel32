include(`sec_boot_params.h')
include(`i8259A.h')
include(`mm_detect.h')
include(`descr.h')
include(`crn.h')
include(`kernel.m4')

define(`DEF_ATTR', `0xf')

.global bootsys_start

/* TODO: maybe add some additional logging messages! */

SEC_RW_DATA(`.bootsys_data')
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

.balign 2
/* TODO: printing the crlf alone is pretty inefficient. it is no necessity,
 * but improving that would indeed be nice. */
crlf: .asciz "\r\n"
mm_detect_msg: .asciz "Running memory detection\r\n"
load_kern_msg: .asciz "Loading the kernel (retrying three times)\r\n"
load_kern_ok_msg: .asciz "Loading the kernel succeded\r\n"
load_kern_fail_msg: .asciz "Loading the kernel failed\r\n"
remap_irqs_msg: .asciz "Remapping IRQs\r\n"
set_pe_msg: .asciz "Setting PE flag\r\n"
kern_jmp_msg: .asciz "Jumping into kernel code\r\n"

SEC_TEXT(`.bootsys_text')
bootsys_start:
	mov %cs, %ax
	mov %ax, %ds

	xor %ax, %ax
	mov %ax, %es
	mov %es:SBP_CURR_LINE, %ah
	call vga_init

	/* TODO: enable the A20 address line. */

	mov $mm_detect_msg, %si
	mov $DEF_ATTR, %al
	call vga_puts

	call mm_detect

	/* Load the kernel now. */

	mov $load_kern_msg, %si
	mov $DEF_ATTR, %al
	call vga_puts

	xor %bp, %bp
1:
	mov $KERNEL_ORG >> 0x4, %ax
	mov %ax, %es
	mov $KERNEL_ORG & 0xf, %bx
	mov $0x200 | KERNEL_SECTORS, %ax
	mov $(LBA_TO_C(HEADS, SECTORS, KERNEL_LBA) << 0x8) | dnl
	LBA_TO_S(HEADS, SECTORS, KERNEL_LBA), %cx
	mov $(LBA_TO_H(HEADS, SECTORS, KERNEL_LBA) << 0x8), %dh
	mov SBP_DRV_NUM, %dl
	int $0x13
	jnc 1f
	dec %ah /* error message with ID 1 is at index `0x0' */
	xor %bx, %bx
	mov %ah, %bl
	shl %bx /* I hate that ELF cannot store arbitrary arithmetical
		 * operations on symbol addresses and therefore we cannot
		 * just do `$symbol * constant`. */
	add $disk_status_table, %bx
	mov (%bx), %si
	mov $ERR_ATTR, %al
	call vga_puts
	mov $crlf, %si
	/* character attribute not needed for CR and LF */
	call vga_puts
	cmp $0x2, %bp
	je kern_load_fail
	inc %bp
	xor %ah, %ah
	int $0x13
	jmp 1b
1:
	mov $load_kern_ok_msg, %si
	mov $OK_ATTR, %al
	call vga_puts

	/*
	 * Remap IRQs. Interrupts have been disabled in the
	 * bootloader already.
	 */

	mov $remap_irqs_msg, %si
	mov $DEF_ATTR, %al
	call vga_puts

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

	mov $set_pe_msg, %si
	mov $DEF_ATTR, %al
	call vga_puts

	mov $kern_jmp_msg, %si
	mov $DEF_ATTR, %al
	call vga_puts

	smsw %ax
	or $CR0_PE, %ax
	lmsw %ax

	ljmp $C0_SEL, $KERNEL_ORG

kern_load_fail:
	mov $load_kern_fail_msg, %si
	mov $ERR_ATTR, %al
	call vga_puts
