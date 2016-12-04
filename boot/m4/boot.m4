ifdef(`BOOT_H',,dnl
define(`BOOT_H')dnl
define(`BOOT_ORG', `0x7c00')dnl
define(`BOOT_LEN', `0x200')dnl
dnl
define(`BOOTSYS_LEN', `0x10000')
dnl
define(`FLOPPY_DRV', `0x0')
dnl
define(`BOOTSYS_C', `0x0')
define(`BOOTSYS_H', `0x0')
define(`BOOTSYS_S', `0x2')
/* TODO: find correlation between bootsys_sectors and bootsys_size!
 * maybe just change bootsys_size to something smaller still < 64 kib, so
 * we can load as much sectors */
define(`BOOTSYS_SECTORS', `0x3')
dnl
define(`KERNEL_C', `0x0')
define(`KERNEL_H', `0x0')
/* TODO: change, so that s overflows to h and to c!
 * maybe write a macro like LBA_TO_CHS! */
define(`KERNEL_S', `(BOOTSYS_S + BOOTSYS_SECTORS)')

/* TODO: find a way to determine the number of kernel sectors in correlation to
 * the kernel size (like num_sectors = kernel_size >> 16). same for boot sectors. */
define(`KERNEL_SECTORS', `0x1')
)dnl
