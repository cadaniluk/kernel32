/* Sources: 3, 15 */

/* TODO: make the documentation comments look beautiful! add
 * periods, commas, and whatever necessary to make them
 * a tad bit prettier. THIS GOES FOR EVERY FILE IN THIS PROJECT.
 * MAKE ALL COMMENTS LOOK PRETTIER. */

/* This file contains CMOS definitions for the IBM PC AT in its original
 * version. More modern CMOS definitions are not included here. Add them
 * manually with a NEW file (DO NOT edit this one) or leave it. I am not
 * even sure the original IBM PC AT CMOS definitions are supported on
 * every x86 machine. */

#ifndef CMOS_H
#define CMOS_H

#include <types.h>
#include <io.h>

/* Used to select the address of a CMOS memory location. Write the offset of
 * the location to this register to select one. */ 
#define CMOS_SELADDR 0x70

/* Used to read or write from or to CMOS memory. After a memory
 * location has been selected using I/O port `CMOS_REGSEL',
 * this port can be used to access the data at that location. */
#define CMOS_DATA 0x71

/* #############################
 * # RTC CMOS RAM INFORMATION: #
 * ############################# */

/* Interesting fact: the IBM 5170 Technical Reference  
 * describes address `0x7' as "Date of month" instead of "Day of month";
 * I never thought such a manual could make such a mistake... :O 
#define CMOS_RTC_SECS 0x0
#define CMOS_RTC_SECS_ALRM 0x1
#define CMOS_RTC_MINS 0x2
#define CMOS_RTC_MINS_ALRM 0x3
#define CMOS_RTC_HRS 0x4
#define CMOS_RTC_HRS_ALRM 0x5
#define CMOS_RTC_WEEKDAY 0x6
#define CMOS_RTC_DAY 0x7
#define CMOS_RTC_MONTH 0x8
#define CMOS_RTC_YEAR 0x9

/* I did not deem an RTC status register buffer necessary because it is
 * not needed that often and the overhead is a bit too much if it lacks any
 * significant use cases. */  

/* RTC status register A. */
#define CMOS_RTC_REGA 0xa

/* 0 - Date and time available to read.
 * 1 - Date and time are updated. */
#define CMOS_RTC_UIP 0x80

/* TODO: find out what this does and add appropriate documentation!
 * ATM, I have no idea what this is about. something with
 * 22-stage divider. -.- */
#define CMOS_RTC_ 0x70

/* TODO: find out what this does and add appropriate documentation!  
 * ATM, I have no idea what this is about. something with
 * rate selection. -.- */ 
#define CMOS_RTC_RATESEL 0xf


/* RTC status register B. */
#define CMOS_RTC_REGB 0xb

/* 0 - Updates RTC once per second as normal. Disables the "update 
 * mode" enabled by setting this bit.
 * 1 - Stops updating, so that the RTC can be written to by the CPU
 * (us). */ 
#define CMOS_RTC_SET 0x80

/* TODO: find out what this does and add appropriate documentation!
 * ATM, I have no idea what this is about. -.- */
/* 0 - (default)
 * 1 - */
#define CMOS_RTC_PIE 0x40

/* 0 - Disables alarm interrupt (default).
 * 1 - Enables alarm interrupt.
#define CMOS_RTC_AIE 0x20

/* 0 - Disables update-ended interrupt (default).
 * 1 - Enables update-ended interrupt. 
#define CMOS_RTC_UIE 0x10

/* 0 - (default) 
 * 1 - TODO: add documentation! */
#define CMOS_RTC_SQWE 0x8

/* 0 - BCD format for date and time (default). 
 * 1 - Binary format for date and time.
#define CMOS_RTC_DM 0x4

/* 0 - 12-hour mode (as with a.m. and p.m.).
 * 1 - 24-hour mode (default).
#define CMOS_RTC_HRMODE 0x2

/* 0 - Disables daylight savings (default).
 * 1 - Enables daylight savings.
#define CMOS_RTC_DSE 0x1


/* RTC status register C. */
#define CMOS_RTC_REGC 0xc

/* TODO: add documentation for these flags! the technical reference
 * describes them only superficially. */
/* 0 - 
 * 1 - */
#define CMOS_RTC_IRQF 0x80

#define CMOS_RTC_PF 0x40

#define CMOS_RTC_AF 0x20

#define CMOS_RTC_UF 0x10

/* The rest of the flags is reserved. */


/* RTC status register D. */
#define CMOS_RTC_REGD 0xd

/* 0 - RTC clock has lost power.
 * 1 - RTC clock is powered. */
#define CMOS_RTC_VRB 0x80

/* The rest of the flags is reserved. */


/* #######################################
 * # CMOS RAM CONFIGURATION INFORMATION: #
 * ####################################### */ 

/* Diagnostic status byte. */
#define CMOS_DIAG 0xe

/* 0 - RTC chip is powered.
 * 1 - RTC chip has lost power. */
#define CMOS_RTCPOW 0x80

/* 0 - Checksum good. 
 * 1 - Checksum bad.
 * The official IBM PC AT reference term is "Configuration Record" but
 * that sounded too vague to me. */ 
#define CMOS_CHKSTAT 0x40

/* 0 - Configuration information valid. Validity implies `CMOS_ONEDRV'
 * or `CMOS_TWODRVS' to be set and the primary display adapter settings
 * to match the system board's display switch settings and the actual display
 * hardware.
 * 1 - Configuration information invalid.
 * The official IBM PC AT reference term is "Incorrect Configuration
 * Information" - way too long and vague for a macro name. */
#define CMOS_BADINFO 0x20

/* 0 - Power-on check determined same memory size as configuration
 * record (TODO: replace with actual flag names!).
 * 1 - Power-on check determined different memory size than configuration
 * record (TODO: replace with actual flag names!). */ 
#define CMOS_MEM_MISCMP 0x10

/* 0 - Fixed disk adapter and Drive C function properly.
 * 1 - Fixed disk adapter and/or Drive C malfunction. 
 * The official IBM PC AT reference term is "Fixed Disk Adapter/Drive C
 * Initialization Status." I could not come up with a more descriptive
 * abbreviation. :-( */
#define CMOS_DISKINITSTAT 0x8

/* 0 - RTC time valid.
 * 1 - RTC time invalid.
 * The official IBM PC AT reference term is "Time Status Indicator"; it
 * indicates the status of the RTC clock. That's what "TSI" stands for. */
#define CMOS_TSI 0x4

/* The rest of the flags is reserved. */


/* TODO: Look it up in the IBM 5160 reference manual and add documentation for
 * the bits! */
#define CMOS_SHUTDOWN 0xf


/* Type of the diskette drives installed. */
#define CMOS_DRIVETYPE 0x10

/* Generates bit mask for one of the drives. Any value less than `0' or greater
 * than `1' triggers an undefined result. */
#define CMOS_DRIVE(num) (0xf0 >> (num)) 

/* No drive present. */
#define CMOS_NODRIVE 0x0

/* Double-sided diskette drive (48 tracks per inch). */
#define CMOS_DOUBLESIDED 0x10

/* High capacity diskette drive (96 tracks per inch).
#define CMOS_HIGHCAP 0x20


/* Defines the type of the fixed disk drives installed. */
#define CMOS_FDRIVETYPE 0x12


/* Argument for `CMOS_FDRIVE' macro, which selects drive C (first drive). */
#define CMOS_FDRIVEC 0x0

/* Argument for `CMOS_FDRIVE' macro, which selects drive D (second drive). */
#define CMOS_FDRIVED 0x1

/* Generates bit mask for one of the drives. Any value less than `0' or greater
 * than `1' triggers an undefined result.
 * Recommended to be used in conjunction with the `CMOS_FDRIVE.' macros. */
#define CMOS_FDRIVE(num) (0xf0 >> (num))

#define CMOS_NOFDRIVE 0x0

/* Explanation for abbreviations:
 * C = Cylinder count
 * H = Head count
 * W = Write precompensation
 * L = Landing Zone
 * 
 * The IBM PC AT reference says in column "Write Pre-comp" for the 0x9th
 * type "no8," which is obviously a mistake. So is it "no" or "8"? I guessed
 * "no" because the "8" key is close to the "o" and it's just a typing error.
 * The other way around seems illogical due to the "n," so I finally decided
 * on "no."
 *
 * I hope I have not made a mistake here... it is easy to get confused with
 * that table from the IBM PC AT reference I have copied those values from. */
#define CMOS_C306_H4_W128_L305 0x1
#define CMOS_C615_H4_W300_L615 0x2
#define CMOS_C615_H6_W300_L615 0x3
#define CMOS_C940_H8_W512_L940 0x4
#define CMOS_C940_H6_W512_L940 0x5
#define CMOS_C615_H4_L615 0x6
#define CMOS_C462_H8_W256_L511 0x7
#define CMOS_C733_H5_L733 0x8
#define CMOS_C900_H15_L901 0x9
#define CMOS_C820_H3_L820 0xa
#define CMOS_C855_H5_L855 0xb
#define CMOS_C855_H7_L855 0xc
#define CMOS_C306_H8_W128_L319 0xd
#define CMOS_C733_H7_L733 0xe

/* The last type is reserved. The IBM PC AT reference says it is
 * set to all zeros but I don't really see the benefit in this assumption. */


/* So-called "equipment byte." Believe it or not, it contains information
 * about the system equipment! */
#define CMOS_EQUIP 0x14

/* Bit mask to get the number-of-drives value. */
#define CMOS_NUMDRVS 0xc0

/* One drive installed. */
#define CMOS_ONEDRV 0x0

/* Two drives installed. */
#define CMOS_TWODRVS 0x40

/* The rest of the values is reserved. */

/* Bit mask to get the type of the primary display. */
#define CMOS_PRIMDISP 0x30

/* The value `0x0' is reserved. */

/* Primary display is attached to the Color/Graphics Monitor Adapter in the
 * 80-column mode. */
#define CMOS_COLOR_C40 0x10

/* Primary display is attached to the Color/Graphics Monitor Adapter in the
 * 80-column mode. */ 
#define CMOS_COLOR_C80 0x20

/* Primary display is attached to the Monochrome Display and Printer
 * Adapter. */
#define CMOS_MONO 0x30

/* Bits `3' and `2' are not used. */

/* 0 - FPU not installed.
 * 1 - FPU installed. */
#define CMOS_FPU 0x2

/* 0 - No diskette drive installed.
 * 1 - At least one diskette drive installed. */
#define CMOS_DRIVEAVAIL 0x1


/* Contains the amount of available memory. 
 * Both I/O ports constitute a 16-bit value, which defines a certain value.
 * Therefore, you need to put both values together to get a meaningful
 * result. */
#define CMOS_LOWMEMSIZE 0x15
#define CMOS_HIGHMEMSIZE 0x16

/* 256 KB system-board RAM. */
#define CMOS_MEM_256KB 0x100

/* 512 KB system-board RAM. */
#define CMOS_MEM_512KB 0x200

/* 640 KB system-board RAM + 128 KB memory expansion option.
 * The IBM PC AT reference says "640Kb 512Kb" about the size here. Since
 * 512 KB has already been defined, it has to be 640 KB instead. I hate
 * mistakes in such manuals... :-( */ 
#define CMOS_MEM_640KB 0x280


/* TODO: add documentation here! i currently do not fully understand
 * what expansion memory is supposed to mean. and what is the difference
 * between I/O addresses 0x17,0x18 and 0x30,0x31? and what is meant by
 * "I/O adapter" even though we are talking MEMORY, that is, RAM? */ 
/* Both I/O ports constitute a 16-bit value, which defines a certain value.
 * Therefore, you need to put both values together to get a meaningful
 * result. */
#define CMOS_MEMEXLOW 0x17
#define CMOS_MEMEXHIGH 0x18


/* TODO: the IBM PC AT reference says something along the lines "the checksum
 * is at CMOS addresses 0x10-0x20." but isn't it at 0x2e,0x2f? how is that?
 * what does that mean? */
/* Checksum bytes. */
#define CMOS_CHKSUMHIGH 0x2e
#define CMOS_CHKSUMLOW 0x2f


/* TODO: add documentation here! read the TODO comment of CMOS addresses
 * 0x17,0x18 to learn what im talking about. and, as always, READ THE IBM PC
 * AT REFERENCE! */
#define CMOS_MEMEXLOW 0x30
#define CMOS_MEMEXHIGH 0x31


/* Century byte. */
#define CMOS_RTC_CENT 0x32


/* Contains arbitrary system information. */
#define CMOS_INFO 0x33

/* 0 - 128 KB memory expansion option not installed. 
 * 1 - 128 KB memory expansion option installed. */
#define CMOS_MEMEX_128KB_AVAIL 0x80

/* Used to print out a first user message after initial setup by the setup
 * utility.
 * That's what the IBM PC AT reference says. I don't know when it is set or
 * cleared. Dig deeper for more information. */
#define CMOS_INITMSG 0x40


/* ################################
 * Here begins the high-level part.
 * ################################ */ 

#if __ASSEMBLER__ != 1

/* TODO: add typedef uint8_t cmos_err_t, cmos_init and cmos_destroy if needed! 
 * typedef uint8_t cmos_err_t;
 * cmos_err_t cmos_init(void);
 * cmos_err_t cmos_destroy(void);
 * These are just examplatory declarations. Change them if need be.  */

/* Waits until the RTC is readable. */
static inline void cmos_rtc_waitrd(void);

/* Acquires the RTC, so that the values are no more updated and the CPU (us)
 * can write to it. */ 
static inline void cmos_rtc_lock(void);

/* Releases the RTC, so that the values are resumed to be updated and the
 * CPU (us) cannot write ot  
/* Writes a value to a CMOS configuration memory location. This includes
 * those memory locations for which no explicit writing function exists. */
static inline void cmos_wr_conf(uint8_t mask, uint8_t val, uint8_t cmos_addr);

/* Writes a value to a CMOS RTC memory location. To be used with all RTC 
 * values but NOT the RTC status registers! */
static inline void cmos_wr_rtc(uint8_t val, uint8_t cmos_addr);

/* Reads a value from a CMOS configuration memory location. This includes
 * those memory locations for which no explicit reading function exists. */
static inline void cmos_rd_conf(uint8_t cmos_addr);

/* Reads a value from a CMOS RTC memory location. To be used with all RTC
 * values but NOT the RTC status registers! */   
static inline void cmos_rd_rtc(uint8_t cmos_addr);


/* ################
 * # I/O HELPERS: #
 * ################ */

/* The RTC functions might make use of the functions actually intended for
 * configuration bytes; the RTC functions basically add some security wrapper
 * to the configuration functions and call them internally. I could have
 * just as well called the configuration functions something with `nrml' for
 * "normal" functions but to the driver user the usage of configuration
 * functions is not visible anyway and a "normal" might appear confusing: what
 * is normal in this context? */

static inline void cmos_wr_conf(uint8_t mask, uint8_t val, uint8_t cmos_addr) {
    io_outb(cmos_addr, CMOS_SELADDR);
    io_outb(val & mask, cmos_addr);
}

static inline void cmos_wr_rtc(uint8_t val, uint8_t cmos_addr) {
    uint8_t reg_a = cmos_rd_conf(CMOS_RTC_REGA);

/* TODO: optimize this. if we want to update multiple rtc values simultaneously
 * what to do? calling this function multiple times requires setting CMOS_UIP
 * every time, which is bad for performance. */
/* TODO: do i have to wait until the RTC is accessible here first? Or can I just
 * "acquire" the without further action? */ 
    cmos_wr_conf(reg_a | CMOS_UIP, CMOS_RTC_REGA);
    cmos_wr_conf(val, cmos_addr);

    /* `CMOS_UIP' is explicitly cleared because the original status register
     * A content might already contain that bit set. */ 
    cmos_wr_conf(reg_a & ~CMOS_UIP, CMOS_RTC_REGA);  
}

static inline void cmos_rd_conf(uint8_t cmos_addr) {
    io_outb(cmos_addr, CMOS_SELADDR);
    return io_inb(CMOS_DATA);
}

static inline void cmos_rd_rtc(uint8_t cmos_addr) {
    /* Waits as long as the RTC is updated. A synchronization mechanism
     * similar to vertical synchronization on screens. We simply presume
     * the CPU is fast enough to read the data before the next RTC update
     * follows. */ 
    while (cmos_rd_conf(CMOS_RTC_REGA) & CMOS_RTC_UIP);
    return io_inb(CMOS_DATA);
}

/* I thought about having a `struct cmos_rtc' to keep all the RTC values there
 * but decided against it because it would be pretty useless. No one needs
 * to read/write all the RTC values at a time. If needed at a higher level,
 * add it there, but it is not necessary in this CMOS driver. */

/* TODO: find out if the CMOS controller requires some waiting period for it
 * to process the input and possibly produce output similarly to
 * `i8259A_waitout'. */

#endif /* __ASSEMBLER__ != 1 */

#endif /* CMOS_H */
