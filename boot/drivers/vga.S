/* Simple VGA driver for basic adapter interaction. */

include(`vga.h')

.global vga_init
.global vga_puts

SEC_RW_DATA(`.bootsys_data')
.balign 2

curr_pos: .word 0x0

SEC_TEXT(`.bootsys_text')
/*
 * `ah' - line where to start
 */
vga_init:
	push %ax

	mov $NUM_COLS << 0x1, %al
	mul %ah
	mov %ax, curr_pos

	pop %ax
	ret

/*
 * `ds:si' - string to print
 * `al' - character attribute
 */
vga_puts:
	push %ax
	push %bx
	push %dx
	push %si
	push %di
	push %es

	mov %al, %bl
	mov $VID_SEG, %ax
	mov %ax, %es
	mov curr_pos, %di
1:
	lodsb

	test %al, %al
	jz 4f
	cmp $0xa, %al
	je 2f
	cmp $0xd, %al
	je 3f

	mov %bl, %ah
	stosw
	jmp 1b
2:
	add $NUM_COLS << 0x1, %di
	jmp 1b
3:
	mov $NUM_COLS, %cx
	xor %dx, %dx
	mov %di, %ax
	div %cx
	sub %dx, %di
	
	jmp 1b
4:
	mov %di, curr_pos

	pop %es
	pop %di
	pop %si
	pop %dx
	pop %bx
	pop %ax
	ret
