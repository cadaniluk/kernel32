#ifndef IO_H
#define IO_H

#include <types.h>

/* TODO: maybe write a macro like out, from which you can derive all other
 * out functions. same for in. */

/* I/O address space in/out operations. */

#if __ASSEMBLER__ != 1

static inline void io_outb(uint8_t val, uint16_t port) {
    __asm__ volatile("out %0, %1" :: "a" (val), "dN" (port));
}

static inline uint8_t io_inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile("in %1, %0" : "=a" (val) : "dN" (port));
    return val;
}

static inline void io_outw(uint8_t val, uint16_t port) {
    __asm__ volatile("out %0, %1" :: "a" (val), "dN" (port));
}

static inline uint16_t io_inw(uint16_t port) {
    io_port_t val;
    __asm__ volatile("in %1, %0" : "=a" (val) : "dN" (port));
    return val;
}

static inline void io_outl(uint32_t val, uint16_t port) {
    __asm__ volatile("out %0, %1" :: "a" (val), "dN" (port));
}

static inline uint32_t io_inl(uint16_t port) {
    uint32_t val;
    __asm__ volatile("in %1, %0" : "=a" (val) : "dN" (port));
    return val;
}

#endif /* __ASSEMBLY__ != 1 */

#endif /* IO_H */
