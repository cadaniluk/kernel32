#ifndef uPD765_H
#define uPD765_H

/* TODO: How to handle multi-bit fields? with a bit mask or simply provide
 * possible values? example: drive selection. */

#define uPD765_STATUS 0x34f


/* Bits in the main status register. (Names taken directly from the IBM XT
 * manual.) */
/* TODO: add doc about clear bits when having looked up what seek mode is! */

/* 1 - Drive A is in Seek mode. */
#define uPD765_DAB 0x1

/* 1 - Drive B is in Seek mode. */
#define uPD765_DBB 0x2

/* 1 - Drive C is in Seek mode. */
#define uPD765_DCB 0x4

/* 1 - Drive D is in Seek mode. */
#define uPD765_DDB 0x8

/* 0 - Controller is idle.
 * 1 - Controller is busy. */
#define uPD765_CB 0x10

/* 0 - Controller is in DMA mode.
 * 1 - Controller is in non-DMA mode. */
#define uPD765_NDM 0x20

/* 0 - Data is transferred from the processor to the controller's data
 * register.
 * 1 - Data is transferred from the controller's data register to the
 * processor. */
#define uPD765_DIO 0x40

/* 0 - Data register is not ready to exchange data with the CPU.
 * 1 - Data register is ready to exchange data with the CPU. */
#define uPD765_RQM 0x80

/* TODO: continue by reading the pc xt manual about the floppy disk controller
 * and its commands! there are 15 of them i heard. */

/* Bits in the digital-output register (DOR). */

/* Bit mask for drive A. */
#define uPD765_DRV_A 0x0

/* 0 - Controller disabled.
 * 1 - Controller enabled. */
#define uPD765_ENABLE 0x4

/* 0 - TODO: add doc! something with io interface. */
#define uPD765_IO 0x8

/* 0 - Drive's associated motor is off.
 * 1 - Drive's associated motor is on. */
#define uPD765_DRV_A_ON 0x10
#define uPD765_DRV_B_ON 0x20
#define uPD765_DRV_C_ON 0x40
#define uPD765_DRV_D_ON 0x80

#endif