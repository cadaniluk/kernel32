.global disk_status_table

SEC_R_DATA(`.bootsys_data')

/* TODO: if the need arises, add the other error msgs as well! */
disk_status_table:
.long msg_1, msg_2, msg_3, msg_4, msg_8, msg_9, msg_10, msg_20, msg_40, msg_80

/* Original IBM PC Errors: */

msg_1: .asciz "Bad Command"
msg_2: .asciz "Address Mark Not Found"
msg_3: .asciz "Disk Write-Protected"
msg_4: .asciz "Sector Not Found"
msg_8: .asciz "DMA Overrun"
msg_9: .asciz "Data Boundary Error" /* attempted DMA across 64K boundary or
                                      * > 0x80 sectors */
msg_10: .asciz "Uncorrectable CRC or ECC Error on Read"
msg_20: .asciz "Controller Failure"
msg_40: .asciz "Seek Failed"
msg_80: .asciz "Attachment Failed to Respond"

/* Added With IBM PC AT: */

msg_6: .asciz "Disk Changed"


/* Other Ones From RBIL: */

/* Commented out. remove comment markers once they are needed. */
ifelse(`

msg_5: .asciz "Reset Failed"
/* .asciz "Data Verified Incorrectly" /* TODO: only for TIPC, so find a way
				       * to differentiate both msg_s!  or just
                                       * remove it because nobody has a tipc! */
msg_7: .asciz "Drive Parameter Activity Failed"
msg_a: .asciz "Bad Sector Detected"
msg_b: .asciz "Bad Track Detected"
msg_c: .asciz "Unsupported Track/Media"
msg_d: .asciz "Bad Sector Count on Format"
msg_e: .asciz "Control Data Address Mark Detected"
msg_f: .asciz "DMA Arbitration Level out of Range"
msg_10: .asciz "Uncorrectable CRC or ECC Error on Read"
msg_11: .asciz "Data ECC Corrected"
msg_20: .asciz "Controller Failure"
msg_31: .asciz "No Media in Drive"
msg_32: .asciz "Incorrect Drive Type Stored in CMOS"
msg_aa: .asciz "Drive Not Ready"
msg_b0: .asciz "Volume Not Locked in Drive"
msg_b1: .asciz "Volume Locked in Drive"
msg_b2: .asciz "Volume Not Removable"
msg_b3: .asciz "Volume in Use"
msg_b4: .asciz "Lock Count Exceeded"
msg_b5: .asciz "Valid Eject Request Failed"
msg_b6: .asciz "Volume Present But Read Protected"
msg_bb: .asciz "Undefined Error"
msg_cc: .asciz "Write Fault"
msg_e0: .asciz "Status Register Error"
msg_ff: .asciz "Sense Operation Failed"

')
