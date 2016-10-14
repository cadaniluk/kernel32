ifdef(`APM_H', , `define(`APM_H')

/* This file contains all constants necessary to work with APM. */

/* TODO: split the file up into BIOS-specific and general APM constants or
 * leave it as is? ask yourself that! */ 

/* APM BIOS interrupt code. */
define(`APM_INT', `0x15')

/* APM BIOS function IDs. */
define(`APM_INSTALLCHECK', `0x00')
define(`APM_IF_CONN_RM', `0x1')
define(`APM_IF_CONN_PM16', `0x2')
define(`APM_IF_CONN_PM32', `0x3')
define(`APM_IF_DISCONN', `0x4')
define(`APM_CPU_IDLE', `0x5')
define(`APM_CPU_BUSY', `0x6')
define(`APM_SET_POW_STAT', `0x7')
define(`APM_POW_MANAG_SET_SYS', `0x8')
define(`APM_REST_DEF', `0x9') /* Restore APM BIOS Power-On Defaults */
define(`APM_POW_STAT_SYS', `0xa')
define(`APM_PM_EVENT', `0xb')
define(`APM_POW_STAT_DEV', `0xc')
define(`APM_POW_MANAG_SET_DEV', `0xd')
define(`APM_DRIVER_VERSION', `0xe')
define(`APM_POW_MANAG_SET', `0xf') /* Engage/Disengage Power Management */
define(`APM_CAP', `0x10') /* Get Capabilities */
define(`APM_RES_TIMER', `0x11') /* Get/Set/Disable Resume Timer */
define(`APM_SET_RES_RIND', `0x12') /* Enable/Disable Resume on Ring Indicator */
define(`APM_BIOS_SET_TIMER_REQS', `0x13') /* Enable/Disable Timer Based
                                           * Requests */

/* OEM-defined APM functions. */
define(`APM_OEM_INSTALLCHECK', `0x80')
define(`APM_OEM_FUNC', `0x80')

/* APM function-specific constants. */

/* Installation Check. */
define(`APM_PM', `0x504d') /* the value of `ax' on successful return from
                            * the Installation Check function */

/* Bitmasks for returned information from Installation Check function. */
define(`APM_IF_PM16', `0x1') /* 16-bit Protected Mode interface supported */
define(`APM_IF_PM32', `0x2') /* 32-bit Protected Mode interface supported */
define(`APM_IDLE_SPEED', `0x4') /* CPU Idle mode reduces CPU speed */
define(`APM_BIOS_OFF', `0x8') /* APM BIOS disabled */
define(`APM_BIOS_DISENG', `0x10') /* APM BIOS disengaged (since v1.1) */
/* The rest of bits is reserved. */

/* Set Power State.
 * `0x6' - `0x1f': reserved system states
 * `0x20' - `0x3f': OEM-defined system states
 * `0x40': `0x7f': OEM-defined device states
 * `0x80': `0xffff': reserved device states */  
define(`APM_ON', `0x0') /* APM Enabled (not supported for ID `0x1') */
define(`APM_STANDBY', `0x1') /* Standby */
define(`APM_SUSPEND', `0x2') /* Suspend */
define(`APM_OFF', `0x3') /* Off */
define(`APM_LAST_REQ_PROC', `0x4') /* Last Request Processing Notification
                                    * (only for ID `0x1') */
define(`APM_LAST_REQ_REJ', `0x5') /* Last Request Rejected (only for ID
                                   * `0x1') */

/* Enable/Disable Power Management. */

/* Disable or enable. */
define(`APM_DISABLE', `0x0')
define(`APM_ENABLE', `0x1')

/* Get Power Status. */

/* TODO: since the battery unit numbers start at 1, maybe add some macro to
 * help enforce that or something similar... */

/* AC line status. */
define(`APM_AC_OFF', `0x0') /* Off-line */
define(`APM_AC_ON', `0x1') /* On-line */
define(`APM_AC_BACKUP', `0x2') /* On backup power */
define(`APM_AC_UNKNOWN', `0xff') /* Unknown
                                  * NOTE: this is a value, not a bitmask! */
/* All other AC line status values are reserved. */

/* Battery status. */
define(`APM_BATT_HIGH', `0x0') /* High */
define(`APM_BATT_LOW', `0x1') /* Low */
define(`APM_BATT_CRITIC', `0x2') /* Critical */
define(`APM_BATT_CHARGE', `0x3') /* Charging */
define(`APM_BATT_UNKNOWN', `0xff') /* Unknown
                                    * NOTE: this is a value, not a bitmask! */
/* All other battery status values are reserved. */

/* Battery flag bitmasks. */
define(`APM_BATTF_HIGH', `0x1') /* High */
define(`APM_BATTF_LOW', `0x2') /* Low */
define(`APM_BATTF_CRITIC', `0x4') /* Critical */
define(`APM_BATTF_CHARGE', `0x8') /* Charging */
define(`APM_BATTF_BATT_NA', `0x10') /* Selected battery not present */
define(`APM_BATTF_BATT_NA_SYS', `0x80') /* No system battery */
define(`APM_BATTF_UNKNOWN', `0xff') /* Unknown
                                     * NOTE: this is a value, not a bitmask! */
/* All other battery flag bitmask values are reserved. */

/* Remaining battery life - percentage of charge. */
define(`APM_BATT_CHARGE_UNKNOWN', `0xff') /* Unknown
                                           * NOTE: this is a value, not a
                                           * bitmask! */

/* Remaining battery life - time units. */
/* `0' - Time units are seconds
 * `1' - Time units are minutes */
define(`APM_BATT_TIME_MINS', `0x8000')
define(`APM_BATT_TIME_UNKNOWN', `0xffff') /* Unknown
                                           * NOTE: this is a value, not a
                                           * bitmask! */

 APM BIOS error codes.
 * `0xe' - `0x1f': reserved for other interface and general errors
 * `0x20' - `0x3f': reserved for CPU errors
 * `0x40' - `0x5f': reserved for device errors
 * `0x61' - `0x7f': reserved for other system errors
 * `0x81' - `0x85' and `0x87' - `0x9f': reserved for other power management
 *          errors
 * `0xa0' - `0xfe': reserved
 * `0xff': undefined function */
/* NOTE: RBIL and the APM v1.2 specification contradict each other here:
 * RBIL defines `0x4' but the spec does not. Of course, the official spec
 * seems more credible but who knows... if anyone knows a solution, tell me! */
/* NOTE: The right comments are the corresponding error messages, verbatim
 * copied from the APM v1.2 specification except for minor changes
 * ("protected mode" to "Protected Mode", e.g.). I understand all messages
 * and they are sensible formulations but what is the matter with that
 * capitalization rules!? */
define(`APM_OFF', `0x1') /* Power Management functionality disabled */
define(`APM_IF_RM_ALRDY', `0x2') /* Real Mode interface connection already
                                  * established */
define(`APM_IF_NO_CONN', `0x3') /* Interface not connected */
define(`APM_IF_PM16_ALRDY', `0x5') /* 16-bit Protected Mode interface already
                                    * established */
define(`APM_IF_PM16_NA', `0x6') /* 16-bit Protected Mode interface not
                                 * supported */
define(`APM_IF_PM32_ALRDY', `0x7') /* 32-bit Protected Mode interface already
                                    * established */
define(`APM_IF_PM32_NA', `0x8') /* 32-bit Protected Mode interface not
                                 * supported */
define(`APM_BAD_DEV_ID', `0x9') /* Unrecognized device ID */
define(`APM_PARAM_OOR', `0xa') /* Parameter value out of range */
define(`APM_IF_NO_ENG', `0xb') /* Interface not engaged (APM v1.1) */
define(`APM_FUNC_NA', `0xc') /* Function not supported (APM v1.2) */
define(`APM_RES_TIMER_OFF', `0xd') /* Resume timer disabled (APM v1.2) */
define(`APM_ENTER_STATE_FAIL', `0x60') /* Unable to enter requested state */
define(`APM_NO_EVENTS', `0x80') /* No power management events pending */
define(`APM_NA', `0x86') /* Reserved - APM not present */

/* TODO: add descriptions to all constants in this header file! */

/* APM event codes.
 * `0xd' - `0xff': reserved system events
 * `0x100' - `0x1ff': reserved device events
 * `0x200' - `0x2ff': OEM-defined APM events
 * `0x300' - `0xffff': reserved */
define(`APM_STANDBY_REQ', `0x1')
define(`APM_SUSPEND_REQ', `0x2')
define(`APM_NORMAL_RES', `0x3')
define(`APM_CRITIC_RES', `0x4')
define(`APM_LOW_BATT', `0x5')
define(`APM_POW_STAT_CHNG', `0x6')
define(`APM_UPDATE_TIME', `0x7')
define(`APM_CRITIC_SUSPEND', `0x8')
define(`APM_USR_STANDBY_REQ', `0x9')
define(`APM_USR_SUSPEND_REQ', `0xa')
define(`APM_STANDBY_RES', `0xb')
define(`APM_CAP_CHNG', `0xc')

/* APM power device class IDs and standardized IDs.
 * `0x700' - `0x7fff', `0x8100' - `0xdfff', and `0xf000' - `0xffff': reserved
 * `0xe000' - `0xefff': OEM-defined power device IDs */
define(`APM_ID_APM_BIOS', `0x0')
define(`APM_ID_ALL', `0x1') /* All devices power managed by the APM BIOS */
define(`APM_ID_CLASS_ALL', `0xff')
define(`APM_CL_DISPLAY', `0x1')
define(`APM_CL_SEC_STORAGE', `0x2')
define(`APM_CL_PARA_PORT', `0x3')
define(`APM_CL_SERI_PORT', `0x4')
define(`APM_CL_NET_ADAPT', `0x5')
define(`APM_CL_PCMCIA', `0x6')
define(`APM_CL_BATT', `0x80')
define(`APM_ID_ALL_COMPAT, `0xffff') /* All devices power managed by the APM
                                      * BIOS (mandatory for compatibility with
                                      * APM v1.0) */


/* Miscellaneous constants. */
define(`APM_BIOS_BASE', `0x5300') /* value of `ah' when calling a function from
                                   * APM BIOS */
define(`APM_PM', `0x504d') /* the value of `ax' on successful return from
                            * the Installation Check function */

')
