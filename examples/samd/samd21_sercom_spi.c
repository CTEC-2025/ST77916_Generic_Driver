#include <sam.h>
#include "../../ST77916.h"

#define ST77916_WIDTH_U16              (320u)
#define ST77916_HEIGHT_U16             (385u)

static const U8 demo_bitmap_2x2_au8[8] =
{
    0xF8u, 0x00u, 0x07u, 0xE0u,
    0x00u, 0x1Fu, 0xFFu, 0xFFu
};

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

    if (ST77916_init_ex(&lcd_st) != ST77916_STATUS_OK)
    {
        return;
    }

    ST77916_fill_screen(&lcd_st, ST77916_COLOUR_BLACK_U16);
    (void)ST77916_draw_bitmap(&lcd_st, 10, 10, 2u, 2u,
                              demo_bitmap_2x2_au8);
    (void)ST77916_draw_string(&lcd_st, 20, 10, "ST77916",
                              ST77916_COLOUR_WHITE_U16,
                              ST77916_COLOUR_BLACK_U16);
    (void)ST77916_draw_string(&lcd_st, 20, 24, "SAMD21",
                              ST77916_COLOUR_GREEN_U16,
                              ST77916_COLOUR_BLACK_U16);
}
