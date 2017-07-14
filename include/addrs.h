#ifndef ADDRS_H
#define ADDRS_H

#include <boot.h>

#define BOOT_ORG 0x7c00
#define BOOT_LEN 0x200

#define SBP_BASE 0x400 /* right after the IVT, overwriting the BDA */
#define SBP_SIZE 0x200

#define BOOTSYS_LBA 0x1
#define BOOTSYS_SECTORS 0x8
#define BOOTSYS_ORG (SBP_BASE + SBP_SIZE)
#define BOOTSYS_LEN (BOOTSYS_SECTORS * SECTOR_SIZE)

#define KERNEL_LBA (BOOTSYS_LBA + BOOTSYS_SECTORS)
#define KERNEL_SECTORS 0x8
#define KERNEL_ORG 0x10000	/* 64 KiB for other stuff below */
#define KERNEL_LEN (KERNEL_SECTORS * SECTOR_SIZE)

#define KP_SIZE 0x800
#define KP_BASE (KERNEL_ORG - KP_SIZE)

#if BOOTSYS_LEN > KP_BASE - (SBP_BASE + SBP_SIZE)
#error "Boot system too large"
#endif

#if SBP_SIZE + BOOTSYS_LEN + KP_SIZE > KERNEL_ORG
#error "Data does not fit into space below kernel"
#endif


#ifdef __ASSEMBLER__

#define SBP_DRV_NUM SBP_BASE
#define SBP_CURR_LINE (SBP_BASE + 0x1)

#define KP_E820_MAP KP_BASE

#elif defined(__C__)

static struct {
	uint8_t drv_num;
	uint8_t curr_line;
} * const sec_boot_params = (void *) SBP_BASE;
/* TODO: i hope this static pointer is optimized out... i really do not want
 * to have that for every single instance... */

_Static_assert(sizeof(*sec_boot_params) <= SBP_SIZE,
"Secondary bootloader parameters too large");


static struct {
	struct e820_entry e820_map[E820_MAX_ENTRIES];
} * const kernel_params = (void *) KP_BASE;

_Static_assert(sizeof(*kernel_params) <= KP_SIZE,
"Kernel parameters too large");

#endif

#endif
