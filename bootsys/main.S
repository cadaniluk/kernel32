.code16

.global bootsys_start

.data

.text
bootsys_start:
	mov $0xaa55, %ax
	cli
	hlt
