#include <SPI.h>

extern "C" {
#include "../../ST77916.h"
}

#define ST77916_PIN_CS                 (10u)
#define ST77916_PIN_DC                 (9u)
#define ST77916_PIN_RST                (8u)
#define ST77916_WIDTH_U16              (320u)
#define ST77916_HEIGHT_U16             (385u)

static const U8 demo_bitmap_2x2_au8[8] =
{
    0xF8u, 0x00u, 0x07u, 0xE0u,
    0x00u, 0x1Fu, 0xFFu, 0xFFu
};

static ST77916_st lcd_st;

static void arduino_write_cmd(U8 command_u8)
{
    digitalWrite(ST77916_PIN_DC, LOW);
    digitalWrite(ST77916_PIN_CS, LOW);
    SPI.transfer(command_u8);
    digitalWrite(ST77916_PIN_CS, HIGH);
}

static void arduino_write_data(const U8 * data_pu8, U16 length_u16)
{
    U16 index_u16;

    if (data_pu8 != NULL)
    {
        digitalWrite(ST77916_PIN_DC, HIGH);
        digitalWrite(ST77916_PIN_CS, LOW);

        for (index_u16 = 0u; index_u16 < length_u16; index_u16++)
        {
            SPI.transfer(data_pu8[index_u16]);
        }

        digitalWrite(ST77916_PIN_CS, HIGH);
    }
}

static void arduino_delay_ms(U32 delay_ms_u32)
{
    delay(delay_ms_u32);
}

static void arduino_reset(U8 level_u8)
{
    if (level_u8 == ST77916_RESET_HIGH_U8)
    {
        digitalWrite(ST77916_PIN_RST, HIGH);
    }
    else
    {
        digitalWrite(ST77916_PIN_RST, LOW);
    }
}

void setup(void)
{
    pinMode(ST77916_PIN_CS, OUTPUT);
    pinMode(ST77916_PIN_DC, OUTPUT);
    pinMode(ST77916_PIN_RST, OUTPUT);

    digitalWrite(ST77916_PIN_CS, HIGH);
    SPI.begin();

    lcd_st.bus_st.write_cmd = arduino_write_cmd;
    lcd_st.bus_st.write_data = arduino_write_data;
    lcd_st.bus_st.delay_ms = arduino_delay_ms;
    lcd_st.bus_st.reset_pin = arduino_reset;
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
    (void)ST77916_draw_string(&lcd_st, 20, 24, "ARDUINO",
                              ST77916_COLOUR_GREEN_U16,
                              ST77916_COLOUR_BLACK_U16);
}

void loop(void)
{
}
