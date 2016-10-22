include(`include/i8042.h')

.global i8042_init
/*.global i8042_destroy*/

SEC_TEXT(`.bootsys_text')
i8042_init:
	/* Disables, so the initialization will not be disrupted. */
	i8042_disable

	/* Finishs the output buffer, just to be sure. The output buffer is
	 * checked for data availability first to avoid any unexpected
	 * behavior. */
	i8042_instat
	test $i8042_OUTFULL, %al	
	jz 1f

	/* Reads and discards. */
	call i8042_indata

1:
	/* Alters the command byte somewhat. Clear `i8042_CMD_INT',
	 * `i8042_CMD_PCMODE', and `i8042_CMD_PC_COMPAT' bits. */
	i8042_cmdbyte
	and ~(i8042_CMD_INT | i8042_CMD_PCMODE | i8042_CMD_PC_COMPAT), %al
	i8042_setcmdbyte

	/* Doing the tests at the beginning of the function would be better for
	 * performance because the function could return earlier in case of
	 * failure but maybe the other initialization stuff changes the outcomes
	 * of those tests, so I added them last in the end. */  

	/* Performs internal self-tests. */
	i8042_selftest
	cmp $i8042_SELFT_NOERR, %al
	je 1f
	mov $E_i8042_SELFTEST, %al
	ret
1:
	/* Perform interface tests, that is, test clock and data lines. */

	i8042_iftest
	cmp $i8042_IFT_NOERR, %al
	je 1f
	mov $E_i8042_IFTEST, %al
	ret
1:
	mov $E_i8042_NOERR, %al
	ret

/* TODO: no `i8042_destroy' function so far... add if necessary! */
