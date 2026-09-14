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
static S16 ST77916_abs_s16(S16 value_s16);
static void ST77916_circle_points(ST77916_st * lcd_pst,
                                  S16 x_pos_s16,
                                  S16 y_pos_s16,
                                  S16 x_offset_s16,
                                  S16 y_offset_s16,
                                  U16 colour_u16);
static void ST77916_circle_spans(ST77916_st * lcd_pst,
                                 S16 x_pos_s16,
                                 S16 y_pos_s16,
                                 S16 x_offset_s16,
                                 S16 y_offset_s16,
                                 U16 colour_u16);

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

static S16 ST77916_abs_s16(S16 value_s16)
{
    S16 result_s16;

    result_s16 = value_s16;

    if (value_s16 < 0)
    {
        result_s16 = (S16)(0 - value_s16);
    }

    return result_s16;
}

static void ST77916_circle_points(ST77916_st * lcd_pst,
                                  S16 x_pos_s16,
                                  S16 y_pos_s16,
                                  S16 x_offset_s16,
                                  S16 y_offset_s16,
                                  U16 colour_u16)
{
    ST77916_draw_pixel(
        lcd_pst,
        (U16)(x_pos_s16 + x_offset_s16),
        (U16)(y_pos_s16 + y_offset_s16),
        colour_u16);
    ST77916_draw_pixel(
        lcd_pst,
        (U16)(x_pos_s16 - x_offset_s16),
        (U16)(y_pos_s16 + y_offset_s16),
        colour_u16);
    ST77916_draw_pixel(
        lcd_pst,
        (U16)(x_pos_s16 + x_offset_s16),
        (U16)(y_pos_s16 - y_offset_s16),
        colour_u16);
    ST77916_draw_pixel(
        lcd_pst,
        (U16)(x_pos_s16 - x_offset_s16),
        (U16)(y_pos_s16 - y_offset_s16),
        colour_u16);
    ST77916_draw_pixel(
        lcd_pst,
        (U16)(x_pos_s16 + y_offset_s16),
        (U16)(y_pos_s16 + x_offset_s16),
        colour_u16);
    ST77916_draw_pixel(
        lcd_pst,
        (U16)(x_pos_s16 - y_offset_s16),
        (U16)(y_pos_s16 + x_offset_s16),
        colour_u16);
    ST77916_draw_pixel(
        lcd_pst,
        (U16)(x_pos_s16 + y_offset_s16),
        (U16)(y_pos_s16 - x_offset_s16),
        colour_u16);
    ST77916_draw_pixel(
        lcd_pst,
        (U16)(x_pos_s16 - y_offset_s16),
        (U16)(y_pos_s16 - x_offset_s16),
        colour_u16);
}

static void ST77916_circle_spans(ST77916_st * lcd_pst,
                                 S16 x_pos_s16,
                                 S16 y_pos_s16,
                                 S16 x_offset_s16,
                                 S16 y_offset_s16,
                                 U16 colour_u16)
{
    ST77916_draw_vline(
        lcd_pst,
        (S16)(x_pos_s16 + x_offset_s16),
        (S16)(y_pos_s16 - y_offset_s16),
        (U16)((y_offset_s16 * 2) + 1),
        colour_u16);
    ST77916_draw_vline(
        lcd_pst,
        (S16)(x_pos_s16 - x_offset_s16),
        (S16)(y_pos_s16 - y_offset_s16),
        (U16)((y_offset_s16 * 2) + 1),
        colour_u16);
    ST77916_draw_vline(
        lcd_pst,
        (S16)(x_pos_s16 + y_offset_s16),
        (S16)(y_pos_s16 - x_offset_s16),
        (U16)((x_offset_s16 * 2) + 1),
        colour_u16);
    ST77916_draw_vline(
        lcd_pst,
        (S16)(x_pos_s16 - y_offset_s16),
        (S16)(y_pos_s16 - x_offset_s16),
        (U16)((x_offset_s16 * 2) + 1),
        colour_u16);
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

U16 ST77916_rgb565(U8 red_u8, U8 green_u8, U8 blue_u8)
{
    U16 colour_u16;

    colour_u16 = (U16)((U16)(red_u8 & ST77916_RED_MASK_U8) <<
        ST77916_RED_SHIFT_U8);
    colour_u16 |= (U16)((U16)(green_u8 & ST77916_GREEN_MASK_U8) <<
        ST77916_GREEN_SHIFT_U8);
    colour_u16 |= (U16)((U16)(blue_u8 & ST77916_BLUE_MASK_U8) >>
        ST77916_BLUE_SHIFT_U8);

    return colour_u16;
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

void ST77916_fill_screen(ST77916_st * lcd_pst, U16 colour_u16)
{
    U32 pixel_count_u32;
    U16 x_end_u16;
    U16 y_end_u16;

    if (ST77916_bus_ready(lcd_pst) == TRUE)
    {
        if ((lcd_pst->width_u16 > 0u) && (lcd_pst->height_u16 > 0u))
        {
            x_end_u16 = lcd_pst->width_u16 - 1u;
            y_end_u16 = lcd_pst->height_u16 - 1u;
            pixel_count_u32 = (U32)lcd_pst->width_u16;
            pixel_count_u32 *= (U32)lcd_pst->height_u16;

            ST77916_set_window(lcd_pst, 0u, 0u, x_end_u16, y_end_u16);
            ST77916_fill_colour(lcd_pst, colour_u16, pixel_count_u32);
        }
    }
}

void ST77916_draw_pixel(ST77916_st * lcd_pst,
                        U16 x_pos_u16,
                        U16 y_pos_u16,
                        U16 colour_u16)
{
    if (ST77916_bus_ready(lcd_pst) == TRUE)
    {
        if ((x_pos_u16 < lcd_pst->width_u16) &&
            (y_pos_u16 < lcd_pst->height_u16))
        {
            ST77916_set_window(
                lcd_pst,
                x_pos_u16,
                y_pos_u16,
                x_pos_u16,
                y_pos_u16);
            ST77916_fill_colour(lcd_pst, colour_u16, 1u);
        }
    }
}

void ST77916_fill_rect(ST77916_st * lcd_pst,
                       U16 x_pos_u16,
                       U16 y_pos_u16,
                       U16 width_u16,
                       U16 height_u16,
                       U16 colour_u16)
{
    U32 x_end_u32;
    U32 y_end_u32;
    U16 x_end_u16;
    U16 y_end_u16;
    U16 draw_width_u16;
    U16 draw_height_u16;
    U32 pixel_count_u32;

    if (ST77916_bus_ready(lcd_pst) == TRUE)
    {
        if ((width_u16 > 0u) && (height_u16 > 0u) &&
            (x_pos_u16 < lcd_pst->width_u16) &&
            (y_pos_u16 < lcd_pst->height_u16))
        {
            x_end_u32 = (U32)x_pos_u16 + (U32)width_u16 - 1u;
            y_end_u32 = (U32)y_pos_u16 + (U32)height_u16 - 1u;

            if (x_end_u32 >= (U32)lcd_pst->width_u16)
            {
                x_end_u16 = lcd_pst->width_u16 - 1u;
            }
            else
            {
                x_end_u16 = (U16)x_end_u32;
            }

            if (y_end_u32 >= (U32)lcd_pst->height_u16)
            {
                y_end_u16 = lcd_pst->height_u16 - 1u;
            }
            else
            {
                y_end_u16 = (U16)y_end_u32;
            }

            draw_width_u16 = x_end_u16 - x_pos_u16 + 1u;
            draw_height_u16 = y_end_u16 - y_pos_u16 + 1u;
            pixel_count_u32 = (U32)draw_width_u16;
            pixel_count_u32 *= (U32)draw_height_u16;

            ST77916_set_window(
                lcd_pst,
                x_pos_u16,
                y_pos_u16,
                x_end_u16,
                y_end_u16);
            ST77916_fill_colour(lcd_pst, colour_u16, pixel_count_u32);
        }
    }
}

void ST77916_draw_hline(ST77916_st * lcd_pst,
                        S16 x_pos_s16,
                        S16 y_pos_s16,
                        U16 length_u16,
                        U16 colour_u16)
{
    S16 x_start_s16;
    S16 x_end_s16;
    U16 width_u16;

    if (ST77916_bus_ready(lcd_pst) == TRUE)
    {
        if ((length_u16 > 0u) &&
            (y_pos_s16 >= 0) &&
            (y_pos_s16 < (S16)lcd_pst->height_u16))
        {
            x_start_s16 = x_pos_s16;
            x_end_s16 = (S16)(x_pos_s16 + (S16)length_u16 - 1);

            if ((x_end_s16 >= 0) &&
                (x_start_s16 < (S16)lcd_pst->width_u16))
            {
                if (x_start_s16 < 0)
                {
                    x_start_s16 = 0;
                }

                if (x_end_s16 >= (S16)lcd_pst->width_u16)
                {
                    x_end_s16 = (S16)lcd_pst->width_u16 - 1;
                }

                width_u16 = (U16)(x_end_s16 - x_start_s16 + 1);
                ST77916_fill_rect(
                    lcd_pst,
                    (U16)x_start_s16,
                    (U16)y_pos_s16,
                    width_u16,
                    1u,
                    colour_u16);
            }
        }
    }
}

void ST77916_draw_vline(ST77916_st * lcd_pst,
                        S16 x_pos_s16,
                        S16 y_pos_s16,
                        U16 length_u16,
                        U16 colour_u16)
{
    S16 y_start_s16;
    S16 y_end_s16;
    U16 height_u16;

    if (ST77916_bus_ready(lcd_pst) == TRUE)
    {
        if ((length_u16 > 0u) &&
            (x_pos_s16 >= 0) &&
            (x_pos_s16 < (S16)lcd_pst->width_u16))
        {
            y_start_s16 = y_pos_s16;
            y_end_s16 = (S16)(y_pos_s16 + (S16)length_u16 - 1);

            if ((y_end_s16 >= 0) &&
                (y_start_s16 < (S16)lcd_pst->height_u16))
            {
                if (y_start_s16 < 0)
                {
                    y_start_s16 = 0;
                }

                if (y_end_s16 >= (S16)lcd_pst->height_u16)
                {
                    y_end_s16 = (S16)lcd_pst->height_u16 - 1;
                }

                height_u16 = (U16)(y_end_s16 - y_start_s16 + 1);
                ST77916_fill_rect(
                    lcd_pst,
                    (U16)x_pos_s16,
                    (U16)y_start_s16,
                    1u,
                    height_u16,
                    colour_u16);
            }
        }
    }
}

void ST77916_draw_line(ST77916_st * lcd_pst,
                       S16 x_start_s16,
                       S16 y_start_s16,
                       S16 x_end_s16,
                       S16 y_end_s16,
                       U16 colour_u16)
{
    S16 dx_s16;
    S16 dy_s16;
    S16 step_x_s16;
    S16 step_y_s16;
    S16 error_s16;
    S16 twice_error_s16;
    U8 complete_u8;

    complete_u8 = FALSE;

    if (ST77916_bus_ready(lcd_pst) == TRUE)
    {
        dx_s16 = ST77916_abs_s16((S16)(x_end_s16 - x_start_s16));
        dy_s16 = (S16)(0 - ST77916_abs_s16(
            (S16)(y_end_s16 - y_start_s16)));
        step_x_s16 = -1;
        step_y_s16 = -1;

        if (x_start_s16 < x_end_s16)
        {
            step_x_s16 = 1;
        }

        if (y_start_s16 < y_end_s16)
        {
            step_y_s16 = 1;
        }

        error_s16 = (S16)(dx_s16 + dy_s16);

        while (complete_u8 == FALSE)
        {
            if ((x_start_s16 >= 0) && (y_start_s16 >= 0))
            {
                ST77916_draw_pixel(
                    lcd_pst,
                    (U16)x_start_s16,
                    (U16)y_start_s16,
                    colour_u16);
            }

            if ((x_start_s16 == x_end_s16) &&
                (y_start_s16 == y_end_s16))
            {
                complete_u8 = TRUE;
            }
            else
            {
                twice_error_s16 = (S16)(error_s16 * 2);

                if (twice_error_s16 >= dy_s16)
                {
                    error_s16 = (S16)(error_s16 + dy_s16);
                    x_start_s16 = (S16)(x_start_s16 + step_x_s16);
                }

                if (twice_error_s16 <= dx_s16)
                {
                    error_s16 = (S16)(error_s16 + dx_s16);
                    y_start_s16 = (S16)(y_start_s16 + step_y_s16);
                }
            }
        }
    }
}

void ST77916_draw_rect(ST77916_st * lcd_pst,
                       S16 x_pos_s16,
                       S16 y_pos_s16,
                       U16 width_u16,
                       U16 height_u16,
                       U16 colour_u16)
{
    if ((width_u16 > 0u) && (height_u16 > 0u))
    {
        ST77916_draw_hline(
            lcd_pst,
            x_pos_s16,
            y_pos_s16,
            width_u16,
            colour_u16);
        ST77916_draw_hline(
            lcd_pst,
            x_pos_s16,
            (S16)(y_pos_s16 + (S16)height_u16 - 1),
            width_u16,
            colour_u16);
        ST77916_draw_vline(
            lcd_pst,
            x_pos_s16,
            y_pos_s16,
            height_u16,
            colour_u16);
        ST77916_draw_vline(
            lcd_pst,
            (S16)(x_pos_s16 + (S16)width_u16 - 1),
            y_pos_s16,
            height_u16,
            colour_u16);
    }
}

void ST77916_draw_circle(ST77916_st * lcd_pst,
                         S16 x_pos_s16,
                         S16 y_pos_s16,
                         U16 radius_u16,
                         U16 colour_u16)
{
    S16 x_offset_s16;
    S16 y_offset_s16;
    S16 error_s16;

    if (ST77916_bus_ready(lcd_pst) == TRUE)
    {
        x_offset_s16 = 0;
        y_offset_s16 = (S16)radius_u16;
        error_s16 = (S16)(3 - ((S16)radius_u16 * 2));

        while (x_offset_s16 <= y_offset_s16)
        {
            ST77916_circle_points(
                lcd_pst,
                x_pos_s16,
                y_pos_s16,
                x_offset_s16,
                y_offset_s16,
                colour_u16);

            if (error_s16 < 0)
            {
                error_s16 = (S16)(
                    error_s16 + (x_offset_s16 * 4) + 6);
            }
            else
            {
                error_s16 = (S16)(
                    error_s16 +
                    ((x_offset_s16 - y_offset_s16) * 4) +
                    10);
                y_offset_s16--;
            }

            x_offset_s16++;
        }
    }
}

void ST77916_fill_circle(ST77916_st * lcd_pst,
                         S16 x_pos_s16,
                         S16 y_pos_s16,
                         U16 radius_u16,
                         U16 colour_u16)
{
    S16 x_offset_s16;
    S16 y_offset_s16;
    S16 error_s16;

    if (ST77916_bus_ready(lcd_pst) == TRUE)
    {
        x_offset_s16 = 0;
        y_offset_s16 = (S16)radius_u16;
        error_s16 = (S16)(3 - ((S16)radius_u16 * 2));

        while (x_offset_s16 <= y_offset_s16)
        {
            ST77916_circle_spans(
                lcd_pst,
                x_pos_s16,
                y_pos_s16,
                x_offset_s16,
                y_offset_s16,
                colour_u16);

            if (error_s16 < 0)
            {
                error_s16 = (S16)(
                    error_s16 + (x_offset_s16 * 4) + 6);
            }
            else
            {
                error_s16 = (S16)(
                    error_s16 +
                    ((x_offset_s16 - y_offset_s16) * 4) +
                    10);
                y_offset_s16--;
            }

            x_offset_s16++;
        }
    }
}
