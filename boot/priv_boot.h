#ifndef BOOT_H
#define BOOT_H

/* Necessary bootloader parts' sizes: */

#define BOOT_CODE_SIZE 0x200
#define BOOT_PARAMS_SIZE 0x100



/* Video mode-specific constants: */

/* grayscale 80x25 (i.e. 640x200) resolution; 16 colors */
#define BOOT_VIDEO_MODE 0x02
#define STD_CHAR_ATTR 0x0f



/* Segment selectors: */

#define BOOT_CODE 0x08
#define BOOT_DATA 0x10 



/* Location of arguments passed to kernel within the BDA : */

#define BDA_NUM_COLS 0x44a
#define BDA_VIDEO_BASE_PORT 0x463

/* Offsets of arguments passed to kernel and other stuff related to that: */

#define BOOT_PARAMS_BASE 0x300	/* whole bootloader size (0x400) -
				 * BOOT_PARAMS_SIZE */
#define NUM_COLS (BOOT_PARAMS_BASE + 0x00)	/* number of text mode
						 * columns */
#define VIDEO_BASE_PORT (BOOT_PARAMS_BASE + 0x02)	/* video card
							 * base I/O
							 * port */
#define MEMORY_MAP (BOOT_PARAMS_BASE + 0x04)
#define MAGIC_SMAP 0x534d4150	/* string "SMAP" without null byte */



/* Miscellaneous constants: */

#define KERNEL_START 0x500	/* TODO: change this to 0x0 */

#define KERN_SECTORS 0x20

#define A20_SEG 0xffff
#define A20_OFF 0x10
#define FREESTORE 0xa000

#define RELOC_SP 0xfffe	/* not 0xffff to keep stack aligned */

/* Actually, this header file is only intended for usage within boot.S (that is,
 * an x86 Assembly file) but if someone uses it in C anyway, we're safe. */
#ifdef ASSEMBLY

/* Note that ax, bx, cx, dx, and bp are undefined after executing this macro.
 * es is expected to be set by preceding code. */
.macro print_str str_addr, str_len
	/* Find out cursor position. */
	mov $0x03, %ah
	xor %bh, %bh
	int $0x10

	/* Print actual string. */
	mov $0x1301, %ax
	mov $STD_CHAR_ATTR, %bx
	mov \str_len, %cx
	/* dx set above by ISR for determining the cursor position */
	mov \str_addr, %bp
	int $0x10
.endm

/* I hope two instructions on one line won't have a negative impact on
 * debugging... */
#define hlt_cpu cli; hlt /* prevent interruption of hlt instruction */

#endif

#endif
