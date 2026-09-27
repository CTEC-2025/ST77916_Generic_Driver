#include <avr/io.h>
#ifndef F_CPU
#define F_CPU                          (8000000UL)
#endif
#include <util/delay.h>
#include "../../ST77916.h"

#define ST77916_WIDTH_U16              (320u)
#define ST77916_HEIGHT_U16             (385u)

static const U8 demo_bitmap_2x2_au8[8] =
{
    0xF8u, 0x00u, 0x07u, 0xE0u,
    0x00u, 0x1Fu, 0xFFu, 0xFFu
};

#define ST77916_PORT                   PORTB
#define ST77916_DDR                    DDRB
#define ST77916_CS_BIT                 PB3
#define ST77916_DC_BIT                 PB2
#define ST77916_RST_BIT                PB1
#define ST77916_SCK_BIT                PB0
#define ST77916_MOSI_BIT               PB4

static ST77916_st lcd_st;

static void attiny_pin_high(U8 bit_u8)
{
    ST77916_PORT |= (U8)(1u << bit_u8);
}

static void attiny_pin_low(U8 bit_u8)
{
    ST77916_PORT &= (U8)(~(1u << bit_u8));
}

static void attiny_spi_write(U8 value_u8)
{
    U8 bit_count_u8;

    for (bit_count_u8 = 0u; bit_count_u8 < 8u; bit_count_u8++)
    {
        if ((value_u8 & 0x80u) != 0u)
        {
            attiny_pin_high(ST77916_MOSI_BIT);
        }
        else
        {
            attiny_pin_low(ST77916_MOSI_BIT);
        }

        attiny_pin_high(ST77916_SCK_BIT);
        attiny_pin_low(ST77916_SCK_BIT);
        value_u8 <<= 1u;
    }
}

static void attiny_write_cmd(U8 command_u8)
{
    attiny_pin_low(ST77916_DC_BIT);
    attiny_pin_low(ST77916_CS_BIT);
    attiny_spi_write(command_u8);
    attiny_pin_high(ST77916_CS_BIT);
}

static void attiny_write_data(const U8 * data_pu8, U16 length_u16)
{
    U16 index_u16;

    if (data_pu8 != NULL)
    {
        attiny_pin_high(ST77916_DC_BIT);
        attiny_pin_low(ST77916_CS_BIT);

        for (index_u16 = 0u; index_u16 < length_u16; index_u16++)
        {
            attiny_spi_write(data_pu8[index_u16]);
        }

        attiny_pin_high(ST77916_CS_BIT);
    }
}

static void attiny_delay_ms(U32 delay_ms_u32)
{
    while (delay_ms_u32 > 0u)
    {
        _delay_ms(1.0);
        delay_ms_u32--;
    }
}

static void attiny_reset(U8 level_u8)
{
    if (level_u8 == ST77916_RESET_HIGH_U8)
    {
        attiny_pin_high(ST77916_RST_BIT);
    }
    else
    {
        attiny_pin_low(ST77916_RST_BIT);
    }
}

void app_display_init(void)
{
    ST77916_DDR |= (U8)(1u << ST77916_CS_BIT);
    ST77916_DDR |= (U8)(1u << ST77916_DC_BIT);
    ST77916_DDR |= (U8)(1u << ST77916_RST_BIT);
    ST77916_DDR |= (U8)(1u << ST77916_SCK_BIT);
    ST77916_DDR |= (U8)(1u << ST77916_MOSI_BIT);

    lcd_st.bus_st.write_cmd = attiny_write_cmd;
    lcd_st.bus_st.write_data = attiny_write_data;
    lcd_st.bus_st.delay_ms = attiny_delay_ms;
    lcd_st.bus_st.reset_pin = attiny_reset;
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
    (void)ST77916_draw_string(&lcd_st, 20, 24, "ATTINY",
                              ST77916_COLOUR_GREEN_U16,
                              ST77916_COLOUR_BLACK_U16);
}
