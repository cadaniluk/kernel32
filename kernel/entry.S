.global kernel_start

.data
/*
 * Technically, we don't need to reload the GDTR register. However, booting
 * system and kernel are strictly kept apart and so are their addresses and
 * symbols. It's just easier to reload the GDTR register at a negligible
 * performance cost.
 */
.balign 8
gdt:
.word 0x0, 0x0, 0x0, 0x0
.word 0xffff, 0x0, 0x9800, 0xcf /* ring 0 code */
.word 0xffff, 0x0, 0x9200, 0xcf /* ring 0 data */
.skip 0x8 * 0x10 - (. - gdt)
.set gdt_len, . - gdt

.balign 8
idt:
.skip 0x8 * 0x10 /* TODO: replace with descriptors! */
.set idt_len, . - idt

.balign 2
gdtr:
.word gdt_len
.int gdt

.balign 2
idtr:
.word idt_len
.int idt


.text
kernel_start:
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
