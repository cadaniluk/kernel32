ifdef(`BOOT_H',,`

define(`BOOT_TEXT', `0x7c00')
define(`BOOT_SIZE', `0x200')

define(`BOOTSYS_TEXT', `(0x90000 - BOOTSYS_SIZE)')
define(`BOOTSYS_SIZE', `0x10000')
')
