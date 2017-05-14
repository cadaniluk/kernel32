ifdef(`KERNEL_M4',,dnl
include(`boot.m4')
dnl
define(`KERNEL_ORG', `0x1000')dnl
define(`KERNEL_MAX_LEN', `(KERNEL_SECTORS * 512)')dnl
dnl
) /* KERNEL_M4 */
