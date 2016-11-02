include(`include/i8259A.h')
include(`include/mm_detect.h')

define(`USER_INT_START', `0x20') /* first usable interrupt */
define(`i8259A_MASTER', `0x20')
define(`i8259A_SLAVE', `0xa0')

.global bootsys_start

SEC_RW_DATA(`.bootsys_data')

SEC_TEXT(`.bootsys_text')
bootsys_start:
	/* Remap IRQs. */

	mov i8259A_ICW1($i8259A_IC4), %al
	out %al, i8259A_ICW1_ADDR($i8259A_MASTER)
	out %al, i8259A_ICW1_ADDR($i8259A_SLAVE)

	mov i8259A_ICW2($USER_INT_START), %al
	out %al, i8259A_ICW2_ADDR($i8259A_MASTER)
	mov i8259A_ICW2($USER_INT_START + 8), %al
	out %al, i8259A_ICW2_ADDR($i8259A_SLAVE)

	mov i8259A_ICW3($0x4), %al
	out %al, i8259A_ICW3_ADDR($i8259A_MASTER)
	mov i8259A_ICW3($0x2), %al
	out %al, i8259A_ICW3_ADDR($i8259A_SLAVE)

	mov i8259A_ICW4($i8259A_uPM & i8259A_x86), %al
	out %al, i8259A_ICW4_ADDR($i8259A_MASTER)
	out %al, i8259A_ICW4_ADDR($i8259A_SLAVE)

	call mm_detect
	

	cli
	hlt
