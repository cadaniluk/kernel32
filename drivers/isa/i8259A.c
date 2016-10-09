#include <drivers/i8259A.h>

/* IRR (0x0) is default. */
uint8_t rd_reg = 0x0;

void i8259A_init(uint8_t icw1, uint8_t icw2, uint8_t icw3, uint8_t icw4,
uint16_t base) {
    i8259A_icw1(icw1, base);
    i8259A_icw2(icw2, base);
    if (!(icw2 & i8259A_SINGLE))
        i8259A_icw3(icw3, base);
    if (icw3 & i8259A_ICW4)
        i8259A_icw4(icw4, base);
}
