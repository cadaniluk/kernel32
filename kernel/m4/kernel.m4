ifdef(`KERNEL_M4',,dnl
/* TODO: find a cleaner way to make boot.m4 accessible to kernel.m4 */
include(`../boot/m4/boot.m4')
dnl
define(`KERNEL_ORG', `0x0')dnl
define(`KERNEL_MAX_LEN', `(KERNEL_SECTORS * 512)')dnl
dnl
) /* `KERNEL_M4' */
