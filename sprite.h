#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <lcom/lcf.h>

typedef struct
{
    uint16_t x;
    uint16_t y;
    int16_t x_speed;
    int16_t y_speed;
    xpm_image_t info;
} sprite;

sprite *createSprite(uint16_t mode, xpm_map_t map, uint16_t x, uint16_t y, uint16_t x_speed, uint16_t y_speed);

void destroySprite(sprite *sp);

void displaySprite(sprite *sp);

#endif
