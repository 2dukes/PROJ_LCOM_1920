#ifndef _I8042_H_
#define _I8042_H_


#define BIT(n) (1 << (n))

#define KEYBOARD_IRQ 1                  // Keyboard IRQ

#define STAT_REG         0x64                // Status register
#define KBC_CMD_REG      0x64                  // Kbc command register
#define KBC_ARG_REG      0x60                 // Kbc command arguments register
#define OUT_BUF          0x60                // Output buffer

//Status Register

#define STAT_PARITY     BIT(7)          // Status Parity flag
#define STAT_TIMEOUT    BIT(6)          // Status Timeout flag
#define STAT_AUX        BIT(5)          // Status Mouse data flag
#define STAT_INH        BIT(4)          // Status Inhibit flag
#define STAT_A2         BIT(3)          // Status A2 input line flag
#define STAT_SYS        BIT(2)          // Status System flag
#define STAT_IBF        BIT(1)          // Status Input buffer flag
#define STAT_OBF        BIT(0)          // Status Output buffer flag

// Keyboard controller commands

#define READ_CMD_BYTE               0x20        // Read Command Byte
#define WRITE_CMD_BYTE              0x60        // Write Command Byte
#define CHECK_KBC                   0xAA        // Check KBC(Self-test)
#define DISABLE_KBD_INTERFACE       0xAD        // Disable KBD interface
#define ENABLE_KBD_INTERFACE        0xAE        // Enable KBD interface

// Command Byte

#define CMD_BYTE_DIS2       BIT(5)          // Disable mouse flag
#define CMD_BYTE_DIS        BIT(4)          // Disable KBD interface flag
#define CMD_BYTE_INT2       BIT(1)          // Enable interrupt on OBF, from mouse flag
#define CMD_BYTE_INT        BIT(0)          // Enable interrupt on OBF, from KBD flag

#define KEYBOARD_FUNC_ERROR         -1
#define KEYBOARD_FUNC_SUCCESS        0


#define FIRST_BYTE                   0xE0  // First byte in a sequence of two bytes
#define ESC_BREAK_CODE               0x81  // ESC key Breakcode
#define ERROR_CODE                   0xFF  // Error Scancode

#define DELAY_US    20000                  // 20ms delay used when an operation isn't successful
#define MAX_TRYS    5                      // Maximum tries when an operation isn't successful
#define WAIT tickdelay(micros_to_ticks(DELAY_US)) // 20ms delay

/* MOUSE DEFINITIONS */

#define MOUSE_FUNC_ERROR               -1
#define MOUSE_FUNC_SUCCESS              0

#define MOUSE_IRQ                       12  // Corresponding IRQ Line for the mouse

#define WRITE_BYTE_TO_MOUSE             0xD4 // Write Byte to Mouse KBC Command

/* Mouse Commands */

#define MOUSE_RESET                     0xFF
#define DISABLE_DATA_REP                0xF5
#define ENABLE_DATA_REP                 0xF4
#define SET_REMOTE_MODE                 0xF0
#define READ_DATA                       0xEB
#define SET_STREAM_MODE                 0xEA
#define STATUS_REQUEST                  0xE9

#define ACK                             0xFA
#define NACK                            0xFE
#define ERROR                           0xFC

/* Mouse Packets */
#define LEFT_BUTTON_DOWN                BIT(0)
#define RIGHT_BUTTON_DOWN               BIT(1)
#define MIDDLE_BUTTON_DOWN              BIT(2)


#endif

