define(`USER_INT_START', `0x20') /* first usable interrupt */

.global install_ints

SEC_R_DATA(`.bootsys_data')
user_ints_tbl:
.word SEG(vga_entry), OFF(vga_entry)

.set num_user_ints, (. - user_ints_tbl) / 0x4

/* I had the choice between creating a macro for the interrupt installation
 * (much repetitive code) or using a loop and storing the addresses in a table
 * (less code, but more data). I chose the latter approach. Better for
 * debugging too. */

/* TODO: how do i organize the segmentation? give every segment a purpose?
 * treat them with some nice virtualization mechanism? */

SEC_TEXT(`.bootsys_text')
/* TODO: should i make this a macro? */
install_ints:
	mov SEG(ABS_ADDR($user_ints_tbl)), %ax
	mov %ax, %es
	mov $user_ints_tbl, %si
	mov $USER_INT_START * 0x4, %di
	mov $num_user_ints, %cx
1:
	test %cx, %cx
	jz 2f
	movsw
	movsw
	jmp 1b
2:
	ret
