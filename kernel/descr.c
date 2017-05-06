include(`descr.h')

descr_t gdt[GDT_SIZE] = {
	0x0, /* null descriptor */
	MK_DESCR(
		0x0,
		0xffffff,
		CD_DESCR,
		MK_SEGTYPE(SEG_CODE, R_OR_W_ON, EUP_OR_NONCONFORM),
		SEG_PRESENT,
		0,
		SEG_32,
		GRANUL_4KiB
	),
	MK_DESCR(
		0x0,
		0xffffff,
		CD_DESCR,
		MK_SEGTYPE(SEG_DATA, R_OR_W_ON, EUP_OR_NONCONFORM),
		SEG_PRESENT,
		0,
		SEG_32,
		GRANUL_4KiB
	),
	MK_DESCR(
		0x0,
		0xffffff,
		CD_DESCR,
		MK_SEGTYPE(SEG_CODE, R_OR_W_ON, EUP_OR_NONCONFORM),
		SEG_PRESENT,
		3,
		SEG_32,
		GRANUL_4KiB
	),
	MK_DESCR(
		0x0,
		0xffffff,
		CD_DESCR,
		MK_SEGTYPE(SEG_DATA, R_OR_W_ON, EUP_OR_NONCONFORM),
		SEG_PRESENT,
		3,
		SEG_32,
		GRANUL_4KiB
	)
};