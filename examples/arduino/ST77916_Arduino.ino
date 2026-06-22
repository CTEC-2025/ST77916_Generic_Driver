#include <SPI.h>

extern "C" {
#include "../../ST77916.h"
}

#define ST77916_PIN_CS                 (10u)
#define ST77916_PIN_DC                 (9u)
#define ST77916_PIN_RST                (8u)
#define ST77916_WIDTH_U16              (320u)
#define ST77916_HEIGHT_U16             (385u)
#define ST77916_PIXEL_COUNT_U32        (123200u)

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

    ST77916_init(&lcd_st);
    ST77916_fill_screen(&lcd_st, 0x0000u);
    ST77916_fill_rect(&lcd_st, 20u, 20u, 80u, 40u, 0xF800u);
    ST77916_draw_rect(&lcd_st, 18, 18, 84u, 44u, 0xFFFFu);
    ST77916_draw_line(&lcd_st, 0, 0, 319, 384, 0x07E0u);
    ST77916_draw_circle(&lcd_st, 160, 192, 48u, 0x001Fu);
    ST77916_fill_circle(&lcd_st, 160, 192, 24u, 0xFFE0u);
}

void loop(void)
{
}
