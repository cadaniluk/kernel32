ifdef(`VGA_H',,
define(`VGA_H')

/* TODO: some registers have bits that need to be always set or cleared on
 * read or write. how can i ensure that in the implementation of the macros? */

/*
 * Concerning the I/O Address Select Flag and the choosing of the right address:
 * 0xb = 1011b
 * 0xd = 1101b
 * IOS = 0 -> 0xb
 * IOS = 1 -> 0xd
 * 0xb ^ 0110b = 0xd
 * 0xb ^ 0000b = 0xb
 * 0xb ^ (IOS << 2 | IOS << 1) <-> if IOS = 0 then 0xb else 0xd
 */

/* TODO: maybe generalize _VGA_MAP_BITS, so it takes the number bounds and 
 * an error message in addition. then remove _VGA_SCN and use _VGA_MAP_BITS
 * in place. */
/* Returns a bitfield according to the map numbers passed to it. */
define(`_VGA_MAP_BITS', `dnl
pushdef(`arg', `$1')dnl
ifelse(eval(`('arg`) < 0 || ('arg`) > 3'), `1', `errprint(`Map index out of 'dnl
`bounds (must be 0 <= index <= 3, is 'arg`).')', dnl
`(0x1 << ('arg`))dnl
popdef(`arg')dnl
ifelse(`$#', `1', , ` | _VGA_MAP_BITS(shift($@))')dnl
')')

/* General Registers: */

define(`VGA_MISC_OUT_RD', `0x3cc') /* Miscellaneous Output Register Read */
define(`VGA_MISC_OUT_WR', `0x3c2') /* Miscellaneous Output Register Write */
define(`VGA_IN_STAT0', `0x3c2') /* Input Status Register 0 */

/* Input Status Register 1 */ 
define(`VGA_IN_STAT1', `
pushdef(`arg', eval(`$1'))
ifelse(eval(arg` == 0 || 'arg` == 1'), `1',
`(0x3ba ^ ('arg` << 6 | 'arg` << 5))',
`errprint(`Argument is not `0' or `1' but 'dnl
changequote([,])dnl
`arg'dnl
changequote()dnl
`.')')
popdef(`arg')
')

define(`VGA_FEAT_CTRL_RD', `0x3ca') /* Feature Control Register Read */

/* Feature Control Register Write */
define(`VGA_FEAT_CTRL_WR', `
pushdef(`arg', eval(`$1'))
ifelse(eval(arg` == 0 || 'arg` == 1'), `1',
`(0x3ba ^ ('arg` << 6 | 'arg` << 5))',
`errprint(`Argument is not `0' or `1' but 'dnl
changequote([,])dnl
`arg'dnl
changequote()dnl
`.')')
popdef(`arg')
')

define(`VGA_VID_ENABLE_RD', `0x3c3') /* Video Subsystem Enable Read */
define(`VGA_VID_ENABLE_WR', `0x3c3') /* Video Subsystem Enable Write */

/* TODO: VSP and HSP work together to determine the number of lines.
 * i could summarize both and just have one bitfield for the number of lines.
 * however, they actually serve different purposes, that is, determining
 * whether a positive or negative horizontal/vertical retrace signal is issued.
 * can i ignore this functionality and just summarize them or do they have
 * two purposes at the same time? if yes, i'd need some hybrid solution, which
 * would require me to think of yet another way to treat bitfields. i wouldnt
 * welcome this. */
/* Miscellaneous Output Register Bits: */
define(`VGA_VSP', `0x80') /* Vertical Sync Polarity */
define(`VGA_HSP', `0x40') /* Horizontal Sync Polarity */
define(`VGA_CS', `0xc') /* Clock Select */
define(`VGA_ERAM', `0x2') /* Enable RAM */
define(`VGA_IOS', `0x1') /* I/O Address Select */

/* Clock Select Values: */
/* TODO: the ibm vga/xga manual says for the first two values that
 * they go together with a certain frequency in mhz. do the
 * dimensions given in the name (640/320 and 720/360) imply those frequencies?
 * or do i need to add them somehow in the name?
define(`VGA_640_320_HPEL', `0x0') /* Selects 25.175 MHz clock for 640/320
                                   * Horizontal PELs */
define(`VGA_720_360_HPEL', `0x1') /* Selects 28.322 MHz clock for 720/360
                                   * Horizontal PELs */
define(`VGA_EXT_CLOCK', `0x2') /* Selects External Clock */
/* TODO: can i remove this reserved value? does this hold for all
 * reserved values throughout the whole project? */
define(`VGA_CS_RES', `0x3') /* Reserved */

/* Input Status Register 0 Bits: */
define(`VGA_CI', `0x80') /* CRT Interrupt */
define(`VGA_SS', `0x10') /* Switch Sense */

/* Input Status Register 1 Bits: */
define(`VGA_VR', `0x8') /* Vertical Retrace */
define(`VGA_DE', `0x1') /* Display Enable */

/* Sequencer Registers: */

define(`VGA_SEQ_ADDR', `0x3c4') /* Address */
define(`VGA_SEQ_DATA', `0x3d5') /* Data */

define(`VGA_RESET', `0x0') /* Reset */
define(`VGA_CLOCK_MODE', `0x1') /* Clocking Mode */
define(`VGA_MAP_MASK', `0x2') /* Map Mask */
define(`VGA_CHARMAP_SEL', `0x3') /* Character Map Select */
define(`VGA_MEM_MODE', `0x4') /* Memory Mode */

/* Sequence Address Register Bits: */
define(`VGA_SEQ_IND', `0x7') /* (actually `VGA_SA') Sequencer Address */

/* Reset Register Bits: */
define(`VGA_SR', `0x2') /* Synchronous Reset */
define(`VGA_ASR', `0x1') /* Asynchronous Reset */

/* Clocking Mode Register Bits: */
define(`VGA_SO', `0x20') /* Screen Off */
define(`VGA_SH4', `0x10') /* Shift 4 */
define(`VGA_DC', `0x8') /* Dot Clock */
define(`VGA_SL', `0x4') /* Shift Load */
define(`VGA_D89', `0x1') /* 8/9 Dot Clocks */

/* Map Mask Register Bits: */

/* Map n Enable */
define(`VGA_MNE', `ifelse(`$#', `0', `0x0', `(_VGA_MAP_BITS($@))')')

/* Character Map Select Bits: */
define(`VGA_MAH', `0x20') /* Character Map A Select (MSB) */
define(`VGA_MBH', `0x10') /* Character Map B Select (MSB) */
define(`VGA_MAL', `0xc') /* Character Map A Select (LS bits) */
define(`VGA_MBL', `0x3') /* Character Map B Select (LS bits) */

/* Memory Mode Register Bits: */
define(`VGA_CH4', `0x8') /* Chain 4 */
define(`VGA_OE', `0x4') /* Odd/Even */
define(`VGA_EM', `0x2') /* Extended Memory */

/* CRT Controller Registers: */

/* Address */
define(`VGA_CRTC_ADDR', `dnl
pushdef(`arg', eval(`$1'))dnl
ifelse(eval(arg` == 0 || 'arg` == 1'), `1',dnl
`(0x3b4 ^ ('arg` << 6 | 'arg` << 5))',dnl
`errprint(`Argument is not `0' or `1' but 'dnl
changequote([,])dnl
`arg'dnl
changequote()dnl
`.')')dnl
popdef(`arg')dnl
')

/* Data */
define(`VGA_CRTC_DATA', `dnl
pushdef(`arg', eval(`$1'))dnl
ifelse(eval(arg` == 0 || 'arg` == 1'), `1',dnl
`(0x3b5 ^ ('arg` << 6 | 'arg` << 5))',dnl
`errprint(`Argument is not `0' or `1' but 'dnl
changequote([,])dnl
`arg'dnl
changequote()dnl
`.')')dnl
popdef(`arg')dnl
')


define(`VGA_HTOTAL', `0x0') /* Horizontal Total */
define(`VGA_HDISPON_END', `0x1') /* Horizontal Display-Enable End */
define(`VGA_START_HBLANK', `0x2') /* Start Horizontal Blanking */
define(`VGA_END_HBLANK', `0x3') /* End Horizontal Blanking */
define(`VGA_START_HRETR', `0x4') /* Start Horizontal Retracing Pulse */
define(`VGA_END_HRETR', `0x5') /* End Horizontal Retrace */
define(`VGA_VTOTAL', `0x6') /* Vertical Total */
define(`VGA_OVERFLOW', `0x7') /* Overflow */
define(`VGA_PRES_ROW_SCAN', `0x8') /* Preset Row Scan */
define(`VGA_MAX_SCAN_LINE', `0x9') /* Maximum Scan Line */
define(`VGA_CURSOR_START', `0xa') /* Cursor Start */
define(`VGA_CURSOR_END', `0xb') /* Cursor End */
define(`VGA_SADDR_HI', `0xc') /* Start Address High */
define(`VGA_SADDR_LO', `0xd') /* Start Address Low */
define(`VGA_CLOC_HI', `0xe') /* Cursor Location High */
define(`VGA_CLOC_LO', `0xf') /* Cursor Location End */
define(`VGA_VRETR_START', `0x10') /* Vertical Retrace Start */
define(`VGA_VRETR_END', `0x11') /* Vertical Retrace End */
define(`VGA_VDISPON_END', `0x12') /* Vertical Display-Enabled End */
define(`VGA_OFFSET', `0x13') /* Offset */
define(`VGA_UND_LOC', `0x14') /* Underline Location */
define(`VGA_START_VBLANK', `0x15') /* Start Vertical Blanking */
define(`VGA_END_VBLANK', `0x16') /* End Vertical Blanking */
define(`VGA_MODE_CTRL', `0x17') /* CRT Mode Control */
define(`VGA_LINE_CMP', `0x18') /* Line Compare */

/* CRTC Address Register Bits: */
define(`VGA_CRTC_IND', `0x1f') /* Index */

/* End Horizontal Blanking Register Bits: */
define(`VGA_DES', `0x60') /* Display Enable Skew Control */
define(`VGA_EB', `0x1f') /* End Blanking */

/* End Horizontal Retrace Register Bits: */
define(`VGA_EB5', `0x80') /* End Horizontal Blanking, Bit 5 */
define(`VGA_HRD', `0x60') /* Horizontal Retrace Delay */
define(`VGA_EHR', `0x1f') /* End Horizontal Retrace */

/* Overflow Register Bits: */
define(`VGA_VRS9', `0x80') /* Vertical Retrace Start, Bit 9 */
define(`VGA_VDE9', `0x40') /* Vertical Display Enable End, Bit 9 */
define(`VGA_VT9', `0x20') /* Vertical Total, Bit 9 */
define(`VGA_LC8', `0x10') /* Line Compare, Bit 8 */
define(`VGA_VBS8', `0x8') /* Vertical Blanking Start, Bit 8 */
define(`VGA_VRS8', `0x4') /* Vertical Retrace Start, Bit 8 */
define(`VGA_VDE8', `0x2') /* Vertical Display Enable End, Bit 8 */
define(`VGA_VT8', `0x1') /* Vertical Total, Bit 8 */

/* Preset Row Scan Register Bits: */
define(`VGA_BP', `0x60') /* Byte Panning */
define(`VGA_SRS', `0x1f') /* Starting Row Scan Count */

/* Maximum Scan Line Register Bits: */
define(`VGA_DSC', `0x80') /* 200 to Line Conversion (Double Scanning) */
define(`VGA_LC9', `0x40') /* Line Compare, Bit 9 */
define(`VGA_VBS9', `0x20') /* Start Vertical Blanking, Bit 9 */
define(`VGA_MSL', `0x1f') /* Maximum Scan Line */

/* Cursor Start Register Bits: */
define(`VGA_CO', `0x20') /* Cursor Off */
define(`VGA_RSCB', `0x1f') /* Row Scan Cursor Begins */

/* Cursor End Register Bits: */
define(`VGA_CSK', `0x60') /* Cursor Skew Control */
define(`VGA_RSCE', `0x1f') /* Row Scan Cursor Ends */

/* Vertical Retrace End Register Bits: */
define(`VGA_PR', `0x80') /* Protect Registers 0-7 */
define(`VGA_S5R', `0x40') /* Select 5 Refresh Cycles */
define(`VGA_EVI', `0x20') /* Enable Vertical Interrupt */
define(`VGA_CVI', `0x10') /* Clear Vertical Interrupt */
define(`VGA_VRE', `0xf') /* Vertical Retrace End */

/* Underline Location Register Bits: */
define(`VGA_DW', `0x40') /* Doubleword Mode */
define(`VGA_CB4', `0x20') /* Count By 4 */
define(`VGA_SUL', `0x1f') /* Start Underline */

/* CRT Mode Control Register Bits: */
define(`VGA_RST', `0x80') /* Hardware Reset */
define(`VGA_WB', `0x40') /* Word/Byte Mode */
define(`VGA_ADW', `0x20') /* Address Wrap */
define(`VGA_CB2', `0x8') /* Count By Two */
define(`VGA_HRS', `0x4') /* Horizontal Retrace Select */
define(`VGA_SRC', `0x2') /* Select Row Scan Counter */
define(`VGA_CMS0', `0x1') /* CMS 0 */

/* Graphics Controller Registers: */

define(`VGA_GRAPHC_ADDR', `0x3ce') /* Address */
define(`VGA_GRAPHC_DATA', `0x3cf') /* Data */

define(`VGA_RE_SET', `0x0') /* Set/Reset */
define(`VGA_ENABLE_RE_SET', `0x1') /* Enable Set/Reset */
define(`VGA_COLOR_CMP', `0x2') /* Color Compare */
define(`VGA_DATA_ROT', `0x3') /* Data Rotate */
define(`VGA_RD_MAP_SEL', `0x4') /* Read Map Select */
define(`VGA_GRAPH_MODE', `0x5') /* Graphics Mode */
define(`VGA_MISC', `0x6') /* Miscellaneous */
define(`VGA_COLOR_IGN', `0x7') /* Color Don't Care */
define(`VGA_BIT_MASK', `0x8') /* Bit Mask */

/* Graphics Controller Address Register Bits: */
define(`VGA_GRAPHC_IND', `0xf') /* Index */

/* Set/Reset Register Bits: */

/* Set/Reset Map n */
define(`VGA_SRN', `ifelse(`$#', `0', `0x0', `(_VGA_MAP_BITS($@))')')

/* Enable Set/Reset Register Bits: */ 

/* Enable Set/Reset Map n */
define(`VGA_ESRN', `ifelse(`$#', `0', `0x0', `(_VGA_MAP_BITS($@))')')

/* Color Compare Register Bits: */

/* Color Compare Map n */
define(`VGA_CCN', `ifelse(`$#', `0', `0x0', `(_VGA_MAP_BITS($@))')')

/* Data Rotate Register Bits: */
define(`VGA_FS', `0x18') /* Function Select */
define(`VGA_RC', `0x7') /* Rotate Count */

/* Read Map Select Register Bits: */
define(`VGA_MS', `0x3') /* Map Select */

/* Graphics Mode Register Bits: */
define(`VGA_C256', `0x40') /* 256 - Color Mode */
define(`VGA_SR', `0x20') /* Shift Register Mode */
define(`VGA_OE', `0x10') /* Odd/Even */
define(`VGA_RM', `0x8') /* Read Mode */
define(`VGA_WM', `0x3') /* Write Mode */

/* Miscellaneous Register Bits: */
define(`VGA_MM', `0xc') /* Memory Map */
define(`VGA_OE', `0x2') /* Odd/Even */
define(`VGA_GM', `0x1') /* Graphics Mode */

/* Memory Map Field Values: */
define(`VGA_MM_A0_128', `0x0') /* A0000 for 128KB */
define(`VGA_MM_A0_64', `0x1') /* A0000 for 64KB */
define(`VGA_MM_B0_32', `0x2') /* B0000 for 32KB */
define(`VGA_MM_B8_32', `0x3') /* B8000 for 32KB */

/* Color Don't Care Register Bits: */

/* Map n is Don't Care */
define(`VGA_MNX', `ifelse(`$#', `0', `0x0', `(_VGA_MAP_BITS($@))')')

/* Attribute Controller Registers: */

define(`VGA_ATTRC_ADDR', `0x3c0') /* Address */
define(`VGA_ATTRC_WR', `0x3c0') /* Register Write Address */
define(`VGA_ATTRC_RD', `0x3c1') /* Register Read Address */

define(`VGA_INTERN_PAL', `($1)') /* Internal Palette */
define(`VGA_ATTR_MODE_CTRL', `0x10') /* Attribute Mode Control */
define(`VGA_OVERSCAN_COLOR', `0x11') /* Overscan Color */
define(`VGA_COLOR_PLANE_ON', `0x12') /* Color Plane Enable */
define(`VGA_HPEL_PAN', `0x13') /* Horizontal PEL Panning */
define(`VGA_COLOR_SEL', `0x14') /* Color Select */

/* Attribute Controller Address Register Bits: */
define(`VGA_IPAS', `0x20') /* Internal Palette Address Source */
define(`VGA_ATTRC_IND', `0x1f') /* Index */

/* TODO: maybe this needs to be changed to something like _VGA_MAP_BITS with
 * each bit being assigned individually. */
/* Internal Palette Register Bits: */
define(`VGA_REG_PAL_DATA', `0x3f') /* Palette Data */

/* Attribute Mode Control Register Bits: */
define(`VGA_PS', `0x80') /* P5, P4 Select */
define(`VGA_PW', `0x40') /* PEL Width */
define(`VGA_PP', `0x20') /* PEL Panning Compatibility */
define(`VGA_EB', `0x8') /* Enable Blink/-Select Background Intensity */
define(`VGA_ELG', `0x4') /* Enable Line Graphics Character Code */
define(`VGA_ME', `0x2') /* Mono Emulation */
define(`VGA_G', `0x1') /* Graphics/-Alphanumeric Mode */

/* Color Plane Enable Register Bits: */
define(`VGA_ECP', `0xf') /* Enable Color Plane */

/* Horizontal PEL Panning Register Bits: */
define(`VGA_HPP', `0xf') /* Horizontal PEL Panning */

/* Color Select Register Bits: */
define(`_VGA_SCN', `dnl
pushdef(`arg', `$1')dnl
ifelse(eval(`('arg`) < 4 || ('arg`) > 7'), `1', `errprint(`Color Select out 'dnl
`of bounds (must be 4 <= index <= 7, is 'arg`).')', dnl
`(0x1 << ('arg`))dnl
popdef(`arg')dnl
ifelse(`$#', `1', , ` | _VGA_SCN(shift($@))')dnl
')')
define(`VGA_SCN', `ifelse(`$#', `0', `0x0', `(_VGA_SCN($@))')')


/* Video Digital-to-Analog Converter Palette Registers: */

define(`VGA_PAL_ADDR_WR', `0x3c8') /* Palette Address (Write Mode) */
define(`VGA_PAL_ADDR_RD', `0x3c7') /* Palette Address (Read Mode) */
define(`VGA_DAC_STATE', `0x3c7') /* DAC State */
define(`VGA_PAL_DATA', `0x3c9') /* Palette Data */
define(`VGA_PEL_MASK', `0x3c6') /* PEL Mask */

/* TODO: add something if you find anything in the manual. there are no
 * direct register descriptions as for the preceding ones, but maybe there is
 * something written in the text. */

/* TODO: very narrow and specific. broaden that and make it more flexible! */
define(`VID_SEG', `0xb800')
define(`NUM_ROWS', `25')
define(`NUM_COLS', `80')

)
