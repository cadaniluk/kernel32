include(`include/i8259A.h')

define(`USER_INT_START', `0x20') /* first usable interrupt */

.global bootsys_start

SEC_RW_DATA(`.bootsys_data')

SEC_TEXT(`.bootsys_text')
bootsys_start:
	cli
	hlt
