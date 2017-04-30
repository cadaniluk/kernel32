ifdef(`BOOT_H',,dnl
define(`BOOT_H')dnl
define(`BOOT_ORG', `0x7c00')dnl
define(`BOOT_LEN', `0x200')dnl
dnl
define(`BOOTSYS_LEN', `0x10000')
dnl
define(`DEF_ATTR', `0xf') /* default */
define(`OK_ATTR', `0x2f') /* for successful operations */
define(`ERR_ATTR', `0x4f') /* for errors */
dnl
/* TODO: make more flexible by reading from cmos! */
/* Format of a 1.44 MiB floppy. */
define(`CYLINDERS', `80')
define(`HEADS', `2')
define(`SECTORS', `18')
/*
 * $1 - Heads per cylinder
 * $2 - Sectors per track
 * $3 - Cylinder number
 * $4 - Head number
 * $5 - Sector number (starts at 1)
 */
define(`CHS_TO_LBA', `eval(`(($3 * $1 + $4) * $2) + $5 - 1')')
dnl
/* $1 - Heads per cylinder
 * $2 - Sectors per track
 * $3 - LBA value
 */
define(`LBA_TO_C', `eval(`$3 / ($1 * $2)')')
dnl
/*
 * $1 - Heads per cylinder
 * $2 - Sectors per track
 * $3 - LBA value
 */
define(`LBA_TO_H', `eval(`($3 % ($1 * $2)) / $2')')
dnl
/*
 * $1 - Heads per cylinder
 * $2 - Sectors per track
 * $3 - LBA value
 */
define(`LBA_TO_S', `eval(`($3 % ($1 * $2)) % $2 + 1')')
dnl
define(`BOOTSYS_LBA', `0x1')
/* TODO: find correlation between bootsys_sectors and bootsys_size!
 * maybe just change bootsys_size to something smaller still < 64 kib, so
 * we can load as much sectors */
define(`BOOTSYS_SECTORS', `0x3')
dnl
define(`KERNEL_LBA', `(BOOTSYS_LBA + BOOTSYS_SECTORS)')

/* TODO: find a way to determine the number of kernel sectors in correlation to
 * the kernel size (like num_sectors = kernel_size >> 16). same for boot sectors. */
define(`KERNEL_SECTORS', `0x8')
)dnl
