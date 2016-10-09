.global apm_err_tbl
.global apm_installcheck

.section apm_data
/* Table of error messages. */
apm_err_tbl:
/* TODO: add addresses of messages here. offset or seg:off? */

/* These error messages are copied verbatim from the APM v1.2 specification. */
apm_off: .ascii "Power Management functionality disabled."
apm_if_rm_alrdy: .ascii "Real Mode interface connection already established."
apm_if_no_conn: .ascii "Interface not connected."
apm_if_pm16_alrdy: .ascii "16-bit Protected Mode interface already established."
apm_if_pm16_na: .ascii "16-bit Protected Mode interface not supported."
apm_if_pm32_alrdy: .ascii "32-bit Protected Mode interface already established."
apm_if_pm32_na: .ascii "32-bit Protected Mode interface not supported."
apm_bad_dev_id: .ascii "Unrecognized device ID."
apm_param_oor: .ascii "Parameter value out of range."
apm_if_no_eng: .ascii "Interface not engaged."
apm_func_na: .ascii "Function not supported."
apm_res_timer_off: .ascii "Resume timer disabled."
apm_enter_state_fail: .ascii "Unable to enter requested state."
apm_no_events: .ascii "No power management events pending."
apm_na: .ascii "Reserved - APM not present." /* TODO: what does the "Reserved"
                                              * mean here? */
.section apm_text

