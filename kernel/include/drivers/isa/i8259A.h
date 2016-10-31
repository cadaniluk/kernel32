#ifndef i8259A_H
#define i8259A_H

#include <types.h>
#include <io.h>

/* #####################
 * # COMMON I/O PORTS: #
 * ##################### */

/* Master PIC base I/O port. */
#define i8259A_MBASE 0x20

/* Slave PIC base I/O port. */
#define i8259A_SBASE 0xa0

/* None of the original IBM PCs had more than two PICs but I don't think
 * it's prohibited to have more on an x86 machine. Therefore, I wrote the
 * driver to be compliant for an arbitrary number of PICs, without being
 * restricted to just one master and one slave.
 * Every Intel 8259A chip in the system requires separate initialization. */

/* #########
 * # ICW1: #
 * ######### */

/* Generates ICW1. `0x10' sets bit 4, which, together with a cleared A0,
 * indicates ICW1. */
#define i8259A_ICW1(bits) (0x10 | (bits))

#define i8259A0_ICW1 0x0

/* For the difference between edge-triggered and level-triggered modes, view
 * this link:
 * http://electronics.stackexchange.com/questions/21886/ \
 * what-does-edge-triggered-and-level-triggered-mean (3/29/16)
 * My understanding:
 * Both modes make some circuit become active.
 * Edge-triggered mode activates it when the signal is at the edge from
 * low to high or high to low.
 * Level-triggered mode activates it when the signal is at a certain level. On
 * this controller, the "level" is reached when the signal is raised high. */

/* 0 - Edge-triggered interrupt mode.
 * 1 - Level-triggered interrupt mode. */
#define i8259A_LTIM 0x8

/* ADI (Address Interval) at `0x4' is not used on x86, so setting that bit has
 * no effect. */

/* 0 - Multiple PICs in the system.
 * 1 - One PIC in the system; no ICW3.
 * The Intel handbook denotes this bit as "SNGL." I think the name I used here
 * is easier to understand and just as short, so I decided to use that
 * instead. */
#define i8259A_SINGLE 0x2

/* 0 - No ICW4.
 * 1 - ICW4 will be read.
 * The Intel handbook denotes this bit as "IC4." I would've changed it
 * to "ICW4" for better readability but that would crash with the `i8259A_ICW4'
 * macro used to generate the ICW4. */
#define i8259A_IC4 0x1

/* #########
 * # ICW2: #
 * ######### */

/* Generates ICW2. `base' contains bits 3 - 7 of the interrupt vector (IV) of
 * the interrupt requests (IR). The code of the IR (0 - 7) consumes bits 0 - 2.
 * `base' and the code constitute the IV. */
#define i8259A_ICW2(base) ((base) << 0x3)

#define i8259A0_ICW2 0x1

/* #########
 * # ICW3: #
 * ######### */

#define i8259A0_ICW3 0x1

/* TODO: complete this! */
#define i8259A_ICW3(bits) ()

/* #########
 * # ICW4: #
 * ######### */

/* Only for consistency's sake. */
#define i8259A_ICW4(bits) (bits)

#define i8259A0_ICW4 0x1

/* 0 - special fully nested mode
 * 1 - not special fully nested mode */
#define i8259A_SFNM 0x10

/* 0 - Buffered mode disabled.
 * 1 - Buffered mode enabled. */
#define i8259A_BUF 0x8

/* 0 - Slave PIC.
 * 1 - Master PIC.
 * If `i8259A_BUF' is set, this bit indicates whether the PIC is a master or
 * slave. If `i8259A_BUF' is clear, it has no function. */
#define i8259A_MS 0x4

/* 0 - Automatic end of interrupt is disabled.
 * 1 - Automatic end of interrupt is enabled.
 * AEOI will perform a non-specific EOI command at the second
 * #INTA pulse. Should only be used when a nested multilevel interrupt
 * structure is not needed within a single PIC.
 * TODO: find out what that "nested multilevel..." means!
 * Pre-1985 8259A chips can only use AEOI as a master. Later controllers
 * can operate with AEOI as master or slave. */
#define i8259AEOI 0x2

/* 0 - Intel MCS processor used.
 * 1 - x86 processor used. */
#define i8259A_uPM 0x1

/* #########
 * # OCW1: #
 * ######### */

/* OCW1 accesses the IMR. Set bit means "masked off" or "IR line disabled,"
 * cleared bit means "IR line enabled." */

/* For consistency's sake. */
#define i8259A_OCW1(bits) (bits)

#define i8259A0_OCW1 0x1

/* #########
 * # OCW2: #
 * ######### */

/* For consistency's sake once again... this makes me question the
 * consistency; is it still consistency if the stubs for consistency
 * constitute the majority of overall functions? I don't think so but
 * this file hasn't reached this vast extent (yet). */
#define i8259A_OCW2(bits) (bits)

/* An IR level encoded in bits `0' - `2' in OCW2. Used only with certain
 * commands. */
#define i8259A_OCW2_IR(level) (level)

#define i8259A0_OCW2 0x0

/* Non-specific EOI command.
 * Clears the current highest priority interrupt level since, in fully
 * nested mode, this must be the last one acknowledged and serviced.
 * Useful in fully nested mode. */
#define i8259A_NEOI 0x20

/* Specific EOI command.
 * Clears a specific interrupt level specified in bits 0 - 2 (see
 * the `i8259A_OCW2_IR' macro).
 * Useful in modes other than fully nested. */
#define i8259A_SEOI 0x60

/* Rotate on non-specific EOI command.
 * The priorites are rotated in such a manner that the IR chosen by the
 * non-specific EOI command gets assigned the lowest priority (priority
 * level 7).
 * Used if the devices have equal priority. */
#define i8259A_ROTNEOI 0xa0

/* Rotate on specific EOI command (in conjunction with IR level). */
#define i8259A_ROTSEOI 0xe0

/* Rotate in AEOI mode (set). */
#define i8259A_ROTAEOIS 0x80

/* Rotate in AEOI mode (clear). */
#define i8259A_ROTAEOIC 0x0

/* Set priority (in conjunction with IR level).
 * The priority level set is rotated, so that the priority level specified
 * in bits 0 - 2 of OCW2 is set as the new bottom priority level. */
#define i8259A_SETPR 0xc0

/* No-op.
 * TODO: remove if superflous! */
#define i8259A_NOP 0x40

/* #########
 * # OCW3: #
 * ######### */

/* OCW3 requires bit 3 to be always set. */
#define i8259A_OCW3(bits) (0x8 | (bits))

#define i8259A0_OCW3 0x0

/* Notice something? While I wrote the single commands of OCW2 as macros,
 * I wrote the single bits of OCW3. That's simply because the manual did
 * not describe the abbr. meanings of the OCW2 bits sufficiently but did so
 * with the commands. I simply could not detect a pattern, so I decided to
 * write out the commands directly. */

/* 0 - Do not change mask mode; `i8259A_SMM''s value is unimportant.
 * 1 - Enable special mask mode (ESMM). */
#define i8259A_ESMM 0x40

/* 0 - Normal mask mode.
 * 1 - Special mask mode (SMM).
 * Some interrupt handlers may want to enable some other interrupts during
 * execution. However, all lower-priority IRs are disabled as long as no EOI
 * occured, so other IRs cannot be enabled.
 * SMM helps here: it inhibits interrupts at the IR bits set in the IMR and
 * enables interrupts from all other levels, be it higher or lower. */
#define i8259A_SMM 0x20

/* 0 - No poll command
 * 1 - Poll command */
#define i8259A_POLL 0x4

/* 0 - Do not read any register
 * 1 - Read a register
 * The abbreviation used in the reference is "RR." I figured it stands for
 * "read register," so I elaborated on that abbreviation a bit and came up with
 * the lower name. No assurance, though. */
#define i8259A_RDREG 0x2

/* 0 - Read IRR
 * 1 - Read ISR
 * The abbreviation used in the reference is "RIS." I figured it stands for
 * "read in-service," so I elaborated on that abbreviation a bit and came up
 * with the lower name. No assurance, though. */
#define i8259A_RDISR 0x1


#if __ASSEMBLER__ != 1

/* TODO: remove typedef if unused. */
typedef uint8_t i8259A_err_t;

/* Array or four separate ICWs? I chose the separate way because arrays are
 * for iteration and more "rythmic" variables, if you know what I mean. */
void i8259A_init(uint8_t icw1, uint8_t icw2, uint8_t icw3, uint8_t icw4,
uint16_t base);
/*void i8259A_destroy(uint16_t base);*/ /* TODO: add destroy function if
                                        * necessary! */

/* TODO: this driver is NOT finished! finally finish it! */

/* Waits until the i8259A is ready for further interaction with the CPU (us). */ 
static inline void i8259A_wait(void);

/* The controller requires some time before it is ready for action. This
 * additional time is granted with this function. */
static inline uint8_t i8259A_in(uint16_t port);

/* The controller requires some time before it is ready for action. This
 * additional time is granted with this function. */
static inline void i8259A_out(uint8_t val, uint16_t port);

/* Writes ICW1. */
static inline void i8259A_icw1(uint8_t icw1, uint16_t base);

/* Writes ICW2. */
static inline void i8259A_icw2(uint8_t icw2, uint16_t base);

/* Writes ICW3. */
static inline void i8259A_icw3(uint8_t icw3, uint16_t base);

/* Writes ICW4. */
static inline void i8259A_icw4(uint8_t icw4, uint16_t base);


/* You may wonder why the OCW writing functions have a bit mask parameter.
 * Well, we need some way to set specific bits but leave others as-is. Simply
 * setting an OCW might erase important bits. A bit mask + new bit values
 * seemed like the best way. */

/* Writes OCW1 (to the IMR). */ 
static inline void i8259A_ocw1_wr(uint8_t ocw1, uint16_t base);

/* Reads OCW1, that is, the IMR. */
static inline uint8_t i8259A_ocw1_rd(uint16_t base);

/* Writes OCW2. */
static inline void i8259A_ocw2(uint8_t ocw2, uint16_t base);

/* Writes OCW3. */
static inline void i8259A_ocw3_wr(uint8_t mask, uint8_t ocw3, uint16_t base);

/* Reads OCW3. */
static inline uint8_t i8259A_ocw3_rd(uint16_t base);


/* TODO: recheck from where to read the register contents! I just assumed
 * they are read from the corresponding OCW I/O port but that might be false!
 * the 8259 datasheet contains some information but it's not fully clear. */

/* #################
 * # REGISTER I/O: #
 * ################# */

/* Reads the IMR's content. */
static inline uint8_t i8259A_imr_rd(uint16_t base) {
    return i8259A_ocw1_rd(base);
}

/* Writes to the IMR. */
static inline void i8259A_imr_wr(uint8_t imr, uint16_t base) {
    i8259A_ocw1(imr, base);
}

/* Using bits 0 - 1 in OCW3 you can select which register is to be read,
 * either IRR or ISR. The thing is we do not need to issue an OCW3 every time
 * we want to read these registers; it is more like a flag, which is changed
 * for every OCW3. By default, that flag indicates IRR to be read.
 * I know this is really bad; an internal variable exposed to the user
 * in shape of a declaration in the header file... Christ, that's really
 * crappy. But it works and I don't know how to do it better. `static inline'
extern uint8_t rd_reg;

/* Reads the ISR's content. */
static inline uint8_t i8259A_isr_rd(uint16_t base) {
    if (!(rd_reg & i8259A_RDISR))
        i8259A_ocw3_wr(0x3, i8259A_RD | i8259A_RDISR, base);

    return i8259A_ocw3_rd(base);
}

/* Reads the IRR's content. */
static inline uint8_t i8259A_irr_rd(uint16_t base) {
    if (rd_reg & i8259A_RDISR)
        i8259A_ocw3_wr(0x3, i8259A_RDREG, base); /* 0 - IRR; no macro because I
                                                * only want to use macros when
                                                * exposing something to the user
                                                * not in the implementation */
    return i8259A_ocw3_rd(base);
}


/* TODO: finish that driver! make it a bit more readable, put the code in
 * some order. some functions haven't been completed or implemented at all
 * yet. be aware that some bits create dependencies to other registers or
 * command words. example:
 * 2-273:
 * it should be noted that an is bit that is masked
 * by an imr bit will not be cleared by a non-specific
 * eoi if the 8259a is in the special mask mode.
 *
 * don't get trapped in those subtleties. they
 * might create bad bugs in the driver and are
 * malevolent. read carefully and FINISH THAT DRIVER!!! */

/* #################
 * # EOI COMMANDS: #
 * ################# */

static inline void i8259A_eoi_spec(uint8_t ir, uint16_t base) {
    i8259A_ocw2(i8259A_SEOI | i8259A_SEOI | i8259A_OCW2_IR(ir), base);
}

static inline void i8259A_eoi_nonspec(uint16_t base) {
    i8259A_ocw2(i8259A_NEOI, base);
}

/* TODO: resume this! */
static inline void i8259A_

/* TODO: FINISH THIS DRIVER! BUFFER MODE AND SPECIAL MASK MODE DESCRIPTIONS
 * AND POSSIBLY INTERFACES HAVE TO BE ADDED STILL!!! */

/* ##################
 * # MISCELLANEOUS: #
 * ################## */

/* Sets the bottom priority level. */
static inline void i8259A_setpr(uint8_t priority, uint16_t base) {
    i8259A_ocw2(0xc7, i8259A_SETPR | i8259A_OCW2_IR(priority), base);
}


/* Some macro magic could probably shorten all these one-liners into one
 * "base" macro and other "instantiations" of that macro. But the current
 * way is just fine, except that you might get some kind of cancer when gawking
 * at your screen for too long.
 * TODO: maybe do what is said above and convert the lower functions to
 * those base macros and instantiations. */

/* ##################
 * # ICW FUNCTIONS: #
 * ################## */

static inline void i8259A_icw1(uint8_t icw1, uint16_t base) {
    i8259A_out(i8259A_ICW1(icw1 & mask), base | i8259A0_ICW1);
}

static inline void i8259A_icw2(uint8_t icw2, uint16_t base) {
    i8259A_out(i8259A_ICW2(icw2), base | i8259A0_ICW2);
}

static inline void i8259A_icw3(uint8_t icw3, uint16_t base) {
    i8259A_out(i8259A_ICW3(icw3), base | i8259A0_ICW3);
}

static inline void i8259A_icw4(uint8_t icw4, uint16_t base) {
    i8259A_out(i8259A_ICW4(icw4), base | i8259A0_ICW4);
}

/* ##################
 * # OCW FUNCTIONS: #
 * ################## */

static inline void i8259A_ocw1_wr(uint8_t mask, uint8_t ocw1, uint16_t base) {
    i8259A_out(i8259A_OCW1(ocw1 & mask), base | i8259A0_OCW1);
}

static inline uint8_t i8259A_ocw1_rd(uint16_t base) {
    i8259A_in(base | i8259A0_OCW1);
}

static inline void i8259A_ocw2_wr(uint8_t mask, uint8_t ocw2, uint16_t base) {
    i8259A_out(i8259A_OCW2(ocw2 & mask), base | i8259A0_OCW2);
}

static inline void i8259A_ocw3_wr(uint8_t mask, uint8_t ocw3, uint16_t base) {
    i8259A_out(i8259A_OCW3(ocw3 & mask), base | i8259A0_OCW3);
}

static inline uint8_t i8259A_ocw3_rd(uint16_t base) {
    return i8259A_in(base | i8259A0_OCW3);
}

/* #########################
 * # I/O HELPER FUNCTIONS: #
 * ######################### */

static inline uint8_t i8259A_in(uint16_t port) {
    i8259A_wait();
    return inb(port);
}

static inline void i8259A_out(uint8_t val, uint16_t port) {
    i8259A_wait();
    outb(val, port);
}

static inline void i8259A_wait(void) {
    /* TODO: replace with something more sophisticated and more
     * precise! */
    __asm__ volatile(
    "jmp 1f\n\t"
    "1:\n\t"
    "jmp 1f\n\t"
    "1:\n\t"
    );
}

#endif /* __ASSEMBLER__ != 1 */

#endif /* i8259A_H */
