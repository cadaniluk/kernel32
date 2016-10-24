include(`include/i8259A.h')

.global i8259A_wait

SEC_TEXT(`.bootsys_text')
/* TODO: replace with something more sophisticated! */
i8259A_wait:
	jmp 1f
1:
	jmp 1f
1:
	ret
