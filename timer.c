#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

unsigned int ti_counter = 0; // timer interrupt counter
int timer_hook_id = 0;

int bin_to_bcd(int* digits, uint16_t timer_val, uint8_t* timer_val_lsb, uint8_t* timer_val_msb)
{
  int length = 0;
  while (timer_val != 0)
  {
    int d = timer_val % 10;
    digits[length++] = d;
    timer_val /= 10;
  }
  if (length < MAX_BCD_LEN)
  {
    printf("Length of Timer's value greater than 4 (decimal)...\n");
    return TIMER_ERROR;
  }
  else
  {
    /* Construction of LSB */
    *timer_val_lsb = digits[0] | (digits[1] << 4); // Left shifts previous digit (digit[1]) to form LSB in BCD format
    // printf("\n LSB = %d", timer_val_lsb);

    /* Construction of MSB */
    *timer_val_msb = digits[2] | (digits[3] << 4); // Right shifts previous digit (digit[3]) to form MSB in BCD format
    // printf("\n MSB = %d", timer_val_msb);
  }

  return TIMER_SUCCESS;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  uint8_t status;
  if (timer_get_conf(timer, &status) != TIMER_SUCCESS) 
      return TIMER_ERROR;

  uint8_t ctrl_word = GET_LSB & status;

  switch (timer)
  {
    case 0: // Timer 0 CONFIGURATION
      ctrl_word = (TIMER_SEL0 | TIMER_LSB_MSB | ctrl_word);
      break;

    case 1: // Timer 1 CONFIGURATION
      ctrl_word = (TIMER_SEL1 | TIMER_LSB_MSB | ctrl_word);
      break;

    case 2: // Timer 2 CONFIGURATION
      ctrl_word = (TIMER_SEL2 | TIMER_LSB_MSB | ctrl_word);
      break;

    default:
      //invalid timer 
      return TIMER_ERROR;
      break;
  }
  
  if (sys_outb(TIMER_CTRL, ctrl_word) != TIMER_SUCCESS) // Send Control Word specifying the right  timer
    return TIMER_ERROR;

  uint16_t timer_val = TIMER_FREQ / freq; // VAL_TIMER = CLOCK / FREQ
  
  /* Distinguish between BCD vs binary */
  uint8_t counting_mode = status & TIMER_BCD; // 0 = binary && 1 == BCD

  uint8_t timer_val_msb = 0, timer_val_lsb = 0;
  
  if (counting_mode == 0)
  { /* BINARY MODE */
    if (freq < TIMER_MIN_FREQ_BIN || freq > TIMER_FREQ) // Valid for binary
      return TIMER_ERROR;

    util_get_MSB(timer_val, &timer_val_msb);
    util_get_LSB(timer_val, &timer_val_lsb);
  }
  else
  { /* BCD MODE */
    if (freq < TIMER_MAX_FREQ_BCD || freq > TIMER_FREQ) // Valid for BCD
      return TIMER_ERROR;

    int digits[] = {0, 0, 0, 0};
    if (bin_to_bcd(digits, timer_val, &timer_val_lsb, &timer_val_msb) != 0)
      return TIMER_ERROR; 
  }  
  
  if (sys_outb(TIMER_0 + timer, timer_val_lsb) != TIMER_SUCCESS) 
    return TIMER_ERROR;
  if (sys_outb(TIMER_0 + timer, timer_val_msb) != TIMER_SUCCESS) 
    return TIMER_ERROR;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
   
   *bit_no = TIMER0_IRQ;
   timer_hook_id = *bit_no;

   if (sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&timer_hook_id) != TIMER_SUCCESS)
   {
     return TIMER_ERROR;
   }
   
  return TIMER_SUCCESS;
}

int (timer_unsubscribe_int)() {
  
  if (sys_irqrmpolicy(&timer_hook_id) != TIMER_SUCCESS)
  {
    return TIMER_ERROR;
  }
  
  return TIMER_SUCCESS;
}

void (timer_int_handler)() {
  ti_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {  
  
  /* Constructing Read-Back CMD */
  u32_t rb_cmd = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));
  
  if (sys_outb(TIMER_CTRL, rb_cmd) != TIMER_SUCCESS) {
    return TIMER_ERROR;
  } // Sending Read-Back CMD to Control Register 
  
  if (util_sys_inb(TIMER_0 + timer, st) != TIMER_SUCCESS) {
    return TIMER_ERROR;
  }
  
  return TIMER_SUCCESS;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  /* Parsing STATUS */
  union timer_status_field_val result;
  uint8_t init_mode = ((st & INITIALIZATION_MASK) >> 4);
  switch(field)
  {
    case(0): // Entire Status byte
      result.byte = st;
      break;

    case(1): // INITIALIZATION_MODE
      switch(init_mode)
      {
        case(1):
          result.in_mode = LSB_only;
          break;

        case(2):
          result.in_mode = MSB_only;
          break;

        case(3):
          result.in_mode = MSB_after_LSB;
          break;

        default:
          result.in_mode = INVAL_val;
          break;
      }
      break;

    case(2): // COUNT_MODE 
      result.count_mode = (st & COUNT_MODE_MASK) >> 1; 
      
      if(result.count_mode == 6)
          result.count_mode = 2;
      else if(result.count_mode == 7)
          result.count_mode = 3;

      break;

    case(3): // COUNTING_BASE
      result.bcd = st & BCD_MASK;
      break;

     default:
      return TIMER_ERROR;
      break; 
  }
  timer_print_config(timer, field, result);
  return TIMER_SUCCESS;
}

