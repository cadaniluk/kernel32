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

