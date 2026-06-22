#include <sam.h>
#include "../../ST77916.h"

#define ST77916_WIDTH_U16              (320u)
#define ST77916_HEIGHT_U16             (385u)
#define ST77916_PIXEL_COUNT_U32        (123200u)

#define ST77916_CS_GROUP_U8            (0u)
#define ST77916_CS_PIN_U8              (10u)
#define ST77916_DC_GROUP_U8            (0u)
#define ST77916_DC_PIN_U8              (9u)
#define ST77916_RST_GROUP_U8           (0u)
#define ST77916_RST_PIN_U8             (8u)

static ST77916_st lcd_st;

static void samd_pin_write(U8 group_u8, U8 pin_u8, U8 level_u8)
{
    U32 mask_u32;

    mask_u32 = (U32)1u << pin_u8;

    if (level_u8 == ST77916_RESET_HIGH_U8)
    {
        PORT->Group[group_u8].OUTSET.reg = mask_u32;
    }
    else
    {
        PORT->Group[group_u8].OUTCLR.reg = mask_u32;
    }
}

static void samd_spi_write(U8 value_u8)
{
    while (SERCOM0->SPI.INTFLAG.bit.DRE == 0u)
    {
    }

    SERCOM0->SPI.DATA.reg = value_u8;

    while (SERCOM0->SPI.INTFLAG.bit.TXC == 0u)
    {
    }
}

static void samd_write_cmd(U8 command_u8)
{
    samd_pin_write(ST77916_DC_GROUP_U8, ST77916_DC_PIN_U8, 0u);
    samd_pin_write(ST77916_CS_GROUP_U8, ST77916_CS_PIN_U8, 0u);
    samd_spi_write(command_u8);
    samd_pin_write(ST77916_CS_GROUP_U8, ST77916_CS_PIN_U8, 1u);
}

static void samd_write_data(const U8 * data_pu8, U16 length_u16)
{
    U16 index_u16;

    if (data_pu8 != NULL)
    {
        samd_pin_write(ST77916_DC_GROUP_U8, ST77916_DC_PIN_U8, 1u);
        samd_pin_write(ST77916_CS_GROUP_U8, ST77916_CS_PIN_U8, 0u);

        for (index_u16 = 0u; index_u16 < length_u16; index_u16++)
        {
            samd_spi_write(data_pu8[index_u16]);
        }

        samd_pin_write(ST77916_CS_GROUP_U8, ST77916_CS_PIN_U8, 1u);
    }
}

static void samd_delay_ms(U32 delay_ms_u32)
{
    while (delay_ms_u32 > 0u)
    {
        SysTick->LOAD = 48000u - 1u;
        SysTick->VAL = 0u;
        SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;

        while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0u)
        {
        }

        SysTick->CTRL = 0u;
        delay_ms_u32--;
    }
}

static void samd_reset(U8 level_u8)
{
    samd_pin_write(ST77916_RST_GROUP_U8, ST77916_RST_PIN_U8, level_u8);
}

void app_display_init(void)
{
    lcd_st.bus_st.write_cmd = samd_write_cmd;
    lcd_st.bus_st.write_data = samd_write_data;
    lcd_st.bus_st.delay_ms = samd_delay_ms;
    lcd_st.bus_st.reset_pin = samd_reset;
    lcd_st.width_u16 = ST77916_WIDTH_U16;
    lcd_st.height_u16 = ST77916_HEIGHT_U16;
    lcd_st.rotation_u8 = ST77916_ROTATION_0_U8;

    ST77916_init(&lcd_st);
    ST77916_fill_screen(&lcd_st, 0x0000u);
    ST77916_fill_rect(&lcd_st, 20u, 20u, 80u, 40u, 0xFFE0u);
    ST77916_draw_rect(&lcd_st, 18, 18, 84u, 44u, 0xFFFFu);
    ST77916_draw_line(&lcd_st, 0, 0, 319, 384, 0xF800u);
    ST77916_draw_circle(&lcd_st, 160, 192, 48u, 0x001Fu);
    ST77916_fill_circle(&lcd_st, 160, 192, 24u, 0x07E0u);
}
