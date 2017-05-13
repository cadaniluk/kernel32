ifdef(`MM_DETECT_H',,


define(`E820_SMAP', `0x534d4150') /* "SMAP" in ASCII */

define(`E820_AVAILABLE', `0x1')
define(`E820_RESERVED', `0x2')
/* TODO: when youve implemented acpi (like, in 500 years) add functionality
 * for acpi reclaim memory and acpi nvs memory. */
define(`E820_ACPI_RECLAIM', `0x3')
define(`E820_ACPI_NVS', `0x4')

define(`E820_MAX_ENTRIES', `0x20')
define(`E820_ENTRY_LEN', `0x14')

define(`E820_BASE_ADDR', `0x0')
define(`E820_LEN', `0x8')
define(`E820_TYPE', `0x10')

define(`E820_SUCCESS', `0x0') /* success (must be zero) */
define(`E820_TOO_MANY', `0x1') /* too many entries */
define(`E820_BAD_SMAP', `0x2') /* SMAP value not set properly */

) /* MM_DETECT_H */
