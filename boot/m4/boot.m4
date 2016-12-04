ifdef(`BOOT_H',,dnl
define(`BOOT_H')dnl
define(`BOOT_ORG', `0x7c00')dnl
define(`BOOT_LEN', `0x200')dnl
dnl
define(`BOOTSYS_LEN', `0x10000')
define(`BOOTSYS_ORG', `(0x90000 - BOOTSYS_LEN)')dnl
define(`BOOTSYS_SEG', `(BOOTSYS_ORG >> 0x4)')dnl
dnl
)dnl
