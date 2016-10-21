/*
 * The boot system shall be loaded somewhere before `0x90000' (right before the
 * EBDA, but still keeping a safety distance) to leave the max. amount of
 * memory to the kernel, which shall be loaded to `0x0' and from there have
 * contiguous memory at hand, right up to the boot system. I wanted to have
 * the boot system at an arbitrary size. However, this requires some
 * abstraction from segmentation because if the boot system exceeds 64 KiB of
 * size, it needs to switch segment addresses at some point. To avoid
 * hard-wiring that into the code and keeping it flexible, I came up with
 * several m4 macros, which determine the segment address and the offset of an
 * address. That wasn't hard to write since we know the boot system
 * destination and we just need to add that and do `>> 0x4' or `& 0xf' to get
 * the address parts. However, the address of a symbol cannot be resolved by
 * the mere m4 preprocessor and the linker cannot store shift and AND operations
 * in ELF format (http://stackoverflow.com/questions/31360888/
 * how-to-do-computations-with-addresses-at-compile-linking-time). Neither can I
 * tell the compiler and the linker that the compile-time addresses are also
 * the absolute addresses. Therefore, I have decided to switch to a simpler
 * concept: the segment address of the boot system is fixed, allowing only 64
 * KiB, but avoiding the aforementioned complications. In case I'll ever need
 * more than 64 KiB I'll need to find some way to use the former mechanism.
 */

ifdef(`BOOT_H',,dnl
define(`BOOT_H')dnl
define(`BOOT_TEXT', `0x7c00')dnl
define(`BOOT_SIZE', `0x200')dnl
dnl
define(`BOOTSYS_SIZE', `0x10000')
define(`BOOTSYS_SEG', `((0x90000 - BOOTSYS_SIZE) >> 0x4)')dnl
/* `BOOTSYS_OFF' is `0x0'. The boot system is aligned to segment boundaries. */
dnl
)dnl
