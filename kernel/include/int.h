/* The difference between this file and the interrupt controller
 * driver files is that this file contains the interface the CPU or
 * other devices see while the driver files contain specifically
 * the side of the controllers. This distinction may seem subtle but
 * both interfaces serve different purposes, so it is crucial to
 * the easy usability of the header files. */

#ifndef INT_H
#define INT_H

#include <types.h>

typedef uint8_t int_code_t;

/* TODO: I am not sure if these codes are really fixed for all x86 systems
 * or if they are just the "normal" configuration. If latter applies,
 * I could remove the lower codes altogether and determine the codes at
 * runtime (maybe look them up in CMOS registers or something. */
 *  
/* Interrupt request (IRQ) codes of connected devices. */
/* TODO: add IRQ codes here! */

/* TODO: find out if unnamed union member fields exist in either the
 * C11 standard or as a GCC extension. If yes, then leave the file as-is.
 * If not, name the currently unnamed variables in the unions. */
/* I could've written a macro to define the `union's but that would've
 * exposed the macros to the user (after all, it is in a header file), which I
 * cannot allow without any restrictions. The namespace is polluted enough and
 * those macros will do more harm than benefit.
 */
struct int_regs32 {
	/* General-purpose registers. */
	union {
		uint16_t;
		union {
			uint8_t al;
			uint8_t ah;
		} ax;
	} eax;
	union {
		uint16_t;
		union {
			uint8_t cl;
			uint8_t ch;
		} cx;
	} ecx;
	union {
		uint16_t;
		union {
			uint8_t dl;
			uint8_t dh;
		} dx;
	} edx;
	union {
		uint16_t;
		union {
			uint8_t bl;
			uint8_t bh;
		} bx;
	} ebx;

	union {
		uint16_t;
		uint16_t si;
	} esp;
	union {
		uint16_t;
		uint16_t si;
	} ebp;
	union {
		uint16_t;
		uint16_t si;
	} esi;
	union {
		uint16_t;
		uint16_t di;
	} edi;
};

/* AMENDMENT TO SENT PROPOSAL EMAIL:
 * #6 Typedef a struct only if it is really a type like `vertex_t`. If it
 * is just a container like `struct int_regs32', leave it a struct and give it
 * a pluralized name! */
 
void int_issue(uint8_t code, struct int_regs32* iregs, struct int_regs32*
oregs); 

#endif
