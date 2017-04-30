ifdef(`SEC_BOOT_PARAMS_H',,
define(`SEC_BOOT_PARAMS_H')

define(`SBP_BASE', `0x500')
define(`SBP_DRV_NUM', `SBP_BASE')
define(`SBP_CURR_LINE', `(SBP_BASE + 0x1)')
define(`SBP_CRTC_ADDR', `SBP_BASE + 0x2')

)
