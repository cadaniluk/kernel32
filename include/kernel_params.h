ifdef(`KERNEL_PARAMS_H',, `
define(`KERNEL_PARAMS_H')

include(`kernel.m4')

define(`KP_BASE', `0x0')
define(`KP_E820_MAP', `KP_BASE')

define(`KP_SIZE', `KERNEL_ORG')

')