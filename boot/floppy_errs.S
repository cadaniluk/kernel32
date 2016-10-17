.code16

.global disk_status_table

SEC_R_DATA(.bootsys_data)

/* TODO: if the need arises, add the other error msgs as well! */
disk_status_table:
.long msg_1, msg_2, msg_3, msg_4, msg_8, msg_9, msg_10, msg_20, msg_40, msg_80

/* Original IBM PC Errors: */

msg_1: .asciz "Bad Command\n"
msg_2: .asciz "Address Mark Not Found\n"
msg_3: .asciz "Disk Write-Protected\n"
msg_4: .asciz "Sector Not Found\n"
msg_8: .asciz "DMA Overrun\n"
msg_9: .asciz "Data Boundary Error\n" /* attempted DMA across 64K boundary or
                                      * > 0x80 sectors */
msg_10: .asciz "Uncorrectable CRC or ECC Error on Read\n"
msg_20: .asciz "Controller Failure\n"
msg_40: .asciz "Seek Failed\n"
msg_80: .asciz "Attachment Failed to Respond\n"

/* Added With IBM PC AT: */

msg_6: .asciz "Disk Changed\n"


/* Other Ones From RBIL: */

/* Commented out. remove comment markers once they are needed. */
ifelse(`

msg_5: .asciz "Reset Failed\n"
/* .asciz "Data Verified Incorrectly\n" /* TODO: only for TIPC, so find a way
				      * to differentiate both msg_s!  or just
                                      * remove it because nobody has a tipc! */
msg_7: .asciz "Drive Parameter Activity Failed\n"
msg_a: .asciz "Bad Sector Detected\n"
msg_b: .asciz "Bad Track Detected\n"
msg_c: .asciz "Unsupported Track/Media\n"
msg_d: .asciz "Bad Sector Count on Format\n"
msg_e: .asciz "Control Data Address Mark Detected\n"
msg_f: .asciz "DMA Arbitration Level out of Range\n"
msg_10: .asciz "Uncorrectable CRC or ECC Error on Read\n"
msg_11: .asciz "Data ECC Corrected\n"
msg_20: .asciz "Controller Failure\n"
msg_31: .asciz "No Media in Drive\n"
msg_32: .asciz "Incorrect Drive Type Stored in CMOS\n"
msg_aa: .asciz "Drive Not Ready\n"
msg_b0: .asciz "Volume Not Locked in Drive\n"
msg_b1: .asciz "Volume Locked in Drive\n"
msg_b2: .asciz "Volume Not Removable\n"
msg_b3: .asciz "Volume in Use\n"
msg_b4: .asciz "Lock Count Exceeded\n"
msg_b5: .asciz "Valid Eject Request Failed\n"
msg_b6: .asciz "Volume Present But Read Protected\n"
msg_bb: .asciz "Undefined Error\n"
msg_cc: .asciz "Write Fault\n"
msg_e0: .asciz "Status Register Error\n"
msg_ff: .asciz "Sense Operation Failed\n"

')
