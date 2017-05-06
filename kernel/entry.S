include(`descr.h')

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


.text
kernel_start:
/*
 * Technically, we don't need to reload the GDTR register. However, booting
 * system and kernel are strictly kept apart and so are their addresses and
 * symbols. It's just easier to reload the GDTR register at a negligible
 * performance cost.
 */
	mov $0x10, %ax
	mov %ax, %ds

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

	cli
	hlt
