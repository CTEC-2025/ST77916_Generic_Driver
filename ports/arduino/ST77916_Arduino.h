/*****************************************************************************
 * Module: ST77916 Arduino port
 * File: ST77916_Arduino.h
 * Description: Arduino SPI adapter interface for the ST77916 driver.
 *****************************************************************************/

#ifndef ST77916_ARDUINO_H
#define ST77916_ARDUINO_H

#include "../../ST77916.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    ST77916_st lcd_st;
    U8 cs_pin_u8;
    U8 dc_pin_u8;
    U8 reset_pin_u8;
} ST77916_arduino_st;

void ST77916_arduino_begin(ST77916_arduino_st * port_pst,
                           U8 cs_pin_u8,
                           U8 dc_pin_u8,
                           U8 reset_pin_u8,
                           U16 width_u16,
                           U16 height_u16);
ST77916_st * ST77916_arduino_lcd(ST77916_arduino_st * port_pst);

#ifdef __cplusplus
}
#endif

#endif
