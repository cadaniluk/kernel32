include(`descr.h')

.global kernel_start

.data

.balign 8
idt:
.skip 0x8 * 0x10 /* TODO: replace with descriptors! */
.set idt_len, . - idt

.balign 2
gdtr:
.word GDT_SIZE * DESCR_SIZE
.int gdt

.balign 2
idtr:
.word idt_len
.int idt


.text
kernel_start:
/*
 * Technically, we don't need to reload the GDTR register. However, booting
 * system and kernel are strictly kept apart and so are their addresses and
 * symbols. It's just easier to reload the GDTR register at a negligible
 * performance cost.
 */

/* TODO: replace 0x8 and 0x10 selectors with something not as hard-wired */
	mov $0x10, %ax
	mov %ax, %ds

	lgdt gdtr
	lidt idtr
	ljmp $0x8, $1f
1:
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	cli
	hlt
