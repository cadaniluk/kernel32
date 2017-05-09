ifdef(`i8259A_H',,
define(`i8259A_H')

/* #####################
 * # COMMON I/O PORTS: #
 * ##################### */

define(`i8259A_SLAVE', `0x20')
define(`i8259A_MASTER', `0xa0')

define(`USER_INT_START', `0x20')

/* #########
 * # ICW1: #
 * ######### */

define(`i8259A_ICW1', `$1 | 0x10')
define(`i8259A_ICW1_ADDR', `$1')

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
/* TODO: originally, the i8259A_ICW2 macro was `$1 << 0x3'. is the shift correct
 * here? i removed it because i dont think it is... */
define(`i8259A_ICW2', `$1')
define(`i8259A_ICW2_ADDR', `$1 | 0x1')

/* #########
 * # ICW3: #
 * ######### */

define(`i8259A_ICW3', `$1')
define(`i8259A_ICW3_ADDR', `$1 | 0x1')

/* #########
 * # ICW4: #
 * ######### */

define(`i8259A_ICW4', `$1')
define(`i8259A_ICW4_ADDR', `$1 | 0x1')

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
define(`i8259A_MCS', `0x0')
define(`i8259A_x86', `0x1')

/* #########
 * # OCW1: #
 * ######### */

/* OCW1 accesses the IMR. Set bit means "masked off" or "IR line disabled,"
 * cleared bit means "IR line enabled." */

define(`i8259A_OCW1', `$1') /* for consistency's sake */
define(`i8259A_OCW1_ADDR', `$1 | 0x1')

/* #########
 * # OCW2: #
 * ######### */

/* For consistency's sake once again... this makes me question the
 * consistency; is it still consistency if the stubs for consistency
 * constitute the majority of overall functions? I don't think so but
 * this file hasn't reached this vast extent (yet). */
define(`i8259A_OCW2', `$1')
define(`i8259A_OCW2_ADDR', `$1')

/* An IR level encoded in bits `0' - `2' in OCW2. Used only with certain
 * commands. */
define(`i8259A_OCW2_IR', `$1')

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

define(`i8259A_OCW3', `$1 | 0x8')
define(`i8259A_OCW3_ADDR', `$1')

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

