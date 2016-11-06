ifdef(`MM_DETECT_H',,


define(`E820_SMAP', `0x534d4150') /* "SMAP" in ASCII */

define(`E820_AVAILABLE', `0x1')
define(`E820_RESERVED', `0x2')
/* TODO: when youve implemented acpi (like, in 500 years) add functionality
 * for acpi reclaim memory and acpi nvs memory. */
define(`E820_ACPI_RECLAIM', `0x3')
define(`E820_ACPI_NVS', `0x4')


define(`E820_MAX_ENTRIES', `0x20')

define(`E820_BASE_ADDR_LEN', `0x8')
define(`E820_LEN_LEN', `0x8')
define(`E820_TYPE_LEN', `0x4')
define(`E820_ENTRY_LEN', `((E820_BASE_ADDR_LEN + E820_LEN_LEN + dnl
E820_TYPE_LEN) * E820_MAX_ENTRIES)')

define(`E820_SUCCESS', `0x0') /* success (must be zero) */
define(`E820_TOO_MANY', `0x1') /* too many entries */

) /* MM_DETECT_H */
