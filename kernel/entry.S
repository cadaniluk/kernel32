#include <mm/seg.h>
#include <boot.h>

.global kernel_start

.data

.balign 2
gdtr:
.word GDT_SIZE * DESCR_SIZE
.int gdt

.balign 2
idtr:
.word IDT_SIZE * DESCR_SIZE
.int idt


.section .text.kernel_start
kernel_start:
	/* We must use the descriptor from the temporary GDT to reload the GDTR. */
	mov $TMP_D0_SEL, %ax
	mov %ax, %ds

	/*
	 * The temporary GDT might be overwritten and is too small. To keep the
	 * interface between boot system and kernel clean, we just load a new
	 * GDT.
	 */

	lgdt gdtr
	ljmp $C0_SEL, $1f

1:
	mov $D0_SEL, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	mov %ax, %ss
	mov $0x70000, %esp

	lidt idtr
	call idt_init

	call mm_init

	cli
	hlt
