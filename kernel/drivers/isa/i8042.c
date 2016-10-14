#include <drivers/i8042.h>

/* Initializes the Intel 8042 driver.
 * Returns `0' on success, a distinct error code on failure. */
i8042_err_t i8042_init(void) {
    /* Disable, so the initialization will not be disrupted. */
    i8042_disable();

    /* Flush the output buffer, just to be sure. The output buffer is checked
     * for data availability first to avoid any unexpected behavior. */
    if (i8042_instat() & i8042_STAT_OUTFULL)
        (void) i8042_indata();	/* read and discard */

    /* Alter the command byte somewhat. Clear `i8042_CMD_INT',
     * `i8042_CMD_PCMODE', and `i8042_CMD_PC_COMPAT' bits. */
    i8042_setcmdbyte(i8042_cmdbyte() &
    ~(i8042_INT | i8042_PCMODE | i8042_PC_COMPAT));

    /* Doing the tests at the beginning of the function would be better
     * for performance because the function could return earlier in case of
     * failure but maybe the other initialization stuff changes the outcomes
     * of those tests, so I added them last in the end. */

    /* Perform internal self-tests. */
    if (i8042_selftest() != i8042_SELFT_NOERR)
        return E_i8042_SELFTEST;
    
    /* Perform interface tests, that is, test clock and data lines. */
    if (i8042_iftest() != i8042_IFT_NOERR)
        return E_i8042_IFTEST;

    return E_i8042_NOERR;
}


/* TODO: no `i8042_destroy' function so far... add if necessary! */
