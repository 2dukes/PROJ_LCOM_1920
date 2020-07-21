#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <lcom/lcf.h>

#define BIT(n) (1 << (n))

#define BIOS_SERVICES 0x10

// VBE FUNCTIONS -- AL

#define VGA_BIOS_FUNC 0x4F

// VBE FUNCTIONS -- AH

#define GET_VBE_CTRL_INFO 0x00
#define GET_VBE_MODE_INFO 0x01
#define SET_VBA_MODE 0x02

//return values -- AX

#define VBE_SUCCESS 0x00
#define VBE_CALL_FAILED 0x01
#define VBE_NOT_SUPPORTED 0x02
#define VBE_INVALID 0x03

// BX

#define LINEAR_BUFFER_MODEL BIT(14)

//VBE Modes

#define INDEX_MODE 0x105
#define INDEX_MODE_BPP 8

#define DIRECT_15_MODE 0x110
#define DIRECT_15_MODE_BPP 15

#define DIRECT_16_MODE 0x11A
#define DIRECT_16_MODE_BPP 16

#define DIRECT_24_MODE 0x115
#define DIRECT_24_MODE_BPP 24

#define DIRECT_32_MODE 0x14C
#define DIRECT_32_MODE_BPP 32

#define BYTES_PER_PIXEL(n) (n / 8)

#define RED_MASK_5 0x1f
#define RED_MASK_8 0xff

#define GREEN_MASK_5 0x1f
#define GREEN_MASK_6 0x3f
#define GREEN_MASK_8 0xff

#define BLUE_MASK_5 0x1f
#define BLUE_MASK_8 0xff

#define MAXIMUM_RATE 60

int get_mode_info(uint16_t mode, vbe_mode_info_t *info);

int draw_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step);

void cls();

#endif
