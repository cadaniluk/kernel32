include(`i8042.h')

/* TODO: no `i8042_destroy' function so far... add if necessary! */
.global i8042_init/*, i8042_destroy */
.global i8042_disable, i8042_enable
.global i8042_selftest, i8042_iftest
.global i8042_disablea20, i8042_enablea20
.global i8042_sysreset
.global i8042_setcmdbyte

SEC_TEXT(`.bootsys_text')
i8042_init:
	/* Disables, so the initialization will not be disrupted. */
	call i8042_disable

	/* Finishs the output buffer, just to be sure. The output buffer is
	 * checked for data availability first to avoid any unexpected
	 * behavior. */
	in $i8042_STATUS, %al
	test $i8042_STAT_OUTFULL, %al	
	jz 1f

	/* Reads and discards. */
	i8042_RD_DATA

1:
	/* Alters the command byte somewhat. Clear `i8042_CMD_INT',
	 * `i8042_CMD_PCMODE', and `i8042_CMD_PC_COMPAT' bits. */
	i8042_CMD_BYTE
	and $~(i8042_CMD_INT | i8042_CMD_PCMODE | i8042_CMD_PC_COMPAT), %al
	i8042_SET_CMD_BYTE

	/* Doing the tests at the beginning of the function would be better for
	 * performance because the function could return earlier in case of
	 * failure but maybe the other initialization stuff changes the outcomes
	 * of those tests, so I added them last in the end. */  

	/* Performs internal self-tests. */
	call i8042_selftest
	cmp $i8042_SELFT_NOERR, %al
	je 1f
	mov $i8042_E_SELFTEST, %al
	ret
1:
	/* Perform interface tests, that is, test clock and data lines. */

	call i8042_iftest
	cmp $i8042_IFT_NOERR, %al
	je 1f
	mov $i8042_E_IFTEST, %al
	ret
1:
	mov $i8042_E_NOERR, %al
	ret

/* Disables the Intel 8042. */
i8042_disable:
	push %ax
	mov $i8042_CMD_DISABLE, %al
	i8042_WR_CMD
	pop %ax
	ret


/* Enables the Intel 8042. */
i8042_enable:
	push %ax
	mov $i8042_CMD_ENABLE, %al
	i8042_WR_CMD
	pop %ax
	ret

/* Performs a self-test on the Intel 8042. */
i8042_selftest:
	mov $i8042_CMD_SELFTEST, %al
	i8042_WR_CMD
	i8042_RD_DATA
	ret

/* Performs an interface test on the Intel 8042. For more information,
 * see the IBM 5170 Technical Reference. */  
i8042_iftest:
	mov $i8042_CMD_IFTEST, %al
	i8042_WR_CMD
	i8042_RD_DATA
	ret

/* Disables A20.*/  
i8042_disablea20:
	i8042_RD_OUTP
	and $~i8042_A20, %al
	i8042_WR_OUTP
	ret

/* Enables A20. */
i8042_enablea20:
	i8042_RD_OUTP
	or $i8042_A20, %al
	i8042_WR_OUTP
	ret

/* Resets the whole system. */
i8042_sysreset:
	/* No need for any data preservation - the system will be
	 * reset anyway. */

	mov $i8042_CMD_WR_OUTP, %al
	i8042_WR_CMD

	/* TODO: add apopstrophe to 1s complement again when youve found
	 * a way to not parse comments in quotes! */
	/* 1s-complement with `~' because `i8042_SYSRESET == 0' means
	 * reset, not `i8042_SYSRESET == 1' as with most other flags. */
	mov $~i8042_SYSRESET, %al
	i8042_WR_DATA

	/* TODO: replace with something better, look at
	 * /include/i8042.h. */
	cli
	hlt
	/* No `ret' needed. */
