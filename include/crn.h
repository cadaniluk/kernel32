ifdef(`CTRL_REGS',, `
define(`CTRL_REGS_H')

/* TODO: add documentation to all bits! */

/* CR0: */

/*
 * 0 - CPU in Real Mode
 * 1 - CPU in Protected Mode
 */
define(`CR0_PE', `0x1')

define(`CR0_MP', `0x2')

define(`CR0_EM', `0x4')

define(`CR0_TS', `0x8')

define(`CR0_ET', `0x10')

define(`CR0_NE', `0x20')

define(`CR0_WP', `0x10000')

define(`CR0_AM', `0x40000')

define(`CR0_NW', `0x20000000')

define(`CR0_CD', `0x40000000')

define(`CR0_PG', `0x80000000')


/* CR3: */

define(`CR3_PWT', `0x8')

define(`CR3_PCD', `0x10')


/* CR4: */

define(`CR4_VME', `0x1')

define(`CR4_PVI', `0x2')

define(`CR4_TSD', `0x4')

define(`CR4_DE', `0x8')

define(`CR4_PSE', `0x10')

define(`CR4_PAE', `0x20')

define(`CR4_MCE', `0x40')

define(`CR4_PGE', `0x80')

define(`CR4_PCE', `0x100')

define(`CR4_OSFXSR', `0x200')

define(`CR4_OSXMMEXCPT', `0x400')

define(`CR4_UIMP', `0x800')

define(`CR4_VMXE', `0x2000')

define(`CR4_SMXE', `0x4000')

define(`CR4_FSGSBASE', `0x10000')

define(`CR4_PCIDE', `0x20000')

define(`CR4_OSXSAVE', `0x40000')

define(`CR4_SMEP', `0x100000')

define(`CR4_SMAP', `0x200000')

define(`CR4_PKE', `0x800000')

')