/* Sources: 3 */

/* TODO: add interfacing with keyboard! */

/* Note the difference between "keyboard controller" and "keyboard-controller":
 * While former refers to, literally, the keyboard controller, latter stands
 * for the sending/receiving done between keyboard and keyboard controller. */

/* Since we are only communicating with the keyboard controller, we do not
 * need to know anything about the communication going on between keyboard
 * and keyboard controller.
 * The Intel 8042 exchanges data with the system through three internal
 * registers: status, input, and output registers.
 * NOTE: unless explicitly mentioned, all registers, buffers, etc. refer to the
 * Intel 8042 controller.
 * The responsible part of the system for keyboard interaction can be
 * visualized as such:
 *
 *                               Keyboard
 *                                  ^
 *                                  |
 *                                  v
 *                          Keyboard Controller
 *           (input buffer)  (status register) (output buffer)
 *                 ^                |                 |
 *                 |                v                 v
 *       0x60 write-0x64 write  0x64 read         0x60 read
 * ---------------------------------------------------------------
 *                              System
 *
 * "System" simply refers to to the system except the keyboard and the keyboard
 * controller.
 * The input buffer stores commands and data sent from the system to the
 * keyboard controller.
 * The output buffer is used to deliver data to the system as requested
 * by a command or in shape of a scancode.
 * The status register contains some status information about the controller.
 */

/* Reading from one of the buffers empties it simultaneously. In conclusion,
 * the bit indicating that the input buffer is full implies that the keyboard
 * controller has not read the buffer yet. Furthermore, the bit indicating
 * that the output buffer is full implies that the system has not read
 * the buffer yet. */

ifdef(`i8042_H',,
define(`i8042_H')

/* ###############################################
 * # I/O PORTS OCCUPIED BY INTEL 8042 REGISTERS: #
 * ############################################### */

/* All the access permissions (read-only, write-only, and read and write)
 * describe the perspective of the CPU; a read-only register is thus read-only
 * to the CPU, i.e., us. */   

/* Status register; read-only. */
define(`i8042_STATUS', `0x64')

/* Command register; write-only */
define(`i8042_CMD', `0x64')

/* Data register: read and write. */ 
define(`i8042_DATA', `0x60')


/* #########################
 * # STATUS REGISTER BITS: #
 * ######################### */


/* 0 - Output buffer is empty.
 * 1 - Output buffer is full. */
define(`i8042_STAT_OUTFULL' `0x1')

/* 0 - Input buffer is empty.
 * 1 - Input buffer is full. */
define(`i8042_STAT_INEMPTY', `0x2')

/* 0 - System flag is 0.
 * 1 - System flag is 1. */
define(`i8042_STAT_SYSFLAG', `0x4')
/* Best description ever. But the manual didn't contain more to it I could
 * find, sorry. */

/* 0 - Data register was written to.
 * 1 - Command register was written to. */
define(`i8042_INPUT', 0x8')

/* TODO: add description! */
define(`i8042_STAT_INHIBIT', `0x10')

/* 0 - Transmission completed successfully.
 * 1 - Transmission by keyboard controller timed out. */
define(`i8042_SEND_TIMEOUT', `0x20')

/* 0 - Receiving completed successfully.
 * 1 - Receiving by keyboard controller timed out. */
define(`i8042_RECV_TIMEOUT', `0x40')

/* 0 - Odd parity of keyboard-controller message (success).
 * 1 - Even parity of keyboard-controller message (error). */
define(`i8042_PARITY', `0x80')


/* ###################################################
 * # VALID COMMANDS AND CORRESPONDING OUTPUT VALUES: #
 * ################################################### */


/* Send current command byte to output buffer. */
define(`i8042_RD_CMD', `0x20')

/* Use next byte written to data register as command byte. */
define(`i8042_WR_CMD', `0x60')


/* ######################
 * # COMMAND BYTE BITS: #
 * ###################### */

/* All bits not documented by any descriptions are reserved and should be left
 * clear. */

/* 0 - Do not generate an interrupt when output buffer is full.
 * 1 - Generate interrupt when output buffer is full. */
define(`i8042_INT', `0x1')

/* 0 - Write 0 to aforementioned system flag bit of status register.
 * 1 - Write 1 to aforementioned system flag bit of status register. */
define(`i8042_CMD_SYSFLAG', `0x4')

/* 0 - Enable keyboard inhibit function.
 * 1 - Disable keyboard inhibit function. */
define(`i8042_CMD_INHIBIT', `0x8')

/* 0 - Enable the keyboard.
 * 1 - Disable the keyboard. */
define(`i8042_CMD_DISABLE', `0x10')

/* 0 - Disable IBM PC mode.
 * 1 - Enable IBM PC mode. */
define(`i8042_PCMODE', `0x20')
/* IBM PC mode denotes the IBM PC keyboard interface. In that mode, no
 * parity-checking is performed and no scan codes are converted. */

/* 0 - Disable IBM PC compatibility mode.
 * 1 - Enable IBM PC compatibility mode. */
define(`i8042_PC_COMPAT', `0x40')
/* IBM PC compatibility mode enables conversion from the scancodes received
 * from the keyboard to the former IBM PC scancodes. */


/* Performs internal self-tests. */
define(`i8042_SELFTEST', `0xaa')

/* Content of output buffer if self-test (command `0xaa') did not find any
 * errors. */
define(`i8042_SELFT_NOERR', `0x55')


/* Interface test, which checks the keyboard clock and data lines. */
define(`i8042_IFTEST', `0xab')

/* Content of output buffer if no error. */
define(`i8042_IFT_NOERR', `0x0')


/* Sends 16 bytes of diagnostic information in scancode format to the system
 * state of input port, state of output port, program status word. */
define(`i8042_DUMP', `0xac')

/* Disables the keyboard by setting the aforementioned bit 4 of the command
 * byte. */
define(`i8042_DISABLE', `0xad')

/* Enables the keyboard by clearing the aforementioned bit 4 of the command
 * byte. */
define(`i8042_ENABLE', `0xae')

/* Reads the input port and stores the content in the output buffer. */
define(`i8042_RD_INP', `0xc0')

/* ####################
 * # INPUT PORT BITS: #
 * #################### */

/* All bits not defined here are reserved. */


/* 0 - Disable 2nd 256kb of RAM.
 * 1 - Enable 2nd 256kb of RAM. */
define(`i8042_256KB', `0x10')

/* 0 - Manufacturing jumper not installed.
 * 1 - Manufacturing jumper installed. */
define(`i8042_JMP', `0x20')

/* 0 - Primary display has color adapter.
 * 1 - Primary display has monochrome adapter. */
define(`i8042_DISPLAY', `0x40')

/* 0 - Keyboard not inhibited.
 * 1 - Keyboard inhibited. */
define(`i8042_INP_INHIBIT', `0x80')


/* Reads the output port and stores the content in the output buffer. */
define(`i8042_RD_OUTP', `0xd0')

/* Write the next byte written to the data register to the output port. */
define(`i8042_WR_OUTP', `0xd1')

/* #####################
 * # OUTPUT PORT BITS: #
 * ##################### */

/* All bits not defined here are reserved. */


/* 0 - Reset the system.
 * 1 - System online. */
define(`i8042_SYSRESET', `0x1')

/* 0 - A20 disabled.
 * 1 - A20 enabled. */
define(`i8042_A20', `0x2')

/* 0 - Output buffer empty.
 * 1 - Output buffer full. */
define(`i8042_OUTP_OUTFULL', `0x10')

/* 0 - Input buffer full.
 * 1 - Input buffer empty. */
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
define(`i8042_RD_TEST', `0xe0')

/* ########################
 * # TEST INPUT MEANINGS: #
 * ######################## */

/* `T0' test input; state of clock line (input).
 * TODO: find out what that "(input)" means and adjustdefine(` name and
 * description then! */
define(`i8042_CLOCK_IN', `0x0')

/* `T1' test input; state of data line (input).
 * TODO: find out what that "(input)" means and adjustdefine(` name and
 * description then! */
define(`i8042_DATA_IN', `0x1')


/* Clears bits 0 - 3 of the output port for roughly 6 microsecs by indication
 * in the four least significant bits of the command. */
define(`i8042_PULSE_PREFIX', 0xf0')

/* #################
 * # AFFIRM PULSE: #
 * ################# */

/* Clear bit temporarily. */
define(`i8042_PULSE_YES', `0x0')

/* Do not modify bit. */
define(`i8042_PULSE_NO', `0x1')

/* #######################################
 * # INTERFACE TEST (ABBR. IFT) RESULTS: #
 * ####################################### */

/* Keyboard clock line stuck low. */
define(`i8042_IFT_CLOW', `0x1')

/* Keyboard clock line stuck high. */
define(`i8042_IFT_CHIGH', `0x2')

/* Keyboard data line stuck low. */
define(`i8042_IFT_DLOW', `0x3')

/* Keyboard data line stuck high. */
define(`i8042_IFT_DHIGH', `0x4')


/* ########################################################################## 
 * Now, that was all the low-level stuff of the controller. What follows up
 * is the abstraction from that. It ranges from "still severely low-level" to
 * "rather high-level" and thus can be accessed from different abstraction
 * levels to interact with the controller in the most efficient way.
 * ########################################################################## */


/* Disables the Intel 8042. */
define(`i8042_disable',`
	push %ax
	mov $i8042_DISABLE, %al
	i8042_outcmd
	pop %ax
')

/* Enables the Intel 8042. */
define(`i8042_enable',`
	push %ax
	mov $i8042_ENABLE, %al
	i8042_outcmd
	pop %ax
')

/* Performs a self-test on the Intel 8042. */
define(`i8042_selftest',`
	mov $i8042_SELFTEST, %al
	i8042_outcmd
	i8042_indata
')

/* Performs an interface test on the Intel 8042. For more information,
 * see the IBM 5170 Technical Reference. */  
define(`i8042_iftest',`
	mov $i8042_IFTEST, %al
	i8042_outcmd
	i8042_indata
')

/* Disables A20.*/  
define(`i8042_disablea20',`
	i8042_rdoutp
	and ~$i8042_A20, %al
	i8042_wroutp
')

/* Enables A20. */
define(`i8042_enablea20',`
	i8042_rdoutp
	or $i8042_A20, %al
	i8042_wroutp
')

/* Resets the whole system. */
define(`i8042_sysreset',`
	/* No need for any data preservation - the system will be
	 * reset anyway. */

	mov $i8042_WR_OUTP, %al
	i8042_outcmd

	/* TODO: add apopstrophe to 1s complement again when youve found
	 * a way to not parse comments in quotes! */
	/* 1s-complement with `~' because `i8042_SYSRESET == 0' means
	 * reset, not `i8042_SYSRESET == 1' as with most other flags. */
	mov ~$i8042_SYSRESET, %al
	i8042_outdata

	/* TODO: replace with something better, look at
	 * /include/drivers/i8042.h. */
	cli
	hlt
')

/* Sets the content of the command byte. */
define(`i8042_setcmdbyte',`
	push %ax

	xchg %ah, %al
	mov $i8042_WR_CMD, %al
	i8042_outcmd

	xchg %ah, %al
	i8042_outdata

	pop %ax
')

/* Returns the command byte. */
define(`i8042_cmdbyte',`
	mov $i8042_RD_CMD, %al
	i8042_outcmd

	i8042_indata
')

/* Reads from the input port. */ 
define(`i8042_rdinp',`
	mov $i8042_RD_INP, %al
	i8042_outcmd

	i8042_indata
')

/* Reads from the output port. */
define(`i8042_rdoutp',`
	mov $i8042_RD_OUTP, %al
	i8042_outcmd

	i8042_indata
')

/* Writes to the output port. */
define(`i8042_wroutp',`
	push %ax

	xchg %ah, %al
	mov $i8042_WR_OUTP, %al
	i8042_outcmd

	xchg %ah, %al
	i8042_outdata

	pop %ax
')

define(`i8042_waitin',` 
1:
	i8042_instat
	test $i8042_STAT_INEMPTY, %al
	jnz 1f
	pause
	jmp 1b
1:
')

define(`i8042_waitout',`
1:
	i8042_instat
	test $i8042_STAT_OUTFULL, %al
	jnz 1f
	pause
	jmp 1b
1:
')

define(`i8042_waitout_int',`
	pushf
	sti
	hlt
	popf
')

) /* i8042_H */
