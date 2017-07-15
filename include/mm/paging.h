#ifndef PAGING_H
#define PAGING_H

#define PD_SIZE 4096
#define PT_SIZE 4096
#define PAGE_SIZE 4096

/*
 * CR3 for 32-bit paging.
 * `addr' - physical address of 4-KiB aligned page directory
 * `pcd' - page-level cache disable
 * `pwt' - page-level write-through
 */
#define MK_CR3(addr, pcd, pwt) ( \
	(addr << 12) | \
	(pcd << 4) | \
	(pwt << 3) \
)

/*
 * 32-bit paging page directory entry (PDE) pointing to a PTE.
 * `addr' - physical address of 4-KiB aligned page table
 * `pcd' - page-level cache disable 
 * `pwt' - page-level write-through
 * `us' - (User/Supervisor) if user access is allowed
 * `rw' - if writing to the page is allowed
 */
#define MK_PDE_TO_PTE(addr, pcd, pwt, us, rw) ( \
	(addr << 12) | \
	(pcd << 4) | \
	(pwt << 3) | \
	(us << 2) | \
	(rw << 1) | \
	0x1 \
)

#define PDE_PRESENT 0x1
#define PDE_RW 0x2
#define PDE_US 0x4
#define PDE_PWT 0x8
#define PDE_PCD 0x10
#define PDE_ACC 0x20
#define PDE_PS 0x80
#define PDE_ADDR_SHIFT 12

/*
 * 32-bit paging page table entry (PTE) for a 4-KiB page.
 * `addr' - physical address of 4-KiB page frame
 * `global' - whether page is global
 * `pat' - if Page Attribute Table is supported
 * `pcd' - page-level cache disable
 * `pwt' - page-level write-through
 * `us' - (User/Supervisor) if user access is allowed
 * `rw' - if writing to the page is allowed
 */
#define MK_PTE(addr, global, pat, pcd, pwt, us, rw) ( \
	(addr << 12) \
	(global << 8) \
	(pat << 7) \
	(pcd << 4) \
	(pwt << 3) \
	(us << 2) \
	(rw << 1) \
	0x1 \
)

#define PTE_PRESENT 0x1
#define PTE_RW 0x2
#define PTE_US 0x4
#define PTE_PWT 0x8
#define PTE_PCD 0x10
#define PTE_ACC 0x20
#define PTE_PAT 0x80
#define PTE_DIRTY 0x40
#define PTE_GLOBAL 0x100
#define PTE_ADDR_SHIFT 12

#define KPG_DIR 0x100000

#ifdef __C__

#include <basetypes.h>

/*
 * We're going to start slowly: 32-bit paging, no PAE or PSE. Future patches
 * will change that, though, depending on the exact processor in use.
 */

/* A page directory entry for plain 32-bit paging. */
typedef uint32_t pde_t;

/* A page table entry for plain 32-bit paging. */
typedef uint32_t pte_t;

/* TODO: mark this, along with certain other functions, as only needed for
 * initialization, so it can be overwritten later.
 */
void setup_paging(void);

#endif

#endif