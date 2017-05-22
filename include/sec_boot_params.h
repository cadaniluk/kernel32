#ifndef SEC_BOOT_PARAMS_H
#define SEC_BOOT_PARAMS_H

#include <addrs.h>

#ifdef __ASSEMBLER__

#define SBP_DRV_NUM SBP_BASE
#define SBP_CURR_LINE (SBP_BASE + 0x1)

#elif defined(__C__)

static struct {
	uint8_t drv_num;
	uint8_t curr_line;
} * const sec_boot_params = (void *) SBP_BASE;
/* TODO: i hope this static pointer is optimized out... i really do not want
 * to have that for every single instance... */

_Static_assert(sizeof(*sec_boot_params) <= SBP_SIZE,
"Secondary bootloader parameters too large");

#endif

#endif
