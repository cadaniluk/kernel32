include(`include/drivers/isa/i8259A.h')

.code16

.global bootsys_start

SEC_RW_DATA(`.bootsys_data')

SEC_TEXT(`.bootsys_text')
bootsys_start:
	

	cli
	hlt
