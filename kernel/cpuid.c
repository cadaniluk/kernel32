#include <cpuid.h>
#include <flags.h>

int __cpuid_available()
{
	int result;

	__asm__(
		"pushf\n\t"
		"mov (%%esp), %%eax\n\t"
		"xorl %1, (%%esp)\n\t"
		"popf\n\t"
		"pushf\n\t"
		"cmp %%eax, (%%esp)\n\t"
		"setne %%al\n\t"
		"movzbl %%al, %0\n\t"
		"add $0x4, %%esp"
		: "=r" (result) : "i" (FLAGS_ID) : "eax", "esp", "memory", "cc"
	);

	return result;
}

struct cpu_info cpuid(unsigned int no)
{
	struct cpu_info ret;

	__asm__(
		"mov %4, %%eax\n\t"
		"cpuid\n\t"
		"mov %%eax, %0\n\t"
		"mov %%ebx, %1\n\t"
		"mov %%ecx, %2\n\t"
		"mov %%edx, %3"
		: "=m" (ret.eax), "=m" (ret.ebx), "=m" (ret.ecx), "=m" (ret.edx)
		: "m" ((uint32_t) no)
		: "eax", "ebx", "ecx", "edx", "memory"
	);

	return ret;
}