define(`USER_INT_START', `0x20') /* first usable interrupt */

.global install_ints

SEC_R_DATA(`.bootsys_data')
user_ints_tbl:
.word vga_entry

.set num_user_ints, (. - user_ints_tbl) / 0x2

/* I had the choice between creating a macro for the interrupt installation
 * (much repetitive code) or using a loop and storing the addresses in a table
 * (less code, but more data). I chose the latter approach. Better for
 * debugging too. */

SEC_TEXT(`.bootsys_text')
/* TODO: should i make this a macro? */
install_ints:
	mov BOOTSYS_SEG, %ax
	mov %ax, %es
	mov %ax, %ds
	mov $user_ints_tbl, %si
	mov $USER_INT_START * 0x4, %di
	mov $num_user_ints, %cx
1:
	test %cx, %cx
	jz 2f
	movsw /* offset */
	stosw /* segment (offset first due to little-endian) */
	jmp 1b
2:
	ret
