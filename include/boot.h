#ifndef BOOT_H
#define BOOT_H

/* We assume a sector is 512 bytes. That's the standard. */
/* TODO: make this more flexible by reading from cmos or another source that
 * provides information about the sector size, if any. */
#define SECTOR_SIZE 0x200

/* TODO: make more flexible by reading from cmos! */
/* Format of a 1.44 MiB floppy. */
#define CYLINDERS 80
#define HEADS 2
#define SECTORS 18

/*
 * $1 - Heads per cylinder
 * $2 - Sectors per track
 * $3 - Cylinder number
 * $4 - Head number
 * $5 - Sector number (starts at 1)
 */
define(`CHS_TO_LBA', `eval(`(($3 * $1 + $4) * $2) + $5 - 1')')dnl

/*
 * $1 - Heads per cylinder
 * $2 - Sectors per track
 * $3 - LBA value
 */
define(`LBA_TO_C', `eval(`$3 / ($1 * $2)')')dnl

/*
 * $1 - Heads per cylinder
 * $2 - Sectors per track
 * $3 - LBA value
 */
define(`LBA_TO_H', `eval(`($3 % ($1 * $2)) / $2')')dnl

/*
 * $1 - Heads per cylinder
 * $2 - Sectors per track
 * $3 - LBA value
 */
define(`LBA_TO_S', `eval(`($3 % ($1 * $2)) % $2 + 1')')dnl

#define DEF_ATTR 0xf /* default */
#define OK_ATTR 0x2f /* for successful operations */
#define ERR_ATTR 0x4f /* for errors */

#define TMP_C0_SEL 0x8
#define TMP_D0_SEL 0x10

#endif
