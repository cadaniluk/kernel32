/* Sources: 11, 12 */

/* The IR number goes under several different names.
 * I've heard so far "type," "code," and "level," so don't get all too confused
 * when reading datasheets and this code. */

/* Since there can be multiple PICs in the machine, we cannot write the
 * driver explicitly for just one controller with one set of ports and all.
 * To support multiple PICs we distinguish them using the I/O base address
 * (usually `0x20' for master and `0xa0' for slave) and pass these base
 * addresses to the functions. */

/* TODO: I HAVEN'T UNDERSTOOD YET WHAT THE A0 PIN IS FOR IN THE i8259A.
 * SEE THIS POST
 * http://electronics.stackexchange.com/questions/225139/ \
 * intel-8259a-why-use-a0-when-theres-nothing-to-address
 * FOR MY QUESTIONS. I NEED TO FIGURE THAT OUT TO REALLY WRITE A GOOD DRIVER!
 * SO DO IT! FIGURE IT OUT! ASAP!!! */

/* The Intel 8259A can be used together with the 8086 (and therefore the x86
 * line, since all x86 processors are backward-compatible to the 8086) and the
 * MCS-80 and MCS-85 processors, which constitute some other Intel processor
 * line. Those MCS processors are of no interest to us, though. That means that
 * the Intel 8259 driver files describe the x86 line only. */

/* #################################
 * # REGISTERS (WORTH MENTIONING): #
 * ################################# */

/* IMR (Interrupt Mask Register); 8-bit:
 * When the controller receives an interrupt, the IMR is checked first. If
 * the corresponding bit is cleared the interrupt is allowed. If set, it is
 * "masked off," thus making the controller discard it.
 *
 * IRR (Interrupt Request Register); 8-bit:
 * One for every interrupt line (bit 0: IR0, bit 1: IR1, ...,
 * bit 7: IR7). A set bit indicates an interrupt request.
 *
 * ISR (In-Service Register); 8-bit:
 * After interrupt requests have been received in the IRR, the priority
 * resolver figures out which one has the highest priority. The
 * highest priority one has its IRR bit cleared and its ISR bit set in order to
 * indicate it is being serviced. */

/* ##############
 * # CASCADING: #
 * ############## */

/* Multiple Intel 8259A chips can be "cascaded," i.e., they can be tied
 * together, so that more than just eight IRs can be handled as with just a
 * single controller. In cascading mode, there is one "master" PIC, the others
 * are so-called "slave" PICs. The slaves' #INT line is connected to the
 * IR lines of the master. They can issue interrupts and acknowledge them
 * just like the master controller. The master uses the CAS0 - CAS2 lines to
 * send a #CS (chip select) signal to the slave, so the IR code can be put
 * on the data bus. */

/* #######################
 * # INTERRUPT SEQUENCE: #
 * ####################### */

/* 1. Interrupt request (IR) lines are raised, corresponding bits in the IRR are
 *    set.
 * 2. The controller evaluates the requests, checks the IMR, if not masked off,
 *    sets the appropriate bit in the IRR, and raises the #INT line to the CPU.
 * 3. The CPU acknowledges the #INT and raises the #INTA line in return.
 * 4. If the raised #INTA is processed, the highest priority interrupt request
 *    is determined (over the priority resolver) and the appropriate bits of
 *    that IR are cleared/set in the IRR and ISR. All lower-priority interrupts
 *    are inhibited while the ISR bit is set. For information on
 *    higher-priority interrupts, read NOTE 2.
 * 5. The CPU raises the #INTA line again and the controller sends an 8-bit
 *    pointer with the interrupt code in return.
 * 6. AEOI (automatic EOI) mode: the ISR bit is cleared.
 *    EOI (end of interrupt) mode: the ISR bit remains set until the
 *    appropriate EOI command is issued by the CPU, that is, by the interrupt
 *    handler.
 * NOTE 1: if the IR could not be detected anymore at step 4 (because the IR's
 * duration was too short), IR7 will be assumed (spurious interrupt). On a
 * spurious interrupt, a default interrupt will be issued: the IR7 handler will
 * be executed but the ISR bit will not be set. If a normal IR7 interrupt
 * is fired, the ISR bit is set, though. However, if a default IR7 interrupt
 * is fired during a normal IR7 handler, the ISR bit will stay set for
 * the default IR7 interrupt as well. Code for detection of spurious interrupts
 * needs to pay attention to this quirk.
 * NOTE 2: if a higher priority IR occurs between the first and second #INTA
 * signals, the #INT line goes inactive right after the second #INTA raise and
 * after an UNSPECIFIED time period, the #INT goes active again to
 * signalize the higher priority IR.
 */

/* ##################
 * # ICWS AND OCWS: #
 * ################## */

/* A sequence of Initialization Control Words (ICWs) is used to initialize
 * each of the PICs in the system (there may be several, cascaded ones,
 * remember). In fact, the sequence consists of 2 - 4 ICWs.
 * Operation Control Words (OCWs) determine the interrupt mode the controller
 * operates in. The following modes are eligible for selection:
 * - Fully nested mode
 * - Rotating priority mode
 * - Special mask mode
 * - Polled mode
 * OCWs can be sent to the controller anytime after initialization thereof.
 */

ifdef(`i8259A_H',,
define(`i8259A_H')

/* #####################
 * # COMMON I/O PORTS: #
 * ##################### */

/* Master PIC base I/O port. */
define(`i8259A_MBASE', `0x20')


/* None of the original IBM PCs had more than two PICs but I don't think
 * it's prohibited to have more on an x86 machine. Therefore, I wrote the
 * driver to be compliant for an arbitrary number of PICs, without being
 * restricted to just one master and one slave.
 * Every Intel 8259A chip in the system requires separate initialization. */

/* #########
 * # ICW1: #
 * ######### */

define(`i8259A_ICW1', `(0x10 | ($1))')
define(`i8259A_ICW1_ADDR', `($1)')

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
define(`i8259A_LTIM', `0x8')

/* ADI (Address Interval) at `0x4' is not used on x86, so setting that bit has
 * no effect. */

/* 0 - Multiple PICs in the system.
 * 1 - One PIC in the system; no ICW3.
 * The Intel handbook denotes this bit as "SNGL." I think the name I used here
 * is easier to understand and just as short, so I decided to use that
 * instead. */
define(`i8259A_SINGLE', `0x2')

/* 0 - No ICW4.
 * 1 - ICW4 will be read.
 * The Intel handbook denotes this bit as "IC4." I would've changed it
 * to "ICW4" for better readability but that would crash with the `i8259A_ICW4'
 * macro used to generate the ICW4. */
define(`i8259A_IC4', `0x1')

/* #########
 * # ICW2: #
 * ######### */

/* Generates ICW2. `base' contains bits 3 - 7 of the interrupt vector (IV) of
 * the interrupt requests (IR). The code of the IR (0 - 7) consumes bits 0 - 2.
 * `base' and the code constitute the IV. */
define(`i8259A_ICW2', `(($1) << 0x3)')
define(`i8259A_ICW2_ADDR', `(($1) | 0x1)')

/* #########
 * # ICW3: #
 * ######### */

define(`i8259A_ICW3', `($1)')
define(`i8259A_ICW3_ADDR', `(($1) | 0x1)')

/* #########
 * # ICW4: #
 * ######### */

define(`i8259A_ICW4', `($1)')
define(`i8259A_A0_ICW4', `(($1) | 0x1)')

/* 0 - special fully nested mode
 * 1 - not special fully nested mode */
define(`i8259A_SFNM', `0x10')

/* 0 - Buffered mode disabled.
 * 1 - Buffered mode enabled. */
define(`i8259A_BUF', `0x8')

/* 0 - Slave PIC.
 * 1 - Master PIC.
 * If `i8259A_BUF' is set, this bit indicates whether the PIC is a master or
 * slave. If `i8259A_BUF' is clear, it has no function. */
define(`i8259A_MS', `0x4')

/* 0 - Automatic end of interrupt is disabled.
 * 1 - Automatic end of interrupt is enabled.
 * AEOI will perform a non-specific EOI command at the second
 * #INTA pulse. Should only be used when a nested multilevel interrupt
 * structure is not needed within a single PIC.
 * TODO: find out what that "nested multilevel..." means!
 * Pre-1985 8259A chips can only use AEOI as a master. Later controllers
 * can operate with AEOI as master or slave. */
define(`i8259A_AEOI', `0x2')

/* 0 - Intel MCS processor used.
 * 1 - x86 processor used. */
define(`i8259A_uPM', `0x1')

/* #########
 * # OCW1: #
 * ######### */

/* OCW1 accesses the IMR. Set bit means "masked off" or "IR line disabled,"
 * cleared bit means "IR line enabled." */

define(`i8259A_OCW1', `($1)') /* for consistency's sake */
define(`i8259A_OCW1_ADDR', `(($1) | 0x1)')

/* #########
 * # OCW2: #
 * ######### */

/* For consistency's sake once again... this makes me question the
 * consistency; is it still consistency if the stubs for consistency
 * constitute the majority of overall functions? I don't think so but
 * this file hasn't reached this vast extent (yet). */
define(`i8259A_OCW2', `($1)')
define(`i8259A_OCW2_ADDR', `($1)')

/* An IR level encoded in bits `0' - `2' in OCW2. Used only with certain
 * commands. */
define(`i8259A_OCW2_IR', `($1)')

/* Non-specific EOI command.
 * Clears the current highest priority interrupt level since, in fully
 * nested mode, this must be the last one acknowledged and serviced.
 * Useful in fully nested mode. */
define(`i8259A_NEOI', `0x20')

/* Specific EOI command.
 * Clears a specific interrupt level specified in bits 0 - 2 (see
 * the `i8259A_OCW2_IR' macro).
 * Useful in modes other than fully nested. */
define(`i8259A_SEOI', `0x60')

/* Rotate on non-specific EOI command.
 * The priorites are rotated in such a manner that the IR chosen by the
 * non-specific EOI command gets assigned the lowest priority (priority
 * level 7).
 * Used if the devices have equal priority. */
define(`i8259A_ROTNEOI', `0xa0')

/* Rotate on specific EOI command (in conjunction with IR level). */
define(`i8259A_ROTSEOI', `0xe0')

/* Rotate in AEOI mode (set). */
define(`i8259A_ROTAEOIS', `0x80')

/* Rotate in AEOI mode (clear). */
define(`i8259A_ROTAEOIC', `0x0')

/* Set priority (in conjunction with IR level).
 * The priority level set is rotated, so that the priority level specified
 * in bits 0 - 2 of OCW2 is set as the new bottom priority level. */
define(`i8259A_SETPR', `0xc0')

/* No-op.
 * TODO: remove if superflous! */
define(`i8259A_NOP', `0x40')

/* #########
 * # OCW3: #
 * ######### */

define(`i8259A_OCW3', `(($1) | 0x8)')
define(`i8259A_OCW3_ADDR', `($1)')

/* Notice something? While I wrote the single commands of OCW2 as macros,
 * I wrote the single bits of OCW3. That's simply because the manual did
 * not describe the abbr. meanings of the OCW2 bits sufficiently but did so
 * with the commands. I simply could not detect a pattern, so I decided to
 * write out the commands directly. */

/* 0 - Do not change mask mode; `i8259A_SMM''s value is unimportant.
 * 1 - Enable special mask mode (ESMM). */
define(`i8259A_ESMM', `0x40')

/* 0 - Normal mask mode.
 * 1 - Special mask mode (SMM).
 * Some interrupt handlers may want to enable some other interrupts during
 * execution. However, all lower-priority IRs are disabled as long as no EOI
 * occured, so other IRs cannot be enabled.
 * SMM helps here: it inhibits interrupts at the IR bits set in the IMR and
 * enables interrupts from all other levels, be it higher or lower. */
define(`i8259A_SMM', `0x20')

/* 0 - No poll command
 * 1 - Poll command */
define(`i8259A_POLL', `0x4')

/* 0 - Do not read any register
 * 1 - Read a register
 * The abbreviation used in the reference is "RR." I figured it stands for
 * "read register," so I elaborated on that abbreviation a bit and came up with
 * the lower name. No assurance, though. */
define(`i8259A_RDREG', `0x2')

/* 0 - Read IRR
 * 1 - Read ISR
 * The abbreviation used in the reference is "RIS." I figured it stands for
 * "read in-service," so I elaborated on that abbreviation a bit and came up
 * with the lower name. No assurance, though. */
define(`i8259A_RDISR', `0x1')

) /* i8259A_H */

