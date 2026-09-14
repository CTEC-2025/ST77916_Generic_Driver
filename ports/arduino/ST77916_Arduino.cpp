/*****************************************************************************
 * Module: ST77916 Arduino port
 * File: ST77916_Arduino.cpp
 * Description: Arduino SPI adapter implementation for the ST77916 driver.
 *****************************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include "ST77916_Arduino.h"

static ST77916_arduino_st * active_port_pst = NULL;

static void arduino_write_cmd(U8 command_u8)
{
    if (active_port_pst != NULL)
    {
        digitalWrite(active_port_pst->dc_pin_u8, LOW);
        digitalWrite(active_port_pst->cs_pin_u8, LOW);
        SPI.transfer(command_u8);
        digitalWrite(active_port_pst->cs_pin_u8, HIGH);
    }
}

static void arduino_write_data(const U8 * data_pu8, U16 length_u16)
{
    U16 index_u16;

    if ((active_port_pst != NULL) && (data_pu8 != NULL))
    {
        digitalWrite(active_port_pst->dc_pin_u8, HIGH);
        digitalWrite(active_port_pst->cs_pin_u8, LOW);

        for (index_u16 = 0u; index_u16 < length_u16; index_u16++)
        {
            SPI.transfer(data_pu8[index_u16]);
        }

        digitalWrite(active_port_pst->cs_pin_u8, HIGH);
    }
}

static void arduino_delay_ms(U32 delay_ms_u32)
{
    delay(delay_ms_u32);
}

static void arduino_reset(U8 level_u8)
{
    if (active_port_pst != NULL)
    {
        if (level_u8 == ST77916_RESET_HIGH_U8)
        {
            digitalWrite(active_port_pst->reset_pin_u8, HIGH);
        }
        else
        {
            digitalWrite(active_port_pst->reset_pin_u8, LOW);
        }
    }
}

void ST77916_arduino_begin(ST77916_arduino_st * port_pst,
                           U8 cs_pin_u8,
                           U8 dc_pin_u8,
                           U8 reset_pin_u8,
                           U16 width_u16,
                           U16 height_u16)
{
    if (port_pst != NULL)
    {
        active_port_pst = port_pst;

        port_pst->cs_pin_u8 = cs_pin_u8;
        port_pst->dc_pin_u8 = dc_pin_u8;
        port_pst->reset_pin_u8 = reset_pin_u8;

        pinMode(cs_pin_u8, OUTPUT);
        pinMode(dc_pin_u8, OUTPUT);
        pinMode(reset_pin_u8, OUTPUT);
        digitalWrite(cs_pin_u8, HIGH);
        SPI.begin();

        port_pst->lcd_st.bus_st.write_cmd = arduino_write_cmd;
        port_pst->lcd_st.bus_st.write_data = arduino_write_data;
        port_pst->lcd_st.bus_st.delay_ms = arduino_delay_ms;
        port_pst->lcd_st.bus_st.reset_pin = arduino_reset;
        port_pst->lcd_st.width_u16 = width_u16;
        port_pst->lcd_st.height_u16 = height_u16;
        port_pst->lcd_st.rotation_u8 = ST77916_ROTATION_0_U8;

        ST77916_init(&port_pst->lcd_st);
    }
}

ST77916_st * ST77916_arduino_lcd(ST77916_arduino_st * port_pst)
{
    ST77916_st * lcd_pst;

    lcd_pst = NULL;

    if (port_pst != NULL)
    {
        lcd_pst = &port_pst->lcd_st;
    }

    return lcd_pst;
}
