ifdef(`KERNEL_PARAMS_H',, `
define(`KERNEL_PARAMS_H')

include(`kernel.m4')

/* TODO: change to 0x0 as soon as we have all drivers ready and are BIOS-
 * independent. */
define(`KP_BASE', `0x500')
define(`KP_E820_MAP', `KP_BASE')

define(`KP_SIZE', `(KERNEL_ORG - KP_BASE)')

')