/* Simple VGA driver for basic adapter interaction. */

#include <vga.h>

.code16

.global vga_init, vga_print

.data
/* TODO: allocate on heap or similar! */
curr_pos: .skip 0x2

.text
/*
 * Parameters:
 * `al' - starting line
 */
vga_init:
	mov $NUM_COLS * 2, %ah
	mul %ah

	mov %ax, curr_pos

	ret

/*
 * Parameters:
 * `si' - address of string
 * `bl' - attribute
 */
vga_print:
	push %es

	mov $VID_SEG, %ax
	mov %ax, %es
	mov curr_pos, %di
	jmp 2f

1:
	/* The UNIX way: a newline character does a full CRLF. */
	mov curr_pos, %dx

	/* Newline Feed */
	add $NUM_COLS * 2, %di

	/* Carriage Return */
	mov %di, %ax
	mov $NUM_COLS * 2, %dl
	div %dl
	movzx %ah, %ax
	sub %ax, %di

2:
	lodsb
	test %al, %al
	jz 3f

	cmp $'\n, %al
	je 1b

	mov %bl, %ah
	stosw
	jmp 2b

3:
	mov %di, curr_pos
	shr %di
	mov %di, %bx

	mov $VGA_CRTC_ADDR(VGA_IOS_VALUE), %dx
	mov $VGA_CLOC_LO, %al
	out %al, %dx
	mov $VGA_CRTC_DATA(VGA_IOS_VALUE), %dx
	mov %bl, %al
	out %al, %dx

	mov $VGA_CRTC_ADDR(VGA_IOS_VALUE), %dx
	mov $VGA_CLOC_HI, %al
	out %al, %dx
	mov $VGA_CRTC_DATA(VGA_IOS_VALUE), %dx
	mov %bh, %al
	out %al, %dx

	pop %es
	ret
