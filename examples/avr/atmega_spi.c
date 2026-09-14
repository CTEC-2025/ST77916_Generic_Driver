#include <avr/io.h>
#include <util/delay.h>
#include "../../ST77916.h"

#define ST77916_WIDTH_U16              (320u)
#define ST77916_HEIGHT_U16             (385u)
#define ST77916_PIXEL_COUNT_U32        (123200u)

#define ST77916_CS_PORT                PORTB
#define ST77916_CS_DDR                 DDRB
#define ST77916_CS_BIT                 PB2
#define ST77916_DC_PORT                PORTB
#define ST77916_DC_DDR                 DDRB
#define ST77916_DC_BIT                 PB1
#define ST77916_RST_PORT               PORTB
#define ST77916_RST_DDR                DDRB
#define ST77916_RST_BIT                PB0

static ST77916_st lcd_st;

static void avr_pin_high(volatile U8 * port_pu8, U8 bit_u8)
{
    *port_pu8 |= (U8)(1u << bit_u8);
}

static void avr_pin_low(volatile U8 * port_pu8, U8 bit_u8)
{
    *port_pu8 &= (U8)(~(1u << bit_u8));
}

static void avr_spi_write(U8 value_u8)
{
    SPDR = value_u8;

    while ((SPSR & (1u << SPIF)) == 0u)
    {
    }
}

static void avr_write_cmd(U8 command_u8)
{
    avr_pin_low(&ST77916_DC_PORT, ST77916_DC_BIT);
    avr_pin_low(&ST77916_CS_PORT, ST77916_CS_BIT);
    avr_spi_write(command_u8);
    avr_pin_high(&ST77916_CS_PORT, ST77916_CS_BIT);
}

static void avr_write_data(const U8 * data_pu8, U16 length_u16)
{
    U16 index_u16;

    if (data_pu8 != NULL)
    {
        avr_pin_high(&ST77916_DC_PORT, ST77916_DC_BIT);
        avr_pin_low(&ST77916_CS_PORT, ST77916_CS_BIT);

        for (index_u16 = 0u; index_u16 < length_u16; index_u16++)
        {
            avr_spi_write(data_pu8[index_u16]);
        }

        avr_pin_high(&ST77916_CS_PORT, ST77916_CS_BIT);
    }
}

static void avr_delay_ms(U32 delay_ms_u32)
{
    while (delay_ms_u32 > 0u)
    {
        _delay_ms(1.0);
        delay_ms_u32--;
    }
}

static void avr_reset(U8 level_u8)
{
    if (level_u8 == ST77916_RESET_HIGH_U8)
    {
        avr_pin_high(&ST77916_RST_PORT, ST77916_RST_BIT);
    }
    else
    {
        avr_pin_low(&ST77916_RST_PORT, ST77916_RST_BIT);
    }
}

void app_display_init(void)
{
    ST77916_CS_DDR |= (U8)(1u << ST77916_CS_BIT);
    ST77916_DC_DDR |= (U8)(1u << ST77916_DC_BIT);
    ST77916_RST_DDR |= (U8)(1u << ST77916_RST_BIT);

    lcd_st.bus_st.write_cmd = avr_write_cmd;
    lcd_st.bus_st.write_data = avr_write_data;
    lcd_st.bus_st.delay_ms = avr_delay_ms;
    lcd_st.bus_st.reset_pin = avr_reset;
    lcd_st.width_u16 = ST77916_WIDTH_U16;
    lcd_st.height_u16 = ST77916_HEIGHT_U16;
    lcd_st.rotation_u8 = ST77916_ROTATION_0_U8;

    ST77916_init(&lcd_st);
    ST77916_fill_screen(&lcd_st, ST77916_COLOUR_BLACK_U16);
    ST77916_fill_rect(&lcd_st, 20u, 20u, 80u, 40u,
                      ST77916_COLOUR_BLUE_U16);
    ST77916_draw_rect(&lcd_st, 18, 18, 84u, 44u,
                      ST77916_COLOUR_WHITE_U16);
    ST77916_draw_line(&lcd_st, 0, 0, 319, 384,
                      ST77916_COLOUR_RED_U16);
    ST77916_draw_circle(&lcd_st, 160, 192, 48u,
                        ST77916_COLOUR_GREEN_U16);
    ST77916_fill_circle(&lcd_st, 160, 192, 24u,
                        ST77916_COLOUR_YELLOW_U16);
}
