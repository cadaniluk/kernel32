ifdef(`BOOT_H',,dnl
define(`BOOT_H')dnl
define(`BOOT_ORG', `0x7c00')dnl
define(`BOOT_LEN', `0x200')dnl
dnl
define(`BOOTSYS_LEN', `0x10000')
define(`BOOTSYS_SEG', `((0x90000 - BOOTSYS_LEN) >> 0x4)')dnl
/* `BOOTSYS_OFF' is `0x0'. The boot system is aligned to segment boundaries. */
dnl
)dnl
