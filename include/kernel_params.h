#ifndef KERNEL_PARAMS_H
#define KERNEL_PARAMS_H

#define KP_BASE 0x500
#define KP_SIZE (KERNEL_ORG - KP_BASE)

#ifdef __ASSEMBLER__

#include <addrs.h>
#include <mm_detect.h>

#define KP_E820_MAP KP_BASE

#elif defined(__C__)

static struct {
	struct e820_entry e820_map[E820_MAX_ENTRIES];
} * const kernel_params = (void *) KP_BASE;

_Static_assert(sizeof(*kernel_params) <= KP_SIZE,
"Kernel parameters too large");

#endif

#endif
