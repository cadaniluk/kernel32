#include <mm/paging.h>
#include <cpuid.h>
#include <crn.h>
#include <stddef.h>

pde_t pg_dir[PD_LEN];

void mm_init(void)
{
	/*
	 * The first GiB of physical memory is identity-mapped into the linear
	 * address space. If PSE is supported, we use 4 MiB pages, otherwise we
	 * have to use page tables and 4 KiB pages.
	 */
	
	struct cpu_info info = cpuid(0x1);
	if (info.edx & 0x8) {
		/* Enable PSE. */
		__asm__(
			"mov %%cr4, %%eax\n\t"
			"or %0, %%eax\n\t"
			"mov %%eax, %%cr4" :: "i" (CR4_PSE) : "eax", "cc"
		);

		/* Fill page directory with entries for 4-MiB pages. */
		for (size_t i = 0; i < 256; ++i)
			pg_dir[i] = MK_PDE_TO_PG(i << PDE_PG_ADDR_SHIFT, 0, 0, 0, 1, 0, 1);
	
		/* Mark all others not present. */
		for (size_t i = 256; i < PD_LEN; ++i)
			pg_dir[i] = 0x0;
	} else {
		/*
		 * Every page table is 4 KiB large, so 256 of them will occupy 1 MiB. We
		 * can't fit all those tables into the first MiB, obviously, so we do
		 * the mapping with only one page table, mapping 4 MiB of memory,
		 * allocate 1 MiB for the page tables, and initialize them then.
		 */
		
		 /* TODO: implement this. */
		 __asm__("cli; hlt");
	}

	/* Set CR3 to point to the page directory and enable paging. */
	__asm__(
		"mov %0, %%eax\n\t"
		"mov %%eax, %%cr3\n\t"
		"mov %%cr0, %%eax\n\t"
		"or %1, %%eax\n\t"
		"mov %%eax, %%cr0" :: "p" (pg_dir), "i" (CR0_PG) : "eax", "cc"
	);
}
