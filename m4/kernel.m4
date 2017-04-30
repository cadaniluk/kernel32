ifdef(`KERNEL_M4',,dnl
include(`boot.m4')
dnl
/* TODO: as long as we use the BIOS disk routines we must not
 * override the IVT and BDA (im not so sure about the BDA, but let's 
 * not override it anyway), so we need to load the kernel to 0x500.
 * however, as soon as our custom floppy driver is ready, we will
 * use it and load the kernel to 0x0. we could also load to 0x500
 * and then move the thing, but thats inefficient and the benefit
 * is minimal. */
define(`KERNEL_ORG', `0x500')dnl
define(`KERNEL_MAX_LEN', `(KERNEL_SECTORS * 512)')dnl
dnl
) /* `KERNEL_M4' */
