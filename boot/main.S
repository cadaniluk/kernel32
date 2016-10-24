include(`include/i8259A.h')

define(`USER_INT_START', `0x20') /* first usable interrupt */

/*
 * Concerning the interrupt initialization code:
 * I had to decide between going with a macro, which defines code for each
 * IVT entry (much repetitive code, less data) or using a loop and storing the
 * addresses in a table (less code, but more data). I chose the latter approach.
 * Better for debugging too.
 */

.global bootsys_start

SEC_RW_DATA(`.bootsys_data')
/* TODO: alignment directives commented out due to issue #12. resolve that! */
/*.balign 2*/
user_ints_tbl:
.word vga_entry
/*.balign 1*/

.set num_user_ints, (. - user_ints_tbl) / 0x2

SEC_TEXT(`.bootsys_text')
bootsys_start:
	/* Install user-defined interrupts. */

        xor %ax, %ax
        mov %ax, %es
        mov $USER_INT_START * 0x4, %di
        mov $BOOTSYS_SEG, %ax
        mov %ax, %ds 
        mov $user_ints_tbl, %si
        mov $num_user_ints, %cx
1:
        test %cx, %cx
        jz 2f
        movsw
        stosw
        dec %cx
        jmp 1b
2:

	cli
	hlt
