#include <boot.h>
#include <mm/seg.h>
#include <addrs.h>
#include <crn.h>
#include <drivers/i8259A.h>

.code16

/* Parameters: address of string, attribute */
define(`VGA_PRINT', `dnl
mov $1, %si
mov $2, %bl
call vga_print
')

.global bootsys_start

.data
tmp_gdt:
.word 0x0, 0x0, 0x0, 0x0
.word 0xffff, 0x0, 0x9800, 0xcf
.word 0xffff, 0x0, 0x9200, 0xcf
.set tmp_gdt_len, . - tmp_gdt

tmp_gdtr:
.word tmp_gdt_len - 1
.int tmp_gdt

newline: .asciz "\n"

mm_detect_msg: .asciz "Running memory detection\n"
mm_detect_fail_msg: .asciz "Memory detection failed\n"
mm_detect_succ_msg: .asciz "Memory detection succeeded\n"

load_kern_msg: .asciz "Loading the kernel\n"
load_kern_fail_msg: .asciz "Loading the kernel failed\n"
load_kern_succ_msg: .asciz "Loading the kernel succeeded\n"

remap_irqs_msg: .asciz "Remapping IRQs\n"
kern_jmp_msg: .asciz "Jumping to the kernel\n"

.section .text.bootsys_start
bootsys_start:
	xor %ax, %ax
	mov %ax, %ds
	mov %ax, %ss
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	/* TODO: add support for memory section allocation and put the stack
	 * in one section. currently, the stack starts at the beginning of the 
	 * kernel parameter map. look at doc/boot.txt for more info. */
	mov $KP_BASE, %sp

	mov SBP_CURR_LINE, %al
	call vga_init

	/* Detect memory. */
	VGA_PRINT($mm_detect_msg, $DEF_ATTR)
	call init_e820_map
	test %ax, %ax
	jz mm_detect_fail
	movzx %ax, %eax
	mov %eax, KP_E820_MAP_LEN
	VGA_PRINT($mm_detect_succ_msg, $OK_ATTR)

	/* Load the kernel. */
	/* TODO: replace with custom floppy driver, e.g. to be able to load more
	 * than 64 kib in one go. */
	VGA_PRINT($load_kern_msg, $DEF_ATTR)
	mov $0x3, %bp
	mov $(LBA_TO_C(HEADS, SECTORS, KERNEL_LBA) << 0x8) | \
	LBA_TO_S(HEADS, SECTORS, KERNEL_LBA), %cx
	mov $(LBA_TO_H(HEADS, SECTORS, KERNEL_LBA) << 0x8), %dh
	mov SBP_DRV_NUM, %dl
	mov $KERNEL_ORG >> 0x4, %ax
	mov %ax, %es

1:
	mov $KERNEL_ORG & 0xf, %bx
	mov $0x200 | KERNEL_SECTORS, %ax
	int $0x13
	jnc 1f

	dec %ah /* error message with ID 1 is at index `0x0' */
	movzx %ah, %bx
	shl %bx /*
             * I hate that ELF cannot store arbitrary arithmetical
             * operations on symbol addresses and therefore we cannot
             * just do `$symbol * constant`.
             */
	add $disk_status_table, %bx
	VGA_PRINT((%bx), $ERR_ATTR)
	VGA_PRINT(newline, $DEF_ATTR)

	dec %bp
	jz kern_load_fail

	/* Reset the disk system. */
	/* TODO: should the returned code also be checked here and an error message
	 * printed here? */
	xor %ah, %ah
	int $0x13

	jmp 1b

1:
	/* Reset `%es' to zero, so it's left in a known state. */
	xor %ax, %ax
	mov %ax, %es

	VGA_PRINT($load_kern_succ_msg, $OK_ATTR)

	/*
	 * Remap IRQs. Interrupts have been disabled in the
	 * bootloader already.
	 */
	
	VGA_PRINT($remap_irqs_msg, $DEF_ATTR)

	mov $i8259A_ICW1(i8259A_IC4), %al
	out %al, $i8259A_ICW1_ADDR(i8259A_MASTER)
	out %al, $i8259A_ICW1_ADDR(i8259A_SLAVE)

	mov $i8259A_ICW2(USER_INT_START), %al
	out %al, $i8259A_ICW2_ADDR(i8259A_MASTER)
	mov $i8259A_ICW2(USER_INT_START + 8), %al
	out %al, $i8259A_ICW2_ADDR(i8259A_SLAVE)

	mov $i8259A_ICW3(0x4), %al
	out %al, $i8259A_ICW3_ADDR(i8259A_MASTER)
	mov $i8259A_ICW3(0x2), %al
	out %al, $i8259A_ICW3_ADDR(i8259A_SLAVE)

	mov $i8259A_ICW4(i8259A_uPM & i8259A_x86), %al
	out %al, $i8259A_ICW4_ADDR(i8259A_MASTER)
	out %al, $i8259A_ICW4_ADDR(i8259A_SLAVE)

	lgdt tmp_gdtr

	VGA_PRINT($kern_jmp_msg, $DEF_ATTR)
	smsw %ax
	or $CR0_PE, %ax
	lmsw %ax

	/*
	 * First, we load `%cs' to enable 32-bit code, then we do the actual
	 * jump to the kernel. It is a little cumbersome, but I don't see another
	 * way to do this 32-bit jump.
	 */

	ljmp $TMP_C0_SEL, $1f
1:
.code32
	ljmp $TMP_C0_SEL, $KERNEL_ORG
.code16

mm_detect_fail:
	VGA_PRINT($mm_detect_fail_msg, $ERR_ATTR)
	cli
	hlt

kern_load_fail:
	VGA_PRINT($load_kern_fail_msg, $ERR_ATTR)
	cli
	hlt
