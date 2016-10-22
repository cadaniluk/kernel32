/* Simple VGA driver for basic adapter interaction. */

.global vga_entry

SEC_RW_DATA(`.bootsys_data')

SEC_TEXT(`.bootsys_text')
vga_entry:
