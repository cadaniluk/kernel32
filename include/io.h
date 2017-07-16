#ifndef __IO_H
#define __IO_H

#include <stdint.h>

static inline uint8_t inb(uint8_t addr)
{
	uint8_t data;
	__asm__("inb %1, %0" : "=a" (data) : "Nd" (addr));
	return data;
}

static inline void outb(uint8_t addr, uint8_t data)
{
	__asm__("outb %0, %1" :: "a" (data), "Nd" (addr));
}

static inline uint16_t inw(uint16_t addr)
{
	uint16_t data;
	__asm__("inb %1, %0" : "=a" (data) : "Nd" (addr));
	return data;
}

static inline void outw(uint16_t addr, uint16_t data)
{
	__asm__("outb %0, %1" :: "a" (data), "Nd" (addr));
}

#endif