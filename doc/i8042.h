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
