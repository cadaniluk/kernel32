#include <mm/seg.h>
#include <drivers/i8259A.h>

descr_t gdt[GDT_SIZE] = {
	0x0, /* null descriptor */
	MK_CD_DESCR(
		0x0,
		0xffffff,
		MK_SEGTYPE(SEG_CODE, R_OR_W_ON, EUP_OR_NONCONFORM),
		0,
		SEG_32,
		GRANUL_4KiB
	),
	MK_CD_DESCR(
		0x0,
		0xffffff,
		MK_SEGTYPE(SEG_DATA, R_OR_W_ON, EUP_OR_NONCONFORM),
		0,
		SEG_32,
		GRANUL_4KiB
	),
	MK_CD_DESCR(
		0x0,
		0xffffff,
		MK_SEGTYPE(SEG_CODE, R_OR_W_ON, EUP_OR_NONCONFORM),
		3,
		SEG_32,
		GRANUL_4KiB
	),
	MK_CD_DESCR(
		0x0,
		0xffffff,
		MK_SEGTYPE(SEG_DATA, R_OR_W_ON, EUP_OR_NONCONFORM),
		3,
		SEG_32,
		GRANUL_4KiB
	)
};

/*
 * We cannot initialize the IDT here because the handler addresses are
 * not constant and C expects constants in static initializations. Falling
 * back to assembly will not help either because the ELF format, in its
 * very nature, does not support bitwise operations; the effective address
 * can be computed at linking time only. Either we write something ugly that
 * modifies the kernel image after the fact (faster) or we compute the addresses
 * at runtime (slower). The latter is cleaner and we do not care about speed
 * in this stage, so we opt for it.
 */
descr_t idt[IDT_SIZE];

/*
 * Please do not judge me because I do the initialization in assembly. With C,
 * I would have to worry about types, function pointer, and declarations...
 * assembly is shorter here. And possibly even more readable.
 */
/* TODO: place in a section which is overridden by other data later on.
 * this is only needed for initialization and can be removed afterwards.
 */
extern uint32_t addrs1[], addrs2[], addrs3[], addrs4[];
__asm__(
	".data\t\n"
	"addrs1:\t\n"
	".long ex_div0, ex_debug\t\n"
	"addrs2:\t\n"
	".long ex_breakpoint, ex_overflow, ex_beyond_range, ex_bad_opcode,"
	"ex_no_fpu, ex_double_fault, ex_fpu_seg_overrun, ex_bad_tss,"
	"ex_seg_not_present, ex_ss_fault, ex_general_prot, ex_page_fault\t\n"
	"addrs3:\t\n"
	".long ex_fpu_float_error, ex_align_check, ex_machine_check,"
	"ex_simd_float, ex_virtual\t\n"
	"addrs4:\t\n"
	".long irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7, irq8, irq9, irq10,"
	"irq11, irq12, irq13, irq14, irq15"
);

#define ADDR_COUNT1 2
#define ADDR_COUNT2 12
#define ADDR_COUNT3 5
#define ADDR_COUNT4 16

/*
 * A dummy ISR for all user-defined interrupts that do not have their own
 * ISR.
 */
extern uint32_t isr_dummy_addr;
__asm__(
	".text\t\n"
	"isr_dummy: iret\t\n"
	".data\t\n"
	"isr_dummy_addr: .long isr_dummy"
);

/*
 * Storing the addresses in an array and initializing the IDT in a loop
 * should be faster and smaller on disk than initializing every single
 * descriptor with one assignment.
 */
void idt_init(void)
{
	/* TODO: replace unsigned int with size_t. */
	unsigned int i = 0, j = 0;

	for ( ; i < ADDR_COUNT1; ++i, ++j)
		idt[i] = MK_INTG_DESCR(addrs1[j], C0_SEL, GATE_32, 0);
	
	/* Skip vector 0x2 (NMI). */

	for (++i; i < ADDR_COUNT2; ++i, ++j)
		idt[i] = MK_INTG_DESCR(addrs2[j], C0_SEL, GATE_32, 0);
	
	/* Skip vector 0xe (reserved). */

	for (++i; i < ADDR_COUNT3; ++i, ++j)
		idt[i] = MK_INTG_DESCR(addrs3[j], C0_SEL, GATE_32, 0);
	
	for (i = USER_INT_START; i < ADDR_COUNT4; ++i, ++j)
		idt[i] = MK_INTG_DESCR(addrs4[j], C0_SEL, GATE_32, 0);

	/* Start at user-defined interrupts. */
	for ( ; i < IDT_SIZE; ++i)
		idt[i] = MK_INTG_DESCR(isr_dummy_addr, C0_SEL, GATE_32, 0);
}
