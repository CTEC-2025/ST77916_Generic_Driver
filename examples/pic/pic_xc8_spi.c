#include <xc.h>
#include "../../ST77916.h"

#define _XTAL_FREQ                     (16000000UL)
#define ST77916_WIDTH_U16              (320u)
#define ST77916_HEIGHT_U16             (385u)
#define ST77916_PIXEL_COUNT_U32        (123200u)

#define ST77916_CS_LAT                 LATCbits.LATC0
#define ST77916_DC_LAT                 LATCbits.LATC1
#define ST77916_RST_LAT                LATCbits.LATC2

static ST77916_st lcd_st;

static void pic_spi_write(U8 value_u8)
{
    SSP1BUF = value_u8;

    while (SSP1STATbits.BF == 0u)
    {
    }
}

static void pic_write_cmd(U8 command_u8)
{
    ST77916_DC_LAT = 0u;
    ST77916_CS_LAT = 0u;
    pic_spi_write(command_u8);
    ST77916_CS_LAT = 1u;
}

static void pic_write_data(const U8 * data_pu8, U16 length_u16)
{
    U16 index_u16;

    if (data_pu8 != NULL)
    {
        ST77916_DC_LAT = 1u;
        ST77916_CS_LAT = 0u;

        for (index_u16 = 0u; index_u16 < length_u16; index_u16++)
        {
            pic_spi_write(data_pu8[index_u16]);
        }

        ST77916_CS_LAT = 1u;
    }
}

static void pic_delay_ms(U32 delay_ms_u32)
{
    while (delay_ms_u32 > 0u)
    {
        __delay_ms(1u);
        delay_ms_u32--;
    }
}

static void pic_reset(U8 level_u8)
{
    if (level_u8 == ST77916_RESET_HIGH_U8)
    {
        ST77916_RST_LAT = 1u;
    }
    else
    {
        ST77916_RST_LAT = 0u;
    }
}

void app_display_init(void)
{
    lcd_st.bus_st.write_cmd = pic_write_cmd;
    lcd_st.bus_st.write_data = pic_write_data;
    lcd_st.bus_st.delay_ms = pic_delay_ms;
    lcd_st.bus_st.reset_pin = pic_reset;
    lcd_st.width_u16 = ST77916_WIDTH_U16;
    lcd_st.height_u16 = ST77916_HEIGHT_U16;
    lcd_st.rotation_u8 = ST77916_ROTATION_0_U8;

    ST77916_init(&lcd_st);
    ST77916_fill_screen(&lcd_st, ST77916_COLOUR_BLACK_U16);
    ST77916_fill_rect(&lcd_st, 20u, 20u, 80u, 40u,
                      ST77916_COLOUR_GREEN_U16);
    ST77916_draw_rect(&lcd_st, 18, 18, 84u, 44u,
                      ST77916_COLOUR_WHITE_U16);
    ST77916_draw_line(&lcd_st, 0, 384, 319, 0,
                      ST77916_COLOUR_RED_U16);
    ST77916_draw_circle(&lcd_st, 160, 192, 48u,
                        ST77916_COLOUR_BLUE_U16);
    ST77916_fill_circle(&lcd_st, 160, 192, 24u,
                        ST77916_COLOUR_YELLOW_U16);
}
