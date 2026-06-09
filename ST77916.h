/*****************************************************************************
 * Module: ST77916
 * File: ST77916.h
 * Description: Public interface for the ST77916 display driver.
 * Notes: Written for embedded C use with MISRA C:2025 review in mind.
 *****************************************************************************/

#ifndef ST77916_H
#define ST77916_H

#include "DEFS.h"

#define ST77916_SWRESET_U8             (0x01u)
#define ST77916_SLPOUT_U8              (0x11u)
#define ST77916_DISPON_U8              (0x29u)

#define ST77916_RESET_LOW_U8           (0u)
#define ST77916_RESET_HIGH_U8          (1u)

#define ST77916_DELAY_PULSE_MS_U32     (5u)
#define ST77916_DELAY_RESET_MS_U32     (20u)
#define ST77916_DELAY_READY_MS_U32     (120u)
#define ST77916_DELAY_DISPLAY_MS_U32   (20u)

typedef void (*ST77916_write_cmd_t)(U8 command_u8);
typedef void (*ST77916_write_data_t)(const U8 * data_pu8, U16 length_u16);
typedef void (*ST77916_delay_ms_t)(U32 delay_ms_u32);
typedef void (*ST77916_reset_pin_t)(U8 level_u8);

typedef struct
{
    ST77916_write_cmd_t write_cmd;
    ST77916_write_data_t write_data;
    ST77916_delay_ms_t delay_ms;
    ST77916_reset_pin_t reset_pin;
} ST77916_bus_st;

typedef struct
{
    ST77916_bus_st bus_st;
    U16 width_u16;
    U16 height_u16;
    U8 rotation_u8;
} ST77916_st;

void ST77916_init(ST77916_st * lcd_pst);
void ST77916_reset(ST77916_st * lcd_pst);

#endif
