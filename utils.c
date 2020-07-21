#include <lcom/lcf.h>

#include <i8254.h>
#include <stdint.h>

int cnt = 0;

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  
  *lsb = val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  
  *msb = val >> 8;
  return 0;
}

int util_sys_inb(int port, uint8_t *value) {
  /* Read Timer's Status */

  u32_t byte;

  // #ifndef LAB3
  // #define LAB3
  // #endif 

  if (sys_inb(port, &byte) != TIMER_SUCCESS)
    return TIMER_ERROR;
  #ifdef LAB3
  cnt++;
  #endif
  
   // Read 4 bytes

  *value = byte; // Get the Status byte

  return TIMER_SUCCESS;
}
