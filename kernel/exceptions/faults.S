/* Faults push the address of the instruction the trap was fired at. */

.global ex_divide, ex_beyond_range, ex_bad_opcode, ex_no_fpu,
.global ex_fpu_seg_overrun, ex_bad_tss, ex_seg_not_present, ex_ss_fault
.global ex_general_prot, ex_page_fault, ex_fpu_float_error, ex_align_check
.global ex_simd_float, ex_virtual

ex_divide:
	iret

ex_beyond_range:
	iret

ex_bad_opcode:
	iret

ex_no_fpu:
	iret

/* TODO: remove if we dont stay 386-compatible. 386+ dont fire this anymore. */
ex_fpu_seg_overrun:
	iret

ex_bad_tss:
	iret

ex_seg_not_present:
	iret

ex_ss_fault:
	iret

ex_general_prot:
	iret

ex_page_fault:
	iret

ex_fpu_float_error:
	iret

ex_align_check:
	iret

ex_simd_float:
	iret

ex_virtual:
	iret
