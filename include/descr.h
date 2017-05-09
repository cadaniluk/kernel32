ifdef(`DESCR_H',, `
define(`DESCR_H')

define(`GDT_SIZE', `0x10')
define(`IDT_SIZE', `0x100')

define(`DESCR_SIZE', `0x8')

define(`C0_SEL', `0x8')
define(`D0_SEL', `0x10')
define(`C3_SEL', `0x18')
define(`D3_SEL', `0x20')

ifdef(`__ASSEMBLER__',, `

include(`basetypes.h')

typedef uint64_t descr_t;

extern descr_t gdt[] __attribute__((aligned(8)));
extern descr_t idt[] __attribute__((aligned(8)));

/*
 * Initialize the IDT at runtime, not at compile time as is the case with the
 * GDT because of reasons uttered in kernel/descr.c.
 */
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
define(`MK_CD_DESCR', `dnl
((descr_t) (($1) & 0xff000000) << 32) | dnl
((descr_t) ($6) << 55) | dnl
((descr_t) ($5) << 54) | dnl
((descr_t) (($2) & 0xf0000) << 32) | dnl
((descr_t) 0x1 << 47) | dnl
((descr_t) ($4) << 45) | dnl
((descr_t) 0x1 << 44) | dnl
((descr_t) ($3) << 40) | dnl
((descr_t) (($1) & 0xffffff) << 16) | dnl
((descr_t) ($2) & 0xffff)dnl
')

/*
 * Segment type:
 * Creates the segment-type bit field.
 * `$1' - Data (0) or code (1)
 * `$2' - Write-enable (data)/read-enable (code)
 * `$3' - Expansion-direction (data)/conforming (code)
 */
define(`MK_SEGTYPE', `($2) << 1 | ($3) << 2 | ($1) << 3')

define(`SEG_DATA', `0')
define(`SEG_CODE', `1')

define(`R_OR_W_OFF', `0')
define(`R_OR_W_ON', `1')

define(`EUP_OR_NONCONFORM', `0')
define(`EDOWN_OR_CONFORM', `1')

/*
 * D/B flag:
 * Default length/big flag.
 * Code (D): 16-bit (0) or 32-bit (1)
 * Data segment in SS (B): Stack pointer is 16-bit (0) or 32-bit (1); if
 * expand-down, it also indicates the upper bound as below
 * Expand-down data (B) : Upper bound is 64 KiB (0) or 4 GiB (1)
 */
define(`SEG_16', `0')
define(`SEG_32', `1')


/* Granularity: */
define(`GRANUL_BYTE', `0')
define(`GRANUL_4KiB', `1')

/*
 * `$1' - TSS segment selector
 * `$2' - DPL
 */
define(`MK_TASKG_DESCR', `dnl
((descr_t) 0x1 << 47) | dnl
((descr_t) ($2) << 45) | dnl
((descr_t) 0x5 << 40) | dnl
((descr_t) ($1) << 16)dnl
')

/*
 * `$1' - Offset to ISR
 * `$2' - Segment selector
 * `$3' - 16-bit (0) or 32-bit (1) gate
 * `$4' - DPL
 */
define(`MK_INTG_DESCR', `
((descr_t) (($1) & 0xffff0000) << 48) | dnl
((descr_t) 0x1 << 47) | dnl
((descr_t) ($4) << 45) | dnl
((descr_t) ($3) << 43) | dnl
((descr_t) 0x3 << 41) | dnl
((descr_t) ($2) << 16) | dnl
((descr_t) (($1) & 0xffff))dnl
')

/*
 * `$1' - Offset to trap handler
 * `$2' - Segment selector
 * `$3' - 16-bit (0) or 32-bit (1) gate
 * `$4' - DPL
 */
define(`MK_TRAPG_DESCR', `
((descr_t) (($1) & 0xffff0000) << 48) | dnl
((descr_t) 0x1 << 47) | dnl
((descr_t) ($4) << 45) | dnl
((descr_t) ($3) << 43) | dnl
((descr_t) 0x3 << 41) | dnl
((descr_t) ($2) << 16) | dnl
((descr_t) (($1) & 0xffff))dnl
')

define(`GATE_16', `0')
define(`GATE_32', `1')

/*
 * Generates a TSS descriptor.
 * `$1' - 32-bit segment base address
 * `$2' - 20-bit segment limit (meaning depends on granularity flag)
 * `$3' - DPL (Descriptor Privilege Level)
 * `$4' - Granularity flag
 */
define(`MK_TSS_DESCR', `dnl
((descr_t) (($1) & 0xff000000) << 32) | dnl
((descr_t) ($4) << 55) | dnl
((descr_t) (($2) & 0xf0000) << 32) | dnl
((descr_t) 0x1 << 47) | dnl
((descr_t) ($3) << 45) | dnl
((descr_t) 0x9 << 40) | dnl
((descr_t) (($1) & 0xffffff) << 16) | dnl
((descr_t) ($2) & 0xffff)dnl
')

') /* __ASSEMBLER__ */

') /* DESCR_H */