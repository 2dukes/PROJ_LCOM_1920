#include <graphics.h>
#include <machine/int86.h>

void *video_mem; // Process (virtual) address to which VRAM is mapped

int h_res;               // Horizontal resolution in pixels
int v_res;               // Vertical resolution in pixels
unsigned bits_per_pixel; // Number of VRAM bits per pixel
static unsigned RedMask;
static unsigned GreenMask;
static unsigned BlueMask;

static bool is_indexed = false;

void *(vg_init)(uint16_t mode)
{

    if (mode == INDEX_MODE)
    {
        is_indexed = true;
    }

    struct minix_mem_range mr;
    int r;

    // mr.mr_base = 0;
    // mr.mr_limit = 1 << 20;

    // if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    //     panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    vbe_mode_info_t m_info;

    // lm_init(1);
    if (get_mode_info(mode, &m_info) != VBE_SUCCESS)
    {
        return NULL;
    }

    h_res = m_info.XResolution;
    v_res = m_info.YResolution;
    bits_per_pixel = m_info.BitsPerPixel;
    RedMask = m_info.LinRedMaskSize;
    GreenMask = m_info.LinGreenMaskSize;
    BlueMask = m_info.LinBlueMaskSize;

    memset(&mr, 0, sizeof(mr));

    unsigned int vram_base = m_info.PhysBasePtr;                              /* VRAM's physical addresss */
    unsigned int vram_size = v_res * h_res * BYTES_PER_PIXEL(bits_per_pixel); /* VRAM's size, but you can use the frame-buffer size, instead */

    mr.mr_base = (phys_bytes)vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    /* Map memory */

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if (video_mem == MAP_FAILED)
        panic("couldn't map video memory");

    /*CHANGE MODE */
    struct reg86 reg86;

    memset(&reg86, 0, sizeof(reg86));

    reg86.ax = (u_int16_t)((VGA_BIOS_FUNC << 8) | SET_VBA_MODE);
    reg86.bx = LINEAR_BUFFER_MODEL | mode;
    reg86.intno = BIOS_SERVICES;

    if (sys_int86(&reg86) != OK)
    {
        printf("set_vbe_mode: sys_int86() failed \n");
        return NULL;
    }

    return video_mem;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{
    uint8_t *base_adr = video_mem;
    for (int i = x; i < (x + len); i++)
    {
        if (i < h_res && i >= 0)
        {
            //printf("Pixels...\n");
            switch (bits_per_pixel)
            {
            case DIRECT_15_MODE_BPP:
                memcpy((base_adr + (y * h_res + i) * 2), &color, 2);
                break;
            default:
                //printf("Entered...");
                memcpy((base_adr + (y * h_res + i) * BYTES_PER_PIXEL(bits_per_pixel)), &color, (int)BYTES_PER_PIXEL(bits_per_pixel));
                break;
            }
        }
    }

    return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    for (int i = y; i < (y + height); i++)
    {
        if (i < v_res && i >= 0)
        {
            vg_draw_hline(x, i, width, color);
        }
    }

    return 0;
}

void separate_RGB(uint32_t color, uint8_t *R, uint8_t *G, uint8_t *B)
{
    if (bits_per_pixel == DIRECT_15_MODE_BPP)
    {
        *B = color & BLUE_MASK_5;
        *G = (color >> 5) & GREEN_MASK_5;
        *R = (color >> 10) & RED_MASK_5;
    }
    if (bits_per_pixel == DIRECT_16_MODE_BPP)
    {
        *B = color & BLUE_MASK_5;
        *G = (color >> 5) & GREEN_MASK_6;
        *R = (color >> 11) & RED_MASK_5;
    }
    else
    {
        *B = color & BLUE_MASK_8;
        *G = (color >> 8) & GREEN_MASK_8;
        *R = (color >> 16) & RED_MASK_8;
    }
}

void mount_RGB(uint32_t *color, uint8_t R, uint8_t G, uint8_t B)
{
    if (bits_per_pixel == DIRECT_15_MODE_BPP)
    {
        *color = *color | R;
        //printf("Color R = %x\n", *color);
        *color = *color << 5;
        *color = *color | G;
        //printf("Color G = %x\n", *color);
        *color = *color << 5;
        *color = *color | B;
        //printf("Color B = %x\n", *color);
    }
    if (bits_per_pixel == DIRECT_16_MODE_BPP)
    {
        *color = *color | R;
        //printf("Color R 1 = %x\n", *color);
        *color = *color << 6;
        *color = *color | G;
        //printf("Color G 1 = %x\n", *color);
        *color = *color << 5;
        *color = *color | B;
        //printf("Color B 1 = %x\n", *color);
    }
    else
    {
        *color = *color | R;
        *color = *color << 8;
        *color = *color | G;
        *color = *color << 8;
        *color = *color | B;
    }
}

void offsetcolor(uint16_t x, uint16_t y, uint32_t first, uint8_t step, uint8_t no_rectangles, uint32_t *color)
{
    uint32_t c = 0x0;

    if (is_indexed)
    {
        c = (first + (x * no_rectangles + y) * step) % (1 << bits_per_pixel);
        *color = c;
        return;
    }
    uint8_t R = 0, iR = 0, G = 0, iG = 0, B = 0, iB = 0;

    separate_RGB(first, &iR, &iG, &iB);

    R = (iR + y * step) % (1 << RedMask); //? Quais os masks a utilizar
    G = (iG + x * step) % (1 << GreenMask);
    B = (iB + (y + x) * step) % (1 << BlueMask);
    printf("R = %x\n G = %x\n B = %x\n", R, G, B);

    mount_RGB(&c, R, G, B); //! valor de color na funçao diferent do que é imprimido
    *color = c;
    printf("Color = %x\n", *color);
}

int draw_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step)
{
    uint16_t rectangle_width = (int)(h_res / no_rectangles), rectangle_height = (int)(v_res / no_rectangles);
    uint32_t color = 0;
    printf("%d\n", h_res);
    printf("%d\n", rectangle_width);
    printf("%d\n", v_res);
    printf("%d\n", rectangle_height);

    for (size_t i = 0; i < no_rectangles; i++)
    {
        for (size_t j = 0; j < no_rectangles; j++)
        {
            offsetcolor(i, j, first, step, no_rectangles, &color);

            vg_draw_rectangle(j * rectangle_width, i * rectangle_height, rectangle_width, rectangle_height, color);
        }
    }

    return 0;
}

void cls()
{
    int j;
    uint8_t *base_adr = video_mem;
    for (j = 0; j < v_res * h_res; j++)
        *(base_adr + j) = 0;
}

int get_mode_info(uint16_t mode, vbe_mode_info_t *info)
{
    mmap_t mem;

    lm_alloc(sizeof(vbe_mode_info_t), &mem);

    struct reg86 reg86;

    memset(&reg86, 0, sizeof(reg86));

    reg86.ax = (uint16_t)((VGA_BIOS_FUNC << 8) | GET_VBE_MODE_INFO);
    reg86.cx = mode;
    reg86.es = PB2BASE(mem.phys);
    reg86.di = PB2OFF(mem.phys);
    reg86.intno = BIOS_SERVICES;

    if (sys_int86(&reg86) != OK)
    {
        printf("set_vbe_mode: sys_int86() failed \n");
        return 1;
    }

    *info = *(vbe_mode_info_t *)mem.virt;

    lm_free(&mem);

    return 0;
}

int get_controller_info(uint16_t mode, vbe_mode_info_t *info)
{
    return 0;
}
