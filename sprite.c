#include <sprite.h>
#include <graphics.h>
#include <my_xpms.h>

extern unsigned bits_per_pixel;
extern void *video_mem;
extern int h_res;
extern int v_res;

sprite *createSprite(uint16_t mode, xpm_map_t map, uint16_t x, uint16_t y, uint16_t x_speed, uint16_t y_speed)
{
    sprite *sp = (sprite *)malloc(sizeof(sprite));

    enum xpm_image_type type = INVALID_XPM;

    switch (mode)
    {
    case INDEX_MODE:
        type = XPM_INDEXED;
        break;
    case DIRECT_15_MODE:
        type = XPM_1_5_5_5;
        break;
    case DIRECT_16_MODE:
        type = XPM_5_6_5;
        break;
    case DIRECT_24_MODE:
        type = XPM_8_8_8;
        break;
    case DIRECT_32_MODE:
        type = XPM_8_8_8_8;
        break;

    default:
        break;
    }

    if (type == INVALID_XPM)
    {
        free(sp);
        return NULL;
    }

    if (xpm_load(map, type, &sp->info) == NULL)
    {
        free(sp);
        return NULL;
    }

    sp->x = x;
    sp->y = y;
    sp->x_speed = x_speed;
    sp->y_speed = y_speed;

    return sp;
}

void destroySprite(sprite *sp)
{
    free(sp);
}

void displaySprite(sprite *sp)
{
    size_t c = 0;
    uint32_t color = 0;
    uint8_t *base_adr = video_mem;
    if(sp->y + 3 * sp->info.height > v_res)
        sp->y = v_res - 3 * sp->info.height;
    else if(sp->y - sp->info.height < 0)
        sp->y = sp->info.height;      
    if(sp->x + 2 * sp->info.width > h_res)
        sp->x = h_res - 2 * sp->info.width;
    else if(sp->x - sp->info.width < 0)
        sp->x = sp->info.width;   
    // printf("sp->x: %d\n sp->y: %d\n", sp->x, sp->y);           
    for (int i = sp->y; i < sp->y + sp->info.height; i++)
    {
        for (int j = sp->x; j < sp->x + sp->info.width; j++)
        {
            if ( (i < 0) || (j < 0) || (j > h_res) || (i > v_res))
            {
                c++;
                return;
            }    
            // color = (uint32_t)sp->info.bytes[c];
            memcpy(&color, &sp->info.bytes[c], 2);
            //printf("color : %x\n", color);
            switch (bits_per_pixel)
            {
            case DIRECT_15_MODE_BPP:
                memcpy((base_adr + (i * h_res + j) * 2), &color, 2);
                c++;
                break;
            default:
                //printf("Entered...");
                memcpy((base_adr + (i * h_res + j) * BYTES_PER_PIXEL(bits_per_pixel)), &color, (int)BYTES_PER_PIXEL(bits_per_pixel));
                c++;
                c++;
                break;
            }
        }
    }
}
