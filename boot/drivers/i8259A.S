include(`include/i8259A.h')

.global i8259A_init
/*.global i8259A_destroy*/ /* TODO: add if necessary! */

define(`def_i8259A',
.global i8259A$1_rd_reg

.global i8259A$1_buf_icw1
.global i8259A$1_buf_icw2
.global i8259A$1_buf_icw3
.global i8259A$1_buf_icw4

.global i8259A$1_buf_ocw1
.global i8259A$1_buf_ocw2
.global i8259A$1_buf_ocw3

.comm i8259A$1_rd_reg 1

.comm i8259A$1_buf_icw1 1
.comm i8259A$1_buf_icw2 1
.comm i8259A$1_buf_icw3 1
.comm i8259A$1_buf_icw4 1

.comm i8259A$1_buf_ocw1 1
.comm i8259A$1_buf_ocw2 1
.comm i8259A$1_buf_ocw3 1
)

.data
/**/
i8259A_rd_reg: .byte 0x0

.text
i8259A_init:
	ret

/* TODO: add i8259A_destroy if needed! */
