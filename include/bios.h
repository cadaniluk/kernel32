/* TODO: remove if not necessary. */
/* TODO: first of all, do not remove this file without further thought! decide
 * between the register definitions here in bios.h and in int.h and remove
 * the one you do not want to keep. */  

#ifndef BIOS_H
#define BIOS_H

#include <types.h>
#include <int.h>

/* TODO: the code for defining the registers was supposed to be for BIOS
 * calls but turned out to be superflous; I'm pretty sure it's gonna be
 * useful somewhere else in the system, though, so keep it and think about
 * its usage. It's currently disabled with `#if 0 ... #endif' macros. That's
 * on purpose. */

/* TODO: VERY IMPORTANT: UPDATE THE LD SCRIPT AND PUT EVERY CODE & DATA
 * INTO APPROPRIATE SECTIONS!!! */

/* For defining the "X GPRs," namely `eax', `ecx', `edx', and `ebx'. */
#define GPRX32(id) \
    union { \
        uint16_t e##id##h; \
        union { \
            uint8_t id##h; \
            uint8_t id##l; \
        } id##x; \
    } e##id##x

/* For defining the "Other GPRs," namely `esp', `ebp', `esi', `edi'. */
#define GPRO32(id)

/* 32-bit general-purpose registers. */
typedef struct gprs32 {
    GPRX32(a); /* accumulator */
    GPRX32(c); /* counter */
    GPRX32(d); /* data */
    GPRX32(b); /* base */

    GPRO32(sp); /* stack pointer */
    GPRO32(bp); /* base pointer */
    GPRO32(si); /* source index */
    GPRO32(di); /* destination index */
} gprs32_t;

#endif
