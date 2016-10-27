ifdef(`ASM_M4',,`dnl
define(`ASM_M4')dnl
define(`__ASSEMBLER__', `1')dnl
dnl
define(`SEC_RW_DATA', `.section $1, "aw", @progbits')dnl
define(`SEC_R_DATA', `.section $1, "a", @progbits')dnl
define(`SEC_TEXT', `.section $1, "ax"')dnl
define(`SEC_BSS', `.section $1, "aw", @nobits')dnl
dnl
define(`_BIT_POS', `dnl
ifelse(eval(`($1) >> ($2) & 0x1'), `1', `$2', `_BIT_POS(`$1', incr(`$2'))')dnl
')dnl
define(`BIT_POS', `_BIT_POS(`$1', `0')')
')dnl
