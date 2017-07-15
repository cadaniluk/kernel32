#ifndef __CPUID_H
#define __CPUID_H

#include <basetypes.h>

struct __attribute__ ((packed)) cpu_info {
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
};

static inline int cpuid_available(void)
{
	int __cpuid_available(void);

	static int available = -1;

	return (available != -1) ? available : (available = __cpuid_available());
}

struct cpu_info cpuid(unsigned int no);

#endif