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

#ifndef i8042_H
#define i8042_H

#include <types.h>
#include <io.h>


/* ###############################################
 * # I/O PORTS OCCUPIED BY INTEL 8042 REGISTERS: #
 * ############################################### */

/* All the access permissions (read-only, write-only, and read and write)
 * describe the perspective of the CPU; a read-only register is thus read-only
 * to the CPU, i.e., us. */   

/* Status register; read-only. */
#define i8042_STATUS 0x64

/* Command register; write-only */
#define i8042_CMD 0x64

/* Data register: read and write. */ 
#define i8042_DATA 0x60


/* #########################
 * # STATUS REGISTER BITS: #
 * ######################### */


/* 0 - Output buffer is empty.
 * 1 - Output buffer is full. */
#define i8042_STAT_OUTFULL 0x1

/* 0 - Input buffer is empty.
 * 1 - Input buffer is full. */
#define i8042_STAT_INEMPTY 0x2

/* 0 - System flag is 0.
 * 1 - System flag is 1. */
#define i8042_STAT_SYSFLAG 0x4
/* Best description ever. But the manual didn't contain more to it I could
 * find, sorry. */

/* 0 - Data register was written to.
 * 1 - Command register was written to. */
#define i8042_INPUT 0x8

/* TODO: add description! */
#define i8042_STAT_INHIBIT 0x10

/* 0 - Transmission completed successfully.
 * 1 - Transmission by keyboard controller timed out. */
#define i8042_SEND_TIMEOUT 0x20

/* 0 - Receiving completed successfully.
 * 1 - Receiving by keyboard controller timed out. */
#define i8042_RECV_TIMEOUT 0x40

/* 0 - Odd parity of keyboard-controller message (success).
 * 1 - Even parity of keyboard-controller message (error). */
#define i8042_PARITY 0x80


/* ###################################################
 * # VALID COMMANDS AND CORRESPONDING OUTPUT VALUES: #
 * ################################################### */


/* Send current command byte to output buffer. */
#define i8042_RD_CMD 0x20

/* Use next byte written to data register as command byte. */
#define i8042_WR_CMD 0x60


/* ######################
 * # COMMAND BYTE BITS: #
 * ###################### */

/* All bits not documented by any descriptions are reserved and should be left
 * clear. */

/* 0 - Do not generate an interrupt when output buffer is full.
 * 1 - Generate interrupt when output buffer is full. */
#define i8042_INT 0x1

/* 0 - Write 0 to aforementioned system flag bit of status register.
 * 1 - Write 1 to aforementioned system flag bit of status register. */
#define i8042_CMD_SYSFLAG 0x4

/* 0 - Enable keyboard inhibit function.
 * 1 - Disable keyboard inhibit function. */
#define i8042_CMD_INHIBIT 0x8

/* 0 - Enable the keyboard.
 * 1 - Disable the keyboard. */
#define i8042_CMD_DISABLE 0x10

/* 0 - Disable IBM PC mode.
 * 1 - Enable IBM PC mode. */
#define i8042_PCMODE 0x20
/* IBM PC mode denotes the IBM PC keyboard interface. In that mode, no
 * parity-checking is performed and no scan codes are converted. */

/* 0 - Disable IBM PC compatibility mode.
 * 1 - Enable IBM PC compatibility mode. */
#define i8042_PC_COMPAT 0x40
/* IBM PC compatibility mode enables conversion from the scancodes received
 * from the keyboard to the former IBM PC scancodes. */


/* Performs internal self-tests. */
#define i8042_SELFTEST 0xaa

/* Content of output buffer if self-test (command `0xaa') did not find any
 * errors. */
#define i8042_SELFT_NOERR 0x55


/* Interface test, which checks the keyboard clock and data lines. */
#define i8042_IFTEST 0xab

/* Content of output buffer if no error. */
#define i8042_IFT_NOERR 0x0


/* Sends 16 bytes of diagnostic information in scancode format to the system
 * state of input port, state of output port, program status word. */
#define i8042_DUMP 0xac

/* Disables the keyboard by setting the aforementioned bit 4 of the command
 * byte. */
#define i8042_DISABLE 0xad

/* Enables the keyboard by clearing the aforementioned bit 4 of the command
 * byte. */
#define i8042_ENABLE 0xae

/* Reads the input port and stores the content in the output buffer. */
#define i8042_RD_INP 0xc0

/* ####################
 * # INPUT PORT BITS: #
 * #################### */

/* All bits not defined here are reserved. */


/* 0 - Disable 2nd 256kb of RAM.
 * 1 - Enable 2nd 256kb of RAM. */
#define i8042_256KB 0x10

/* 0 - Manufacturing jumper not installed.
 * 1 - Manufacturing jumper installed. */
#define i8042_JMP 0x20

/* 0 - Primary display has color adapter.
 * 1 - Primary display has monochrome adapter. */
#define i8042_DISPLAY 0x40

/* 0 - Keyboard not inhibited.
 * 1 - Keyboard inhibited. */
#define i8042_INP_INHIBIT 0x80


/* Reads the output port and stores the content in the output buffer. */
#define i8042_RD_OUTP 0xd0

/* Write the next byte written to the data register to the output port. */
#define i8042_WR_OUTP 0xd1

/* #####################
 * # OUTPUT PORT BITS: #
 * ##################### */

/* All bits not defined here are reserved. */


/* 0 - Reset the system.
 * 1 - System online. */
#define i8042_SYSRESET 0x1

/* 0 - A20 disabled.
 * 1 - A20 enabled. */
#define i8042_A20 0x2

/* 0 - Output buffer empty.
 * 1 - Output buffer full. */
#define i8042_OUTP_OUTFULL 0x10

/* 0 - Input buffer full.
 * 1 - Input buffer empty. */
#define i8042_OUTP_INEMPTY 0x20

/* Clock line state (output).
 * TODO: find out what that "(output)" means and adjust macro name and
 * description then! */
#define i8042_CLOCK_OUT 0x40

/* Data line state (output).
 * TODO: find out what that "(output)" means and adjust macro name and
 * description then! */
#define i8042_DATA_OUT 0x80

/* ########################################################################## */

/* Read test inputs `T0' and `T1' into output buffer. */
#define i8042_RD_TEST 0xe0

/* ########################
 * # TEST INPUT MEANINGS: #
 * ######################## */

/* `T0' test input; state of clock line (input).
 * TODO: find out what that "(input)" means and adjust macro name and
 * description then! */
#define i8042_CLOCK_IN 0x0

/* `T1' test input; state of data line (input).
 * TODO: find out what that "(input)" means and adjust macro name and
 * description then! */
#define i8042_DATA_IN 0x1


/* Clears bits 0 - 3 of the output port for roughly 6 microsecs by indication
 * in the four least significant bits of the command. */
#define i8042_PULSE_PREFIX 0xf0

/* #################
 * # AFFIRM PULSE: #
 * ################# */

/* Clear bit temporarily. */
#define i8042_PULSE_YES 0x0

/* Do not modify bit. */
#define i8042_PULSE_NO 0x1

/* #######################################
 * # INTERFACE TEST (ABBR. IFT) RESULTS: #
 * ####################################### */

/* Keyboard clock line stuck low. */
#define i8042_IFT_CLOW 0x1

/* Keyboard clock line stuck high. */
#define i8042_IFT_CHIGH 0x2

/* Keyboard data line stuck low. */
#define i8042_IFT_DLOW 0x3

/* Keyboard data line stuck high. */
#define i8042_IFT_DHIGH 0x4


/* ########################################################################## 
 * Now, that was all the low-level stuff of the controller. What follows up
 * is the abstraction from that. It ranges from "still severely low-level" to
 * "rather high-level" and thus can be accessed from different abstraction
 * levels to interact with the controller in the most efficient way.
 * ########################################################################## */




#if __ASSEMBLER__ != 1

/* High-level error code type. Only returned by high-level functions such as
 * `i8042_init'. Unique across the whole Intel 8042 driver implementation. */
typedef uint8_t i8042_err_t;

/* #############################
 * # ERROR CODES AND MEANINGS: #
 * ############################# */

/* I could have associated each function with a certain set of error codes
 * but a per-driver association suffices and anything more detailed would
 * just be overkill, IMO. */

/* No error occured. */
#define E_i8042_NOERR 0x0

/* Self-test returned an error. */
#define E_i8042_SELFTEST 0x1

/* Interface test returned an error. */
#define E_i8042_IFTEST 0x2


/* Initializes the Intel 8042 chip. */
i8042_err_t i8042_init(void);
/*i8042_err_t i8042_destroy(void);*/ /* TODO: add destroy func!
                                      * uncomment when needed */

/* Waits until the input buffer is emptied. */ 
static inline void i8042_waitin(void);

/* Waits until the output buffer is filled. This function uses polling to
 * wait. */ 
static inline void i8042_waitout(void);

/* Waits until the output buffer is filled. This function is
 * interrupt-driven, that is, it is idle until an interrupt indicating the
 * availability of data in the output buffer is fired. */
static inline void i8042_waitout_int(void);

/* Reads from the input port. */
static inline uint8_t i8042_rdinp(void);

/* Reads from the output port. */
static inline void i8042_rdoutp(uint8_t outp);

/* Writes to the output port. */
static inline uint8_t i8042_wroutp(void);

/* Reading from the output buffer requires data to be placed by the controller
 * beforehand. The function `i8042_waitout' waits for this data. Therefore,
 * `i8042_waitout' and `inb' (read from I/O register, e.g., output buffer)
 * are always tied together and this forms the `i8042_indata' routine.
 * The same goes for `i8042_waitin', which waits for the input buffer to be
 * empty, and the data and command registers, which are part of the input
 * buffer. This yields the functions `i8042_indata' and `i8042_incmd'.
 * What is left is the status register, which, according to the IBM 5170
 * Technical Reference Manual, "may be read at any time." Therefore, no
 * synchronization using any of the `i8042_wait*' functions is necessary and
 * the `i8042_instat' is only existent for completeness's sake. */

/* Write data into data register. */
static inline void i8042_outdata(uint8_t val);

/* Write data into command register. */
static inline void i8042_outcmd(uint8_t val);

/* Read data from data register. */
static inline uint8_t i8042_indata(void);

/* Read data from status register. */
static inline uint8_t i8042_instat(void);


/* ######################
 * # ENABLE OR DISABLE. #
 * ###################### */

/* Disable the Intel 8042. */
static inline void i8042_disable(void) {
    i8042_outcmd(i8042_DISABLE);
}

/* Enables the Intel 8042. */
static inline void i8042_enable(void) {
    i8042_outcmd(i8042_ENABLE);
}


/* ##################
 * # TEST COMMANDS: #
 * ################## */

/* I thought about introducing extra error code types for the self-test and
 * interface test results but concluded that we do not need them.
 * First, such codes are only used for type-safety (with `enum's) and code
 * readability - ideals that are (1) irrelevant on such a low-level and
 * (2) especially in such a messy thing as a driver for the Intel 8042.
 * Second, error codes do not fit here well because it is the direct data
 * that is returned, read directly from the I/O port and unprocessed. Error
 * codes should signify a state obtained by processing done by the CPU, i.e.,
 * by us. Of course, that comes along with the decision to return the data
 * directly from the testing routines and not check it against values meaning
 * no error (e.g., `i8042_IFT_NOERR'). But that whole second point is more a
 * matter of personal opinion and taste. */


/* Performs a self-test on the Intel 8042. */
static inline uint8_t i8042_selftest(void) {
    i8042_outcmd(i8042_SELFTEST);
    return i8042_indata();
}

/* Performs an interface test on the Intel 8042. For more information, see
 * the IBM 5170 Technical Reference. */ 
static inline uint8_t i8042_iftest(void) {
    i8042_outcmd(i8042_IFTEST);
    return i8042_indata();
}


/* ##########################
 * # ENABLE OR DISABLE A20. #
 * ########################## */


/* Disables A20. */
static inline void i8042_disablea20(void) {
    i8042_wroutp(i8042_rdoutp() & ~i8042_A20);
}

/* Enables A20. */
static inline void i8042_enablea20(void) {
    i8042_wroutp(i8042_rdoutp() | i8042_A20);
}


/* #################
 * # SYSTEM RESET. #
 * ################# */

/* Resets the whole system. */
static inline void i8042_sysreset(void) {
    /* No need for output port's content preservation - the system will be
     * reset anyway. */
    i8042_outcmd(i8042_WR_OUTP);

    /* 1's-complement with `~' because `i8042_SYSRESET == 0' means reset,
     * not `i8042_SYSRESET == 1' as with most other flags. */
    i8042_outdata(~i8042_SYSRESET);

    /* TODO: replace the cli; hlt sequence with a true error handling
     * mechanism. best would be to use something like assert from the c
     * standard library and then assert passes execution to some other
     * central error catcher or something. */
    /* Should never happen but just in case... */
    __asm__ volatile("cli\n\thlt\n\t");
}

/* ###################################
 * # READ OR WRITE THE COMMAND BYTE. #
 * ################################### */


/* Sets the content of the command byte. */
static inline void i8042_setcmdbyte(uint8_t cmdbyte) {
    i8042_outcmd(i8042_WR_CMD);
    i8042_outdata(cmdbyte);
}

/* Returns the command byte. */
static inline uint8_t i8042_cmdbyte(void) {
    i8042_outcmd(i8042_RD_CMD);
    return i8042_indata();
}

/* ##################################
 * # READ THE INPUT OR OUTPUT PORT. #
 * ################################## */

static inline uint8_t i8042_rdinp(void) {
    i8042_outcmd(i8042_RD_INP);
    return i8042_indata();
}

static inline uint8_t i8042_rdoutp(void) {
    i8042_outcmd(i8042_RD_OUTP);
    return i8042_indata();
}

static inline void i8042_wroutp(uint8_t outp) {
    i8042_outcmd(i8042_WR_OUTP);
    i8042_outdata(outp);
}

/* #################################
 * # WAIT FOR BUFFERS UNTIL READY. #
 * ################################# */

static inline void i8042_waitin(void) {
    for (uint8_t status = i8042_instat(); !(status & i8042_STAT_INEMPTY);
    status = i8042_instat())
        __asm__ volatile("pause");
}

static inline void i8042_waitout(void) {
    for (uint8_t status = i8042_instat(); !(status & i8042_STAT_OUTFULL);
    status = i8042_instat())
        __asm__ volatile("pause");
}

/* TODO: when to use i8042_waitout, when i8042_waitout_int? Or remove
 * `i8042_waitout_int' altogether? Or change the implementation of the
 * interrupt-driven function a bit? */ 

static inline void i8042_waitout_int(void) {
    __asm__ volatile(
        "pushf\n\t"
        "sti\n\t"
        "hlt\n\t"
        "popf\n\t"
    ::: "cc", "memory");
}

/* ################
 * # I/O HELPERS: #
 * ################ */ 

static inline void i8042_outdata(uint8_t val) {
    i8042_waitin();
    outb(val, i8042_DATA);
}

static inline void i8042_outcmd(uint8_t val) {
    i8042_waitin();
    outb(val, i8042_CMD);
}

static inline uint8_t i8042_indata(void) {
    i8042_waitout();
    return inb(i8042_DATA);
}

static inline uint8_t i8042_instat(void) {
    return inb(i8042_STATUS);
}


#endif /* __ASSEMBLER__ != 1 */

#endif /* i8042_H */
