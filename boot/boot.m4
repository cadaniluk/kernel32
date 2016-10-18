ifdef(`BOOT_H',,`dnl
define(`BOOT_H')dnl
define(`BOOT_TEXT', `0x7c00')dnl
define(`BOOT_SIZE', `0x200')dnl
dnl
define(`BOOTSYS_TEXT', `(0x90000 - BOOTSYS_SIZE)')dnl
define(`BOOTSYS_SIZE', `0x10000')dnl
')dnl
