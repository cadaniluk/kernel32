include(`include/i8042.h')

/* TODO: no `i8042_destroy' function so far... add if necessary! */
.global i8042_init/*, i8042_destroy */
.global i8042_disable, i8042_enable
.global i8042_selftest, i8042_iftest
.global i8042_disablea20, i8042_enablea20
.global i8042_sysreset
.global i8042_setcmdbyte, i8042_cmdbyte
.global i8042_rdinp, i8042_rdoutp, i8042_wroutp
.global i8042_waitin, i8042_waitout, i8042_waitout_int

SEC_TEXT(`.bootsys_text')
i8042_init:
	/* Disables, so the initialization will not be disrupted. */
	call i8042_disable

	/* Finishs the output buffer, just to be sure. The output buffer is
	 * checked for data availability first to avoid any unexpected
	 * behavior. */
	call i8042_instat
	test $i8042_STAT_OUTFULL, %al	
	jz 1f

	/* Reads and discards. */
	call i8042_indata

1:
	/* Alters the command byte somewhat. Clear `i8042_CMD_INT',
	 * `i8042_CMD_PCMODE', and `i8042_CMD_PC_COMPAT' bits. */
	call i8042_cmdbyte
	and $~(i8042_CMD_INT | i8042_CMD_PCMODE | i8042_CMD_PC_COMPAT), %al
	call i8042_setcmdbyte

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
	mov $i8042_DISABLE, %al
	call i8042_outcmd
	pop %ax
	ret


/* Enables the Intel 8042. */
i8042_enable:
	push %ax
	mov $i8042_ENABLE, %al
	call i8042_outcmd
	pop %ax
	ret

/* Performs a self-test on the Intel 8042. */
i8042_selftest:
	mov $i8042_SELFTEST, %al
	call i8042_outcmd
	call i8042_indata
	ret

/* Performs an interface test on the Intel 8042. For more information,
 * see the IBM 5170 Technical Reference. */  
i8042_iftest:
	mov $i8042_IFTEST, %al
	call i8042_outcmd
	call i8042_indata
	ret

/* Disables A20.*/  
i8042_disablea20:
	call i8042_rdoutp
	and $~i8042_A20, %al
	call i8042_wroutp
	ret

/* Enables A20. */
i8042_enablea20:
	call i8042_rdoutp
	or $i8042_A20, %al
	call i8042_wroutp
	ret

/* Resets the whole system. */
i8042_sysreset:
	/* No need for any data preservation - the system will be
	 * reset anyway. */

	mov $i8042_WR_OUTP, %al
	call i8042_outcmd

	/* TODO: add apopstrophe to 1s complement again when youve found
	 * a way to not parse comments in quotes! */
	/* 1s-complement with `~' because `i8042_SYSRESET == 0' means
	 * reset, not `i8042_SYSRESET == 1' as with most other flags. */
	mov $~i8042_SYSRESET, %al
	call i8042_outdata

	/* TODO: replace with something better, look at
	 * /include/i8042.h. */
	cli
	hlt
	/* No `ret' needed. */

/* Sets the content of the command byte. */
i8042_setcmdbyte:
	push %ax

	xchg %ah, %al
	mov $i8042_WR_CMD, %al
	call i8042_outcmd

	xchg %ah, %al
	call i8042_outdata

	pop %ax
	ret

/* Returns the command byte. */
i8042_cmdbyte:
	mov $i8042_RD_CMD, %al
	call i8042_outcmd

	call i8042_indata
	ret


/* Reads from the input port. */ 
i8042_rdinp:
	mov $i8042_RD_INP, %al
	call i8042_outcmd
	call i8042_indata
	ret

/* Reads from the output port. */
i8042_rdoutp:
	mov $i8042_RD_OUTP, %al
	call i8042_outcmd
	call i8042_indata
	ret

/* Writes to the output port. */
i8042_wroutp:
	push %ax

	xchg %ah, %al
	mov $i8042_WR_OUTP, %al
	call i8042_outcmd

	xchg %ah, %al
	call i8042_outdata

	pop %ax
	ret

i8042_waitin: 
1:
	call i8042_instat
	test $i8042_STAT_INEMPTY, %al
	jnz 1f
	pause
	jmp 1b
1:
	ret
  
i8042_waitout:
1:
	call i8042_instat
	test $i8042_STAT_OUTFULL, %al
	jnz 1f
	pause
	jmp 1b
1:
	ret
  
i8042_waitout_int:
	pushf
	sti
	hlt
	popf
	ret

i8042_outdata:
	call i8042_waitin
	out %al, $i8042_DATA
	ret

i8042_outcmd:
	call i8042_waitin
	out %al, $i8042_CMD
	ret

i8042_indata:
	call i8042_waitout
	in $i8042_DATA, %al
	ret

i8042_instat:
	in $i8042_STATUS,%al
	ret
