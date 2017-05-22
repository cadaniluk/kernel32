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

#endif
