ifdef(`ASM_M4',,`
define(`ASM_M4')

define(`__ASSEMBLER__', `1')

define(`SEC_RW_DATA', `.section $1, "aw", @progbits')
define(`SEC_R_DATA', `.section $1, "a", @progbits')
define(`SEC_TEXT', `.section $1, "ax"')

')
