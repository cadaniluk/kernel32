ifdef(`i8042_H',,
define(`i8042_H')

/*
 * ###############################################
 * # I/O PORTS OCCUPIED BY INTEL 8042 REGISTERS: #
 * ###############################################
 */

/*
 * All the access permissions (read-only, write-only, and read and write)
 * describe the perspective of the CPU; a read-only register is thus read-only
 * to the CPU, i.e., us.
 */

/* Status register; read-only. */
define(`i8042_STATUS', `0x64')

/* Command register; write-only */
define(`i8042_CMD', `0x64')

/* Data register: read and write. */ 
define(`i8042_DATA', `0x60')


/*
 * #########################
 * # STATUS REGISTER BITS: #
 * #########################
 */


/*
 * 0 - Output buffer is empty.
 * 1 - Output buffer is full.
 */
define(`i8042_STAT_OUTFULL', `0x1')

/*
 * 0 - Input buffer is empty.
 * 1 - Input buffer is full.
 */
define(`i8042_STAT_INEMPTY', `0x2')

/*
 * 0 - System flag is 0.
 * 1 - System flag is 1.
 */
define(`i8042_STAT_SYSFLAG', `0x4')
/*
 * Best description ever. But the manual didn't contain more to it I could
 * find, sorry.
 */

/*
 * 0 - Data register was written to.
 * 1 - Command register was written to.
 */
define(`i8042_INPUT', 0x8')

/* TODO: add description! */
define(`i8042_STAT_INHIBIT', `0x10')

/*
 * 0 - Transmission completed successfully.
 * 1 - Transmission by keyboard controller timed out.
 */
define(`i8042_SEND_TIMEOUT', `0x20')

/*
 * 0 - Receiving completed successfully.
 * 1 - Receiving by keyboard controller timed out.
 */
define(`i8042_RECV_TIMEOUT', `0x40')

/*
 * 0 - Odd parity of keyboard-controller message (success).
 * 1 - Even parity of keyboard-controller message (error).
 */
define(`i8042_PARITY', `0x80')


/*
 * ###################################################
 * # VALID COMMANDS AND CORRESPONDING OUTPUT VALUES: #
 * ###################################################
 */


/* Send current command byte to output buffer. */
define(`i8042_CMD_RD_CMD', `0x20')

/* Use next byte written to data register as command byte. */
define(`i8042_CMD_WR_CMD', `0x60')


/*
 * ######################
 * # COMMAND BYTE BITS: #
 * ######################
 */

/*
 * All bits not documented by any descriptions are reserved and should be left
 * clear.
 */

/*
 * 0 - Do not generate an interrupt when output buffer is full.
 * 1 - Generate interrupt when output buffer is full.
 */
define(`i8042_CMD_INT', `0x1')

/*
 * 0 - Write 0 to aforementioned system flag bit of status register.
 * 1 - Write 1 to aforementioned system flag bit of status register.
 */
define(`i8042_CMD_SYSFLAG', `0x4')

/*
 * 0 - Enable keyboard inhibit function.
 * 1 - Disable keyboard inhibit function.
 */
define(`i8042_CMD_INHIBIT', `0x8')

/*
 * 0 - Enable the keyboard.
 * 1 - Disable the keyboard.
 */
define(`i8042_CMD_DISABLE', `0x10')

/*
 * 0 - Disable IBM PC mode.
 * 1 - Enable IBM PC mode.
 */
define(`i8042_CMD_PCMODE', `0x20')
/*
 * IBM PC mode denotes the IBM PC keyboard interface. In that mode, no
 * parity-checking is performed and no scan codes are converted.
 */

/*
 * 0 - Disable IBM PC compatibility mode.
 * 1 - Enable IBM PC compatibility mode.
 */
define(`i8042_CMD_PC_COMPAT', `0x40')
/*
 * IBM PC compatibility mode enables conversion from the scancodes received
 * from the keyboard to the former IBM PC scancodes.
 */


/* Performs internal self-tests. */
define(`i8042_CMD_SELFTEST', `0xaa')

/*
 * Content of output buffer if self-test (command `0xaa') did not find any
 * errors.
 */
define(`i8042_SELFT_NOERR', `0x55')


/* Interface test, which checks the keyboard clock and data lines. */
define(`i8042_CMD_IFTEST', `0xab')

/* Content of output buffer if no error. */
define(`i8042_IFT_NOERR', `0x0')


/*
 * Sends 16 bytes of diagnostic information in scancode format to the system
 * state of input port, state of output port, program status word.
 */
define(`i8042_CMD_DUMP', `0xac')

/*
 * Disables the keyboard by setting the aforementioned bit 4 of the command
 * byte.
 */
define(`i8042_CMD_DISABLE', `0xad')

/*
 * Enables the keyboard by clearing the aforementioned bit 4 of the command
 * byte.
 */
define(`i8042_CMD_ENABLE', `0xae')

/* Reads the input port and stores the content in the output buffer. */
define(`i8042_CMD_RD_INP', `0xc0')

/*
 * ####################
 * # INPUT PORT BITS: #
 * ####################
 */

/* All bits not defined here are reserved. */


/*
 * 0 - Disable 2nd 256kb of RAM.
 * 1 - Enable 2nd 256kb of RAM.
 */
define(`i8042_256KB', `0x10')

/*
 * 0 - Manufacturing jumper not installed.
 * 1 - Manufacturing jumper installed.
 */
define(`i8042_JMP', `0x20')

/*
 * 0 - Primary display has color adapter.
 * 1 - Primary display has monochrome adapter.
 */
define(`i8042_DISPLAY', `0x40')

/*
 * 0 - Keyboard not inhibited.
 * 1 - Keyboard inhibited.
 */
define(`i8042_INP_INHIBIT', `0x80')


/*
 * Reads the output port and stores the content in the output buffer.
 */
define(`i8042_CMD_RD_OUTP', `0xd0')

/*
 * Write the next byte written to the data register to the output port.
 */
define(`i8042_CMD_WR_OUTP', `0xd1')

/*
 * #####################
 * # OUTPUT PORT BITS: #
 * #####################
 */

/* All bits not defined here are reserved. */

/* TODO: whats the difference between i8042_OUTP_INEMPTY and
 * i8042_STAT_INEMPTY?
 */

/*
 * 0 - Reset the system.
 * 1 - System online.
 */
define(`i8042_SYSRESET', `0x1')

/*
 * 0 - A20 disabled.
 * 1 - A20 enabled.
 */
define(`i8042_A20', `0x2')

/*
 * 0 - Output buffer empty.
 * 1 - Output buffer full.
 */
define(`i8042_OUTP_OUTFULL', `0x10')

/*
 * 0 - Input buffer full.
 * 1 - Input buffer empty.
 */
define(`i8042_OUTP_INEMPTY', `0x20')

/* Clock line state (output).
 * TODO: find out what that "(output)" means and adjustdefine(` name and
 * description then! */
define(`i8042_CLOCK_OUT', `0x40')

/* Data line state (output).
 * TODO: find out what that "(output)" means and adjustdefine(` name and
 * description then! */
define(`i8042_DATA_OUT', `0x80')

/* ########################################################################## */

/* Read test inputs `T0' and `T1' into output buffer. */
define(`i8042_CMD_RD_TEST', `0xe0')

/*
 * ########################
 * # TEST INPUT MEANINGS: #
 * ########################
 */

/* `T0' test input; state of clock line (input).
 * TODO: find out what that "(input)" means and adjustdefine(` name and
 * description then! */
define(`i8042_CLOCK_IN', `0x0')

/* `T1' test input; state of data line (input).
 * TODO: find out what that "(input)" means and adjustdefine(` name and
 * description then! */
define(`i8042_DATA_IN', `0x1')


/*
 * Clears bits 0 - 3 of the output port for roughly 6 microsecs by indication
 * in the four least significant bits of the command.
 */
define(`i8042_PULSE_PREFIX', 0xf0')

/*
 * #################
 * # AFFIRM PULSE: #
 * #################
 */

/* Clear bit temporarily. */
define(`i8042_PULSE_YES', `0x0')

/* Do not modify bit. */
define(`i8042_PULSE_NO', `0x1')

/*
 * #######################################
 * # INTERFACE TEST (ABBR. IFT) RESULTS: #
 * #######################################
 */

/* Keyboard clock line stuck low. */
define(`i8042_IFT_CLOW', `0x1')

/* Keyboard clock line stuck high. */
define(`i8042_IFT_CHIGH', `0x2')

/* Keyboard data line stuck low. */
define(`i8042_IFT_DLOW', `0x3')

/* Keyboard data line stuck high. */
define(`i8042_IFT_DHIGH', `0x4')

/* Error codes for `i8042_init'. */
define(`i8042_E_NOERR', `0x0')
define(`i8042_E_IFTEST', `0x1')
define(`i8042_E_SELFTEST', `0x2')

/* Sets the content of the command byte. */
define(`i8042_SET_CMD_BYTE', `dnl
	push %ax

	mov %al, %ah
	mov $i8042_CMD_WR_CMD, %al
	i8042_WR_CMD

	xchg %ah, %al
	i8042_WR_DATA

	pop %ax
')

/* Returns the command byte. */
define(`i8042_CMD_BYTE', `dnl
	mov $i8042_CMD_RD_CMD, %al
	i8042_WR_CMD
	i8042_RD_DATA
')


/* Reads from the input port. */ 
define(`i8042_RD_INP', `dnl
	mov $i8042_CMD_RD_INP, %al
	i8042_WR_CMD
	i8042_RD_DATA
')

/* Reads from the output port. */
define(`i8042_RD_OUTP', `dnl
	mov $i8042_CMD_RD_OUTP, %al
	i8042_WR_CMD
	i8042_RD_DATA
')

/* Writes to the output port. */
define(`i8042_WR_OUTP', `dnl
	push %ax

	mov %al, %ah
	mov $i8042_CMD_WR_OUTP, %al
	i8042_WR_CMD

	xchg %ah, %al
	i8042_WR_DATA

	pop %ax
')

/* Wait until the input buffer is empty. */
define(`i8042_WIN_EMPTY', `dnl
	push %ax
1:
	in $i8042_STATUS, %al
	test $i8042_STAT_INEMPTY, %al
	jnz 1f
	pause
	jmp 1b
1:
	pop %ax
')

/* Wait until the output buffer is full. */
define(`i8042_WOUT_FULL', `dnl
	push %ax
1:
	in $i8042_STATUS, %al
	test $i8042_STAT_OUTFULL, %al
	jnz 1f
	pause
	jmp 1b
1:
	pop %ax
')
 
/* Write to the data register. */
define(`i8042_WR_DATA', `dnl
	i8042_WIN_EMPTY
	out %al, $i8042_DATA
')

/* Write to the command register. */
define(`i8042_WR_CMD', `dnl
	i8042_WIN_EMPTY
	out %al, $i8042_CMD
')

/* Read from the data register. */
define(`i8042_RD_DATA', `dnl
	i8042_WOUT_FULL
	in $i8042_DATA, %al
')

/*
 * TODO: find a way to have temporary labels without interfering with
 * code outside of a macro. the code outside doesnt see the label in the macro.
 * if code outside does like `jmp 1f' and a macro in between happens to
 * contain a label `1:', the jump is performed to that label in the macro.
 */

) /* i8042_H */
