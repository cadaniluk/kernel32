ifdef(`ASM_M4',,`dnl
define(`ASM_M4')dnl
define(`__ASSEMBLER__', `1')dnl
dnl
define(`SEC_RW_DATA', `.section $1, "aw", @progbits')dnl
define(`SEC_R_DATA', `.section $1, "a", @progbits')dnl
define(`SEC_TEXT', `.section $1, "ax"')dnl
')dnl
