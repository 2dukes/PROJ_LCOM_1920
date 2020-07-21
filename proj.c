#include <lcom/lcf.h>
#include <sprite.h>
#include <graphics.h>
#include <my_xpms.h>
#include <mouse.h>
#include <i8042.h>
#include <i8254.h>
#include <keyboard.h>

extern uint8_t keycode;

int init_game()
{
    return 0;
}

int update_game()
{
    return 0;
}

int exit_game()
{
    return 0;
}

int main(int argc, char *argv[])
{
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/proj/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int(proj_main_loop)(int argc, char *argv[])
{
    
  sprite *sp = createSprite(DIRECT_16_MODE, pointer, 10, 10, 0, 0);
    // if (sp == NULL)
    //     return 1;
  

  if (vg_init(DIRECT_16_MODE) == NULL)
    return 1;

  displaySprite(sp);

  uint8_t mouse_bit_no = 0, bit_no = 0;

  int ipc_status, r;
  message msg;
  
  uint32_t counter = 0, packetCount = 0, previousPacketCount = 0;
  
  mouse_enable_data_reporting();

  if (mouse_subscribe_int(&mouse_bit_no) != MOUSE_FUNC_SUCCESS)
    return MOUSE_FUNC_ERROR;
  if (kbd_subscribe_int(&bit_no) != KEYBOARD_FUNC_SUCCESS)
    return KEYBOARD_FUNC_ERROR;  

  struct packet p;
  p.delta_x = 0;
  p.delta_y = 0;
  while(keycode != ESC_BREAK_CODE) 
  { 
     /* Get a request message. */
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) 
     { 
       printf("driver_receive failed with: %d", r);
       continue;
     }

    if (is_ipc_notify(ipc_status)) /* received notification */
    { 
        switch (_ENDPOINT_P(msg.m_source)) 
        {
            case HARDWARE: /* hardware interrupt notification */
              if (msg.m_notify.interrupts & BIT(mouse_bit_no)) /* Mouse subscribed interrupt */
              {                     
                mouse_ih();
                previousPacketCount = packetCount;
                parse_mouse_byte(&p, &counter, &packetCount);
                if(previousPacketCount == packetCount)
                  mouse_movement(&p, sp);
                break;
              }
              if (msg.m_notify.interrupts & BIT(bit_no)) /* Keyboard subscribed interrupt */
              {
                kbc_ih();
                break;
              }
            default:
              break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
  } 

  if (mouse_unsubscribe_int() != MOUSE_FUNC_SUCCESS)
    return MOUSE_FUNC_ERROR;

  if (kbd_unsubscribe_int() != KEYBOARD_FUNC_SUCCESS)
    return KEYBOARD_FUNC_ERROR;

  read_kbc_output(&bit_no, true);  

  // /* Reset Mouse configuration */
  // do {
  //       write_mouse_command(DISABLE_DATA_REP);
  //       read_kbc_output(&receivedByte, false);
  // } while (receivedByte != ACK); 


  if (vg_exit() != 0)
    return 1;

    // sprite *sp = createSprite(DIRECT_16_MODE, pointer, 10, 10, 0, 0);
    // if (sp == NULL)
    //     return 1;

    // if (vg_init(DIRECT_16_MODE) == NULL)
    //     return 1;
    // displaySprite(sp);
    // sleep(3);

    // if (vg_exit() != 0)
    //     return 1;

    // destroySprite(sp);
    return 0;
}
