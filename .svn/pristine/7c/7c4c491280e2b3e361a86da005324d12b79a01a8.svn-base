#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>

/** @defgroup i8254 i8254
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

#define TIMER_FREQ 1193182 /**< @brief clock frequency for timer in PC and AT */
#define TIMER0_IRQ 0 /**< @brief Timer 0 IRQ line */

/* I/O port addresses */

#define TIMER_0    0x40 /**< @brief Timer 0 count register */
#define TIMER_1    0x41 /**< @brief Timer 1 count register */
#define TIMER_2    0x42 /**< @brief Timer 2 count register */
#define TIMER_CTRL 0x43 /**< @brief Control register */

#define SPEAKER_CTRL 0x61 /**< @brief Register for speaker control  */

/* Timer control */

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0   0x00              /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1   BIT(6)            /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2   BIT(7)            /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD (BIT(7) | BIT(6)) /**< @brief Read Back Command */

/* Register selection: bits 5 and 4 */

#define TIMER_LSB     BIT(4)                  /**< @brief Initialize Counter LSB only */
#define TIMER_MSB     BIT(5)                  /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */

/* Operating mode: bits 3, 2 and 1 */

#define TIMER_SQR_WAVE (BIT(2) | BIT(1)) /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN BIT(2)            /**< @brief Mode 2: rate generator */

/* Counting mode: bit 0 */

#define TIMER_BCD 0x01 /**< @brief Count in BCD */
#define TIMER_BIN 0x00 /**< @brief Count in binary */

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_  BIT(5)
#define TIMER_RB_STATUS_ BIT(4)
#define TIMER_RB_SEL(n)  BIT((n) + 1)


#define COUNT_MODE_MASK  0x0E             // Mask used to get COUNT_MODE bits
#define BCD_MASK         0x01             // Mask used to get BCD bit
#define INITIALIZATION_MASK 0x30          // Mask used to get INITIALIZATION_MODE bits

#define GET_LSB           0x0F            // Mask used to get the LSB from CONTROL WORD

#define REG_FREQ sys_hz()                       // normal frequency of an unchanged timer

#define TIMER_SUCCESS 0
#define TIMER_ERROR -1

#define TIMER_MIN_FREQ_BIN 19                  // Minimum frequency accepted in BINARY mode
#define TIMER_MAX_FREQ_BCD 120                 // Maximum frequency accepted in BCD mode
#define MAX_BCD_LEN        4                   // Maximum size of 16 bits BCD number

#define DELAY_US    20000                      // Delay upon an unsuccessful operation
#define MAX_TRYS    5                          // Maximum number of tries when an operation isn't sucessful
#define WAIT tickdelay(micros_to_ticks(DELAY_US)) // 20ms delay

/**@}*/

#endif /* _LCOM_I8254_H */


