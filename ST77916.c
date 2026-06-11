/*****************************************************************************
 * Module: ST77916
 * File: ST77916.c
 * Description: Reset and minimal initialization for ST77916 displays.
 * Notes: Written for embedded C use with MISRA C:2025 review in mind.
 *****************************************************************************/

#include "ST77916.h"

static void ST77916_write_u16(U8 * data_pu8, U16 value_u16);
static U8 ST77916_get_madctl(U8 rotation_u8);
static U8 ST77916_bus_ready(const ST77916_st * lcd_pst);

static void ST77916_write_u16(U8 * data_pu8, U16 value_u16)
{
    if (data_pu8 != NULL)
    {
        data_pu8[0u] = (U8)(value_u16 >> 8u);
        data_pu8[1u] = (U8)(value_u16 & ST77916_LOW_BYTE_MASK_U16);
    }
}

static U8 ST77916_get_madctl(U8 rotation_u8)
{
    U8 madctl_u8;

    madctl_u8 = ST77916_MADCTL_0_U8;

    if (rotation_u8 == ST77916_ROTATION_90_U8)
    {
        madctl_u8 = ST77916_MADCTL_90_U8;
    }
    else if (rotation_u8 == ST77916_ROTATION_180_U8)
    {
        madctl_u8 = ST77916_MADCTL_180_U8;
    }
    else if (rotation_u8 == ST77916_ROTATION_270_U8)
    {
        madctl_u8 = ST77916_MADCTL_270_U8;
    }
    else
    {
        madctl_u8 = ST77916_MADCTL_0_U8;
    }

    return madctl_u8;
}

static U8 ST77916_bus_ready(const ST77916_st * lcd_pst)
{
    U8 ready_u8;

    ready_u8 = FALSE;

    if (lcd_pst != NULL)
    {
        if ((lcd_pst->bus_st.write_cmd != NULL) &&
            (lcd_pst->bus_st.write_data != NULL) &&
            (lcd_pst->bus_st.delay_ms != NULL))
        {
            ready_u8 = TRUE;
        }
    }

    return ready_u8;
}

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
        if (ST77916_bus_ready(lcd_pst) == TRUE)
        {
            ST77916_reset(lcd_pst);

            ST77916_set_rotation(lcd_pst, lcd_pst->rotation_u8);

            lcd_pst->bus_st.write_cmd(ST77916_SLPOUT_U8);
            lcd_pst->bus_st.delay_ms(ST77916_DELAY_READY_MS_U32);

            lcd_pst->bus_st.write_cmd(ST77916_DISPON_U8);
            lcd_pst->bus_st.delay_ms(ST77916_DELAY_DISPLAY_MS_U32);
        }
    }
}

void ST77916_set_rotation(ST77916_st * lcd_pst, U8 rotation_u8)
{
    U8 madctl_u8;

    if (ST77916_bus_ready(lcd_pst) == TRUE)
    {
        lcd_pst->rotation_u8 = rotation_u8 % ST77916_ROTATION_COUNT_U8;
        madctl_u8 = ST77916_get_madctl(lcd_pst->rotation_u8);

        lcd_pst->bus_st.write_cmd(ST77916_MADCTL_U8);
        lcd_pst->bus_st.write_data(&madctl_u8, ST77916_BYTE_BYTES_U16);
    }
}

void ST77916_set_window(ST77916_st * lcd_pst,
                        U16 x_start_u16,
                        U16 y_start_u16,
                        U16 x_end_u16,
                        U16 y_end_u16)
{
    U8 data_au8[ST77916_ADDR_BYTES_U16];

    if (ST77916_bus_ready(lcd_pst) == TRUE)
    {
        ST77916_write_u16(&data_au8[0u], x_start_u16);
        ST77916_write_u16(&data_au8[ST77916_WORD_BYTES_U16], x_end_u16);

        lcd_pst->bus_st.write_cmd(ST77916_CASET_U8);
        lcd_pst->bus_st.write_data(data_au8, ST77916_ADDR_BYTES_U16);

        ST77916_write_u16(&data_au8[0u], y_start_u16);
        ST77916_write_u16(&data_au8[ST77916_WORD_BYTES_U16], y_end_u16);

        lcd_pst->bus_st.write_cmd(ST77916_RASET_U8);
        lcd_pst->bus_st.write_data(data_au8, ST77916_ADDR_BYTES_U16);

        lcd_pst->bus_st.write_cmd(ST77916_RAMWR_U8);
    }
}

void ST77916_write_pixels(ST77916_st * lcd_pst,
                          const U8 * pixels_pu8,
                          U16 length_u16)
{
    if (ST77916_bus_ready(lcd_pst) == TRUE)
    {
        if (pixels_pu8 != NULL)
        {
            if (length_u16 > 0u)
            {
                lcd_pst->bus_st.write_data(pixels_pu8, length_u16);
            }
        }
    }
}

void ST77916_fill_colour(ST77916_st * lcd_pst,
                         U16 colour_u16,
                         U32 pixel_count_u32)
{
    U8 data_au8[ST77916_FILL_BYTES_U16];
    U16 index_u16;
    U16 chunk_pixels_u16;

    index_u16 = 0u;

    if (ST77916_bus_ready(lcd_pst) == TRUE)
    {
        while (index_u16 < ST77916_FILL_BYTES_U16)
        {
            ST77916_write_u16(&data_au8[index_u16], colour_u16);
            index_u16 += ST77916_WORD_BYTES_U16;
        }

        while (pixel_count_u32 > 0u)
        {
            chunk_pixels_u16 = ST77916_FILL_PIXELS_U16;

            if (pixel_count_u32 < (U32)ST77916_FILL_PIXELS_U16)
            {
                chunk_pixels_u16 = (U16)pixel_count_u32;
            }

            lcd_pst->bus_st.write_data(
                data_au8,
                (U16)(chunk_pixels_u16 * ST77916_WORD_BYTES_U16));

            pixel_count_u32 -= (U32)chunk_pixels_u16;
        }
    }
}
