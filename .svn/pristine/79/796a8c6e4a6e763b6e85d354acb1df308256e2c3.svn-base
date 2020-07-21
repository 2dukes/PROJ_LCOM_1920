#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <inttypes.h>
#include <stdint.h>

/**
 * @brief Interrupt handler to read keyboard scancodes
 */ 
void (kbc_ih)();

/**
 * @brief Subscribes and enables Keyboard interrupts
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */ 
int kbd_subscribe_int(uint8_t *bit_no); // done

/**
 * @brief Unsubscribes Keyboard interrupts
 * 
 * @return Return 0 upon success and non-zero otherwise
 */ 
int kbd_unsubscribe_int(); //done 

/**
 * @brief Verify if OUT_BUF is empty between the unsubscribe interrupt and the end of the function
 */ 
void verify_empty_OUTB();

/**
 * @brief Writes command to either 0x64 (Commands address) and 0x60 (Parameters address)
 * 
 * @param command Command to write
 * @param port Either IN_BUF or OUT_BUF
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int write_kbc_input(uint8_t command, uint8_t port);

/**
 * @brief Reads the KBC OUT_BUF (output) and evaluates any error in the scancode
 * 
 * @param output Pointer to output that will be read
 * @param flag Flag that will be True if there's a need to wait whenever a read operation isn't successful. False otherwise.
 * 
 * @return Return 0 upon success and non-zero otherwise
 * 
 */ 
int read_kbc_output(uint8_t *output, bool flag);//done

/**
 * @brief Reads the Status Register
 * 
 * @param status Pointer to the content of the read Status Register
 * 
 * @return Return 0 upon success and non-zero otherwise
 */ 
int read_status_register(uint8_t *status); // done

/**
 * @brief Issue a KBC Command. The command won't have any parameters
 * 
 * @param command Command to issue
 * 
 * @return Return 0 upon success and non-zero otherwise
 */ 
int issue_kbc_command(uint8_t command);//done

/**
 * @brief Read output upon issuing a command byte that retrieves something.
 * 
 * @param byte Pointer to the content of the read operation
 * 
 * @return Return 0 upon success and non-zero otherwise
 */ 
int read_command_byte(uint8_t *byte);

/**
 * @brief Issue a KBC Command. This time the command will have parameters and the will be followed as well
 * 
 * @param byte Parameter of the KBC Command that will be written
 * 
 * @return Return 0 upon success and non-zero otherwise
 */ 
int write_command_byte(uint8_t byte);

#endif
