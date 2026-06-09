/*****************************************************************************
 * Module: ST77916
 * File: ST77916.c
 * Description: Reset and minimal initialization for ST77916 displays.
 * Notes: Written for embedded C use with MISRA C:2025 review in mind.
 *****************************************************************************/

#include "ST77916.h"

void ST77916_reset(ST77916_st * lcd_pst)
{
    if (lcd_pst != NULL)
    {
        if ((lcd_pst->bus_st.write_cmd != NULL) &&
            (lcd_pst->bus_st.delay_ms != NULL))
        {
            if (lcd_pst->bus_st.reset_pin != NULL)
            {
                lcd_pst->bus_st.reset_pin(ST77916_RESET_HIGH_U8);
                lcd_pst->bus_st.delay_ms(ST77916_DELAY_PULSE_MS_U32);

                lcd_pst->bus_st.reset_pin(ST77916_RESET_LOW_U8);
                lcd_pst->bus_st.delay_ms(ST77916_DELAY_RESET_MS_U32);

                lcd_pst->bus_st.reset_pin(ST77916_RESET_HIGH_U8);
                lcd_pst->bus_st.delay_ms(ST77916_DELAY_READY_MS_U32);
            }
            else
            {
                lcd_pst->bus_st.write_cmd(ST77916_SWRESET_U8);
                lcd_pst->bus_st.delay_ms(ST77916_DELAY_READY_MS_U32);
            }
        }
    }
}

void ST77916_init(ST77916_st * lcd_pst)
{
    if (lcd_pst != NULL)
    {
        if ((lcd_pst->bus_st.write_cmd != NULL) &&
            (lcd_pst->bus_st.delay_ms != NULL))
        {
            ST77916_reset(lcd_pst);

            lcd_pst->bus_st.write_cmd(ST77916_SLPOUT_U8);
            lcd_pst->bus_st.delay_ms(ST77916_DELAY_READY_MS_U32);

            lcd_pst->bus_st.write_cmd(ST77916_DISPON_U8);
            lcd_pst->bus_st.delay_ms(ST77916_DELAY_DISPLAY_MS_U32);
        }
    }
}
