/*
 * Aborts cannot be recovered from and require termination of the task
 * or the whole system.
 */

.global ex_double_fault, ex_machine_check

ex_double_fault:
	hlt

ex_machine_check:
	hlt
