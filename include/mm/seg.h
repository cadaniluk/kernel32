#ifndef DESCR_H
#define DESCR_H

#define GDT_SIZE 0x10
#define IDT_SIZE 0x100

#define DESCR_SIZE 0x8

#define C0_SEL 0x8
#define D0_SEL 0x10
#define C3_SEL 0x18
#define D3_SEL 0x20

#ifdef __C__

#include <stdint.h>

typedef uint64_t descr_t;

extern descr_t gdt[] __attribute__((aligned(8)));
extern descr_t idt[] __attribute__((aligned(8)));

/*
 * Initialize the IDT at runtime, not at compile time as is the case with the
 * GDT because of reasons uttered in kernel/descr.c.
 */
/* TODO: maybe make static and move to an implementation .c file because its
 * only called once. */
void init_idt(void);

/*
 * Generates a code or data descriptor to be inserted into GDT or LDT.
 * `$1' - 32-bit segment base address
 * `$2' - 20-bit segment limit (meaning depends on granularity flag)
 * `$3' - Segment type
 * `$4' - DPL (Descriptor Privilege Level)
 * `$5' - D/B flag (meaning depends on segment type)
 * `$6' - Granularity flag
 */
#define MK_CD_DESCR(base, limit, type, dpl, db, gran) ( \
	((descr_t) ((base) & 0xff000000) << 32) | \
	((descr_t) (gran) << 55) | \
	((descr_t) (db) << 54) | \
	((descr_t) ((limit) & 0xf0000) << 32) | \
	((descr_t) 0x1 << 47) | \
	((descr_t) (dpl) << 45) | \
	((descr_t) 0x1 << 44) | \
	((descr_t) (type) << 40) | \
	((descr_t) ((base) & 0xffffff) << 16) | \
	((descr_t) (limit) & 0xffff) \
)


/*
 * Segment type:
 * Creates the segment-type bit field.
 * `$1' - Data (0) or code (1)
 * `$2' - Write-enable (data)/read-enable (code)
 * `$3' - Expansion-direction (data)/conforming (code)
 */
#define MK_SEGTYPE(type, enable, exp_dir) \
	((enable) << 1 | (exp_dir) << 2 | (type) << 3)

#define SEG_DATA 0
#define SEG_CODE 1

#define R_OR_W_OFF 0
#define R_OR_W_ON 1

#define EUP_OR_NONCONFORM 0
#define EDOWN_OR_CONFORM 1

/*
 * D/B flag:
 * Default length/big flag.
 * Code (D): 16-bit (0) or 32-bit (1)
 * Data segment in SS (B): Stack pointer is 16-bit (0) or 32-bit (1); if
 * expand-down, it also indicates the upper bound as below
 * Expand-down data (B) : Upper bound is 64 KiB (0) or 4 GiB (1)
 */
#define SEG_16 0
#define SEG_32 1


/* Granularity: */
#define GRANUL_BYTE 0
#define GRANUL_4KiB 1

/*
 * `$1' - TSS segment selector
 * `$2' - DPL
 */
#define MK_TASKG_DESCR(sel, dpl) ( \
	((descr_t) 0x1 << 47) | \
	((descr_t) (dpl) << 45) | \
	((descr_t) 0x5 << 40) | \
	((descr_t) (sel) << 16) \
)


/*
 * `$1' - Offset to ISR
 * `$2' - Segment selector
 * `$3' - 16-bit (0) or 32-bit (1) gate
 * `$4' - DPL
 */
#define MK_INTG_DESCR(off, sel, size, dpl) ( \
	((descr_t) ((off) & 0xffff0000) << 48) | \
	((descr_t) 0x1 << 47) | \
	((descr_t) (dpl) << 45) | \
	((descr_t) (size) << 43) | \
	((descr_t) 0x3 << 41) | \
	((descr_t) (sel) << 16) | \
	((descr_t) ((off) & 0xffff)) \
)


/*
 * `$1' - Offset to trap handler
 * `$2' - Segment selector
 * `$3' - 16-bit (0) or 32-bit (1) gate
 * `$4' - DPL
 */
#define MK_TRAPG_DESCR(off, sel, size, dpl) \
	((descr_t) ((off) & 0xffff0000) << 48) | \
	((descr_t) 0x1 << 47) | \
	((descr_t) (dpl) << 45) | \
	((descr_t) (size) << 43) | \
	((descr_t) 0x3 << 41) | \
	((descr_t) (sel) << 16) | \
	((descr_t) ((off) & 0xffff)) \
)


#define GATE_16 0
#define GATE_32 1

/*
 * Generates a TSS descriptor.
 * `$1' - 32-bit segment base address
 * `$2' - 20-bit segment limit (meaning depends on granularity flag)
 * `$3' - DPL (Descriptor Privilege Level)
 * `$4' - Granularity flag
 */
#define MK_TSS_DESCR(base, limit, dpl, gran) \
	((descr_t) ((base) & 0xff000000) << 32) | \
	((descr_t) (gran) << 55) | \
	((descr_t) ((limit) & 0xf0000) << 32) | \
	((descr_t) 0x1 << 47) | \
	((descr_t) (dpl) << 45) | \
	((descr_t) 0x9 << 40) | \
	((descr_t) ((base) & 0xffffff) << 16) | \
	((descr_t) (limit) & 0xffff) \
)

void load_gdtr(const descr_t *gdt, uint16_t len);

#endif

#endif
