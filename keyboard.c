#include <lcom/lcf.h>
#include <keyboard.h>
#include <i8042.h>
#include <minix/sysutil.h>

int kbd_hook_id = 0;
uint8_t keycode = 0;


// Subscribes keyboard interrupts
// bit_no -> Returns activated bit mask 
int kbd_subscribe_int(uint8_t *bit_no){
    *bit_no = KEYBOARD_IRQ;
    kbd_hook_id = *bit_no;
    

    if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE ,&kbd_hook_id) != KEYBOARD_FUNC_SUCCESS)
    {
        return KEYBOARD_FUNC_ERROR;
    }
    
    return KEYBOARD_FUNC_SUCCESS;
}

//Unsubscribes keyboard interrupts
int kbd_unsubscribe_int(){

    if (sys_irqrmpolicy(&kbd_hook_id) != KEYBOARD_FUNC_SUCCESS)
    {
        return KEYBOARD_FUNC_ERROR;
    }

    return KEYBOARD_FUNC_SUCCESS;
    
}

//Reads the keyboard status from the status register
//status -> Keyboard status
int read_status_register(uint8_t *status){
    
    if (util_sys_inb(STAT_REG,status) != KEYBOARD_FUNC_SUCCESS)
    {
        return KEYBOARD_FUNC_ERROR;
    }

    return KEYBOARD_FUNC_SUCCESS;
}

//Reads the Keyboard controller output
// output -> output byte from controller
int read_kbc_output(uint8_t *output, bool flag) {

    uint8_t status = 0, out = 0;
    keycode = ERROR_CODE;
    
    for (size_t i = 0; i < MAX_TRYS; i++)
    {
        if (read_status_register(&status) != KEYBOARD_FUNC_SUCCESS) // Read the status
        {
            return KEYBOARD_FUNC_ERROR;
        }
        
        if (status & STAT_OBF) // checks if output buffer is full
        {
            if (util_sys_inb(OUT_BUF, &out) != KEYBOARD_FUNC_SUCCESS) // reads byte from output buffer
            {
                return KEYBOARD_FUNC_ERROR;
            }

            if ((status & (STAT_PARITY | STAT_TIMEOUT )) == 0) // checks if there was any data error
            {
                *output = out;
                return KEYBOARD_FUNC_SUCCESS;
            }
            else
            {
                return KEYBOARD_FUNC_ERROR;
            }
            if(flag)
                WAIT;
        }
        
        
    }
    
    //printf("Max trys reached\n");
    return KEYBOARD_FUNC_ERROR;

}

void verify_empty_OUTB()
{
    
    uint8_t status = 0, out = 0;
    
    read_status_register(&status); // Read the status
    
    if (status & STAT_OBF) // checks if output buffer is full
        util_sys_inb(OUT_BUF, &out); // reads byte from output buffer
}

//interrupt handler
void (kbc_ih)() {

    if (read_kbc_output(&keycode, false) != KEYBOARD_FUNC_SUCCESS)
    {
        //printf("Output reading error\n");
        return;
    }

   return; 
}

//writes cmd to defined input
//command -> kbc command
//port -> KBC_CMD_REG or KBC_ARG_REG  
int write_kbc_input(uint8_t command, uint8_t port){

    uint8_t status = 0;


    for (size_t i = 0; i < MAX_TRYS; i++)// loop ends when max trys reached or cmd was issued
    {
        if (read_status_register(&status) != KEYBOARD_FUNC_SUCCESS) // reads status
        {
            return KEYBOARD_FUNC_ERROR;
        }
        
        if ((status & STAT_IBF) == 0)// checks if input buffer is clear
        {
            if (sys_outb(port,command) != KEYBOARD_FUNC_SUCCESS) // writes cmd
            {
                return KEYBOARD_FUNC_ERROR;
            }
            return KEYBOARD_FUNC_SUCCESS;
        }
        WAIT;
        
    }

    return KEYBOARD_FUNC_ERROR;
}


//issue command to keyboard controller with no arguments
//command -> cmd to be issued
int issue_kbc_command(uint8_t command){
    if (write_kbc_input(command, KBC_CMD_REG) != KEYBOARD_FUNC_SUCCESS) // writes new command 
    {
        return KEYBOARD_FUNC_ERROR;
    }

    return KEYBOARD_FUNC_SUCCESS;
    
}

int read_command_byte(uint8_t *byte)
{
    if (issue_kbc_command(READ_CMD_BYTE) != KEYBOARD_FUNC_SUCCESS)
        return KEYBOARD_FUNC_ERROR;

    if(read_kbc_output(byte, true) != KEYBOARD_FUNC_SUCCESS)
        return KEYBOARD_FUNC_ERROR;

    return KEYBOARD_FUNC_SUCCESS;

}

//write kbc command byte
//byte -> new command byte
int write_command_byte(uint8_t byte){
    
    if (issue_kbc_command(WRITE_CMD_BYTE) != KEYBOARD_FUNC_SUCCESS) // issues WRITE_CMD_BYTE
    {
        return KEYBOARD_FUNC_ERROR;
    }
    
    if (write_kbc_input(byte,KBC_ARG_REG) != KEYBOARD_FUNC_SUCCESS) // writes new command byte
    {
        return KEYBOARD_FUNC_ERROR; // Why check IN_BUF if argument goes to OUT_BUF?
    }
    
    return KEYBOARD_FUNC_SUCCESS;   
}


