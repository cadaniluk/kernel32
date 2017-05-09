/*
 * Traps push the address of the instruction succeding the one, which fired
 * the fault.
 */

.global ex_debug, ex_breakpoint, ex_overflow

ex_debug:
	iret

ex_breakpoint:
	iret

ex_overflow:
	iret
