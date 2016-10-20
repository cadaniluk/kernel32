define(`USER_INT_START', `0x20') /* first usable interrupt */

.global install_ints

.section .bootsys_data

/* I had the choice between creating a macro for the interrupt installation
 * (much repetitive code) or using a loop and storing the addresses in a table
 * (less code, but more data). I chose the latter approach. Better for
 * debugging too. */

/* TODO: how do i organize the segmentation? give every segment a purpose?
 * treat them with some nice virtualization mechanism? */

.section bootsys_text
/* TODO: should i make this a macro? */
install_ints:
	mov $user_ints_tbl, %si
	mov $USER_INT_START * 0x4, %di
	mov $NUM_USER_INTS, %cx
1:
	test %cx, %cx
	jz 2f
	movsw
	movsw
	jmp 1b
2:
	ret
