#include <lcom/lcf.h>
#include <keyboard.h>
#include <mouse.h>
#include <i8042.h>
#include <minix/sysutil.h>

int mouse_hook_id = 0;
uint8_t mouseByte = 0;

// Subscribes MOUSE interrupts
// bit_no -> Returns activated bit mask 
int mouse_subscribe_int(uint8_t *bit_no) {
    *bit_no = MOUSE_IRQ; // *bit_no = 12
    mouse_hook_id = *bit_no;
    
    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE ,&mouse_hook_id) != MOUSE_FUNC_SUCCESS)
        return MOUSE_FUNC_ERROR;
    
    return MOUSE_FUNC_SUCCESS;
}

// Unsubscribes MOUSE interrupts
int mouse_unsubscribe_int() {

    if (sys_irqrmpolicy(&mouse_hook_id) != MOUSE_FUNC_SUCCESS)
        return MOUSE_FUNC_ERROR;

    return KEYBOARD_FUNC_SUCCESS;
}


int write_mouse_command(uint8_t byte){
    
    if (issue_kbc_command(WRITE_BYTE_TO_MOUSE) != MOUSE_FUNC_SUCCESS) // issues WRITE_CMD_BYTE
        return MOUSE_FUNC_ERROR;
    
    if (write_kbc_input(byte, KBC_ARG_REG) != MOUSE_FUNC_SUCCESS) // writes new command byte
        return MOUSE_FUNC_ERROR; // Why check IN_BUF if argument goes to OUT_BUF?
    
    return KEYBOARD_FUNC_SUCCESS;   
}

bool verifyFirstByte()
{
    /* Verifies if BIT(3) of MouseByte is set to 1. If so, it's the 1st byte out of 3 */
    if(mouseByte & BIT(3))
        return true;
    return false;
}

void parse_mouse_byte(struct packet *p, uint32_t *counter, uint32_t *packetCount)
{
    if(verifyFirstByte() && *counter == 0)
    {
        p->bytes[(*counter)++] = mouseByte;
        p->lb = mouseByte & BIT(0);
        p->rb = mouseByte & BIT(1);
        p->mb = mouseByte & BIT(2);
        p->x_ov = mouseByte & BIT(6);
        p->y_ov = mouseByte & BIT(7);    
    }
    else if(*counter > 0)
    {
        p->bytes[(*counter)++] = mouseByte;

        if(*counter == 2)
        {
            if(p->bytes[0] & BIT(4)) /* Bit 4 of 1st packet is 1... Need to convert CPL2 (X) */
                p->delta_x = p->bytes[1] | 0xFF00;
           else
                p->delta_x = mouseByte; 
        }
        else if(*counter == 3)
        {
            if((p->bytes[0]) & BIT(5)) /* Bit 5 of 1st packet is 1... Need to convert CPL2 (Y) */
                p->delta_y = p->bytes[2] | 0xFF00;
            else
                p->delta_y = mouseByte;

            *counter = 0;
            (*packetCount)++;
            // mouse_print_packet(p);
        }
    }
}

void mouse_movement(struct packet* p, sprite* sp)
{
    sp->x += p->delta_x / 3;
    sp->y -= p->delta_y / 3;  

    cls();
    displaySprite(sp);
    
    // printf("%d - %d\n", sp->x, sp->y);
    // printf("%d\n", displacement);
}

void (mouse_ih)(void)
{
  if (read_kbc_output(&mouseByte, false) != MOUSE_FUNC_SUCCESS)
        return;
   return; 
}
