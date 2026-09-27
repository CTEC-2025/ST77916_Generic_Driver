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
static ST77916_status_e ST77916_check_bus(const ST77916_st * lcd_pst);
static S16 ST77916_abs_s16(S16 value_s16);
static void ST77916_get_font(char character_c, U8 * glyph_pu8);
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

static ST77916_status_e ST77916_check_bus(const ST77916_st * lcd_pst)
{
    ST77916_status_e status_e;

    status_e = ST77916_STATUS_OK;

    if (lcd_pst == NULL)
    {
        status_e = ST77916_STATUS_NULL;
    }
    else if (ST77916_bus_ready(lcd_pst) == FALSE)
    {
        status_e = ST77916_STATUS_BUS;
    }
    else
    {
        status_e = ST77916_STATUS_OK;
    }

    return status_e;
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

static void ST77916_get_font(char character_c, U8 * glyph_pu8)
{
    U8 index_u8;

    if (glyph_pu8 != NULL)
    {
        for (index_u8 = 0u; index_u8 < ST77916_CFG_FONT_WIDTH_U8; index_u8++)
        {
            glyph_pu8[index_u8] = 0u;
        }

        switch (character_c)
        {
            case '0': glyph_pu8[0u] = 0x3Eu; glyph_pu8[1u] = 0x51u;
                      glyph_pu8[2u] = 0x49u; glyph_pu8[3u] = 0x45u;
                      glyph_pu8[4u] = 0x3Eu; break;
            case '1': glyph_pu8[0u] = 0x00u; glyph_pu8[1u] = 0x42u;
                      glyph_pu8[2u] = 0x7Fu; glyph_pu8[3u] = 0x40u;
                      glyph_pu8[4u] = 0x00u; break;
            case '2': glyph_pu8[0u] = 0x42u; glyph_pu8[1u] = 0x61u;
                      glyph_pu8[2u] = 0x51u; glyph_pu8[3u] = 0x49u;
                      glyph_pu8[4u] = 0x46u; break;
            case '3': glyph_pu8[0u] = 0x21u; glyph_pu8[1u] = 0x41u;
                      glyph_pu8[2u] = 0x45u; glyph_pu8[3u] = 0x4Bu;
                      glyph_pu8[4u] = 0x31u; break;
            case '4': glyph_pu8[0u] = 0x18u; glyph_pu8[1u] = 0x14u;
                      glyph_pu8[2u] = 0x12u; glyph_pu8[3u] = 0x7Fu;
                      glyph_pu8[4u] = 0x10u; break;
            case '5': glyph_pu8[0u] = 0x27u; glyph_pu8[1u] = 0x45u;
                      glyph_pu8[2u] = 0x45u; glyph_pu8[3u] = 0x45u;
                      glyph_pu8[4u] = 0x39u; break;
            case '6': glyph_pu8[0u] = 0x3Cu; glyph_pu8[1u] = 0x4Au;
                      glyph_pu8[2u] = 0x49u; glyph_pu8[3u] = 0x49u;
                      glyph_pu8[4u] = 0x30u; break;
            case '7': glyph_pu8[0u] = 0x01u; glyph_pu8[1u] = 0x71u;
                      glyph_pu8[2u] = 0x09u; glyph_pu8[3u] = 0x05u;
                      glyph_pu8[4u] = 0x03u; break;
            case '8': glyph_pu8[0u] = 0x36u; glyph_pu8[1u] = 0x49u;
                      glyph_pu8[2u] = 0x49u; glyph_pu8[3u] = 0x49u;
                      glyph_pu8[4u] = 0x36u; break;
            case '9': glyph_pu8[0u] = 0x06u; glyph_pu8[1u] = 0x49u;
                      glyph_pu8[2u] = 0x49u; glyph_pu8[3u] = 0x29u;
                      glyph_pu8[4u] = 0x1Eu; break;
            case 'A': glyph_pu8[0u] = 0x7Eu; glyph_pu8[1u] = 0x11u;
                      glyph_pu8[2u] = 0x11u; glyph_pu8[3u] = 0x11u;
                      glyph_pu8[4u] = 0x7Eu; break;
            case 'B': glyph_pu8[0u] = 0x7Fu; glyph_pu8[1u] = 0x49u;
                      glyph_pu8[2u] = 0x49u; glyph_pu8[3u] = 0x49u;
                      glyph_pu8[4u] = 0x36u; break;
            case 'C': glyph_pu8[0u] = 0x3Eu; glyph_pu8[1u] = 0x41u;
                      glyph_pu8[2u] = 0x41u; glyph_pu8[3u] = 0x41u;
                      glyph_pu8[4u] = 0x22u; break;
            case 'D': glyph_pu8[0u] = 0x7Fu; glyph_pu8[1u] = 0x41u;
                      glyph_pu8[2u] = 0x41u; glyph_pu8[3u] = 0x22u;
                      glyph_pu8[4u] = 0x1Cu; break;
            case 'E': glyph_pu8[0u] = 0x7Fu; glyph_pu8[1u] = 0x49u;
                      glyph_pu8[2u] = 0x49u; glyph_pu8[3u] = 0x49u;
                      glyph_pu8[4u] = 0x41u; break;
            case 'F': glyph_pu8[0u] = 0x7Fu; glyph_pu8[1u] = 0x09u;
                      glyph_pu8[2u] = 0x09u; glyph_pu8[3u] = 0x09u;
                      glyph_pu8[4u] = 0x01u; break;
            case 'G': glyph_pu8[0u] = 0x3Eu; glyph_pu8[1u] = 0x41u;
                      glyph_pu8[2u] = 0x49u; glyph_pu8[3u] = 0x49u;
                      glyph_pu8[4u] = 0x7Au; break;
            case 'H': glyph_pu8[0u] = 0x7Fu; glyph_pu8[1u] = 0x08u;
                      glyph_pu8[2u] = 0x08u; glyph_pu8[3u] = 0x08u;
                      glyph_pu8[4u] = 0x7Fu; break;
            case 'I': glyph_pu8[0u] = 0x00u; glyph_pu8[1u] = 0x41u;
                      glyph_pu8[2u] = 0x7Fu; glyph_pu8[3u] = 0x41u;
                      glyph_pu8[4u] = 0x00u; break;
            case 'J': glyph_pu8[0u] = 0x20u; glyph_pu8[1u] = 0x40u;
                      glyph_pu8[2u] = 0x41u; glyph_pu8[3u] = 0x3Fu;
                      glyph_pu8[4u] = 0x01u; break;
            case 'K': glyph_pu8[0u] = 0x7Fu; glyph_pu8[1u] = 0x08u;
                      glyph_pu8[2u] = 0x14u; glyph_pu8[3u] = 0x22u;
                      glyph_pu8[4u] = 0x41u; break;
            case 'L': glyph_pu8[0u] = 0x7Fu; glyph_pu8[1u] = 0x40u;
                      glyph_pu8[2u] = 0x40u; glyph_pu8[3u] = 0x40u;
                      glyph_pu8[4u] = 0x40u; break;
            case 'M': glyph_pu8[0u] = 0x7Fu; glyph_pu8[1u] = 0x02u;
                      glyph_pu8[2u] = 0x0Cu; glyph_pu8[3u] = 0x02u;
                      glyph_pu8[4u] = 0x7Fu; break;
            case 'N': glyph_pu8[0u] = 0x7Fu; glyph_pu8[1u] = 0x04u;
                      glyph_pu8[2u] = 0x08u; glyph_pu8[3u] = 0x10u;
                      glyph_pu8[4u] = 0x7Fu; break;
            case 'O': glyph_pu8[0u] = 0x3Eu; glyph_pu8[1u] = 0x41u;
                      glyph_pu8[2u] = 0x41u; glyph_pu8[3u] = 0x41u;
                      glyph_pu8[4u] = 0x3Eu; break;
            case 'P': glyph_pu8[0u] = 0x7Fu; glyph_pu8[1u] = 0x09u;
                      glyph_pu8[2u] = 0x09u; glyph_pu8[3u] = 0x09u;
                      glyph_pu8[4u] = 0x06u; break;
            case 'Q': glyph_pu8[0u] = 0x3Eu; glyph_pu8[1u] = 0x41u;
                      glyph_pu8[2u] = 0x51u; glyph_pu8[3u] = 0x21u;
                      glyph_pu8[4u] = 0x5Eu; break;
            case 'R': glyph_pu8[0u] = 0x7Fu; glyph_pu8[1u] = 0x09u;
                      glyph_pu8[2u] = 0x19u; glyph_pu8[3u] = 0x29u;
                      glyph_pu8[4u] = 0x46u; break;
            case 'S': glyph_pu8[0u] = 0x46u; glyph_pu8[1u] = 0x49u;
                      glyph_pu8[2u] = 0x49u; glyph_pu8[3u] = 0x49u;
                      glyph_pu8[4u] = 0x31u; break;
            case 'T': glyph_pu8[0u] = 0x01u; glyph_pu8[1u] = 0x01u;
                      glyph_pu8[2u] = 0x7Fu; glyph_pu8[3u] = 0x01u;
                      glyph_pu8[4u] = 0x01u; break;
            case 'U': glyph_pu8[0u] = 0x3Fu; glyph_pu8[1u] = 0x40u;
                      glyph_pu8[2u] = 0x40u; glyph_pu8[3u] = 0x40u;
                      glyph_pu8[4u] = 0x3Fu; break;
            case 'V': glyph_pu8[0u] = 0x1Fu; glyph_pu8[1u] = 0x20u;
                      glyph_pu8[2u] = 0x40u; glyph_pu8[3u] = 0x20u;
                      glyph_pu8[4u] = 0x1Fu; break;
            case 'W': glyph_pu8[0u] = 0x3Fu; glyph_pu8[1u] = 0x40u;
                      glyph_pu8[2u] = 0x38u; glyph_pu8[3u] = 0x40u;
                      glyph_pu8[4u] = 0x3Fu; break;
            case 'X': glyph_pu8[0u] = 0x63u; glyph_pu8[1u] = 0x14u;
                      glyph_pu8[2u] = 0x08u; glyph_pu8[3u] = 0x14u;
                      glyph_pu8[4u] = 0x63u; break;
            case 'Y': glyph_pu8[0u] = 0x07u; glyph_pu8[1u] = 0x08u;
                      glyph_pu8[2u] = 0x70u; glyph_pu8[3u] = 0x08u;
                      glyph_pu8[4u] = 0x07u; break;
            case 'Z': glyph_pu8[0u] = 0x61u; glyph_pu8[1u] = 0x51u;
                      glyph_pu8[2u] = 0x49u; glyph_pu8[3u] = 0x45u;
                      glyph_pu8[4u] = 0x43u; break;
            case '-': glyph_pu8[0u] = 0x08u; glyph_pu8[1u] = 0x08u;
                      glyph_pu8[2u] = 0x08u; glyph_pu8[3u] = 0x08u;
                      glyph_pu8[4u] = 0x08u; break;
            case '.': glyph_pu8[2u] = 0x60u; break;
            case ':': glyph_pu8[2u] = 0x36u; break;
            case ' ': break;
            default: glyph_pu8[0u] = 0x02u; glyph_pu8[1u] = 0x01u;
                     glyph_pu8[2u] = 0x51u; glyph_pu8[3u] = 0x09u;
                     glyph_pu8[4u] = 0x06u; break;
        }
    }
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

ST77916_status_e ST77916_reset_ex(ST77916_st * lcd_pst)
{
    ST77916_status_e status_e;

    status_e = ST77916_STATUS_OK;

    if (lcd_pst == NULL)
    {
        status_e = ST77916_STATUS_NULL;
    }
    else if ((lcd_pst->bus_st.write_cmd == NULL) ||
             (lcd_pst->bus_st.delay_ms == NULL))
    {
        status_e = ST77916_STATUS_BUS;
    }
    else
    {
        ST77916_reset(lcd_pst);
    }

    return status_e;
}

ST77916_status_e ST77916_init_ex(ST77916_st * lcd_pst)
{
    ST77916_status_e status_e;

    status_e = ST77916_check_bus(lcd_pst);

    if (status_e == ST77916_STATUS_OK)
    {
        ST77916_init(lcd_pst);
    }

    return status_e;
}

ST77916_status_e ST77916_set_rotation_ex(ST77916_st * lcd_pst,
                                          U8 rotation_u8)
{
    ST77916_status_e status_e;

    status_e = ST77916_check_bus(lcd_pst);

    if (status_e == ST77916_STATUS_OK)
    {
        ST77916_set_rotation(lcd_pst, rotation_u8);
    }

    return status_e;
}

ST77916_status_e ST77916_set_window_ex(ST77916_st * lcd_pst,
                                       U16 x_start_u16,
                                       U16 y_start_u16,
                                       U16 x_end_u16,
                                       U16 y_end_u16)
{
    ST77916_status_e status_e;

    status_e = ST77916_check_bus(lcd_pst);

    if (status_e == ST77916_STATUS_OK)
    {
        if ((x_start_u16 > x_end_u16) ||
            (y_start_u16 > y_end_u16) ||
            (x_end_u16 >= lcd_pst->width_u16) ||
            (y_end_u16 >= lcd_pst->height_u16))
        {
            status_e = ST77916_STATUS_BOUNDS;
        }
        else
        {
            ST77916_set_window(
                lcd_pst,
                x_start_u16,
                y_start_u16,
                x_end_u16,
                y_end_u16);
        }
    }

    return status_e;
}

ST77916_status_e ST77916_write_pixels_ex(ST77916_st * lcd_pst,
                                         const U8 * pixels_pu8,
                                         U16 length_u16)
{
    ST77916_status_e status_e;

    status_e = ST77916_check_bus(lcd_pst);

    if (status_e == ST77916_STATUS_OK)
    {
        if ((pixels_pu8 == NULL) || (length_u16 == 0u))
        {
            status_e = ST77916_STATUS_PARAM;
        }
        else
        {
            ST77916_write_pixels(lcd_pst, pixels_pu8, length_u16);
        }
    }

    return status_e;
}

ST77916_status_e ST77916_fill_colour_ex(ST77916_st * lcd_pst,
                                        U16 colour_u16,
                                        U32 pixel_count_u32)
{
    ST77916_status_e status_e;

    status_e = ST77916_check_bus(lcd_pst);

    if (status_e == ST77916_STATUS_OK)
    {
        if (pixel_count_u32 == 0u)
        {
            status_e = ST77916_STATUS_PARAM;
        }
        else
        {
            ST77916_fill_colour(lcd_pst, colour_u16, pixel_count_u32);
        }
    }

    return status_e;
}

ST77916_status_e ST77916_fill_screen_ex(ST77916_st * lcd_pst,
                                        U16 colour_u16)
{
    ST77916_status_e status_e;

    status_e = ST77916_check_bus(lcd_pst);

    if (status_e == ST77916_STATUS_OK)
    {
        if ((lcd_pst->width_u16 == 0u) || (lcd_pst->height_u16 == 0u))
        {
            status_e = ST77916_STATUS_PARAM;
        }
        else
        {
            ST77916_fill_screen(lcd_pst, colour_u16);
        }
    }

    return status_e;
}

ST77916_status_e ST77916_draw_pixel_ex(ST77916_st * lcd_pst,
                                       U16 x_pos_u16,
                                       U16 y_pos_u16,
                                       U16 colour_u16)
{
    ST77916_status_e status_e;

    status_e = ST77916_check_bus(lcd_pst);

    if (status_e == ST77916_STATUS_OK)
    {
        if ((x_pos_u16 >= lcd_pst->width_u16) ||
            (y_pos_u16 >= lcd_pst->height_u16))
        {
            status_e = ST77916_STATUS_BOUNDS;
        }
        else
        {
            ST77916_draw_pixel(
                lcd_pst,
                x_pos_u16,
                y_pos_u16,
                colour_u16);
        }
    }

    return status_e;
}

ST77916_status_e ST77916_fill_rect_ex(ST77916_st * lcd_pst,
                                      U16 x_pos_u16,
                                      U16 y_pos_u16,
                                      U16 width_u16,
                                      U16 height_u16,
                                      U16 colour_u16)
{
    ST77916_status_e status_e;

    status_e = ST77916_check_bus(lcd_pst);

    if (status_e == ST77916_STATUS_OK)
    {
        if ((width_u16 == 0u) || (height_u16 == 0u))
        {
            status_e = ST77916_STATUS_PARAM;
        }
        else if ((x_pos_u16 >= lcd_pst->width_u16) ||
                 (y_pos_u16 >= lcd_pst->height_u16))
        {
            status_e = ST77916_STATUS_BOUNDS;
        }
        else
        {
            ST77916_fill_rect(
                lcd_pst,
                x_pos_u16,
                y_pos_u16,
                width_u16,
                height_u16,
                colour_u16);
        }
    }

    return status_e;
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

ST77916_status_e ST77916_draw_bitmap(ST77916_st * lcd_pst,
                                     S16 x_pos_s16,
                                     S16 y_pos_s16,
                                     U16 width_u16,
                                     U16 height_u16,
                                     const U8 * pixels_pu8)
{
    ST77916_status_e status_e;
    S16 x_start_s16;
    S16 y_start_s16;
    U16 draw_width_u16;
    U16 draw_height_u16;
    U16 row_u16;
    U16 src_x_u16;
    U16 src_y_u16;
    U32 byte_offset_u32;

    status_e = ST77916_check_bus(lcd_pst);

    if (status_e == ST77916_STATUS_OK)
    {
        if ((pixels_pu8 == NULL) || (width_u16 == 0u) ||
            (height_u16 == 0u))
        {
            status_e = ST77916_STATUS_PARAM;
        }
        else if (((x_pos_s16 + (S16)width_u16) <= 0) ||
                 ((y_pos_s16 + (S16)height_u16) <= 0) ||
                 (x_pos_s16 >= (S16)lcd_pst->width_u16) ||
                 (y_pos_s16 >= (S16)lcd_pst->height_u16))
        {
            status_e = ST77916_STATUS_BOUNDS;
        }
        else
        {
            x_start_s16 = x_pos_s16;
            y_start_s16 = y_pos_s16;
            src_x_u16 = 0u;
            src_y_u16 = 0u;
            draw_width_u16 = width_u16;
            draw_height_u16 = height_u16;

            if (x_start_s16 < 0)
            {
                src_x_u16 = (U16)(0 - x_start_s16);
                draw_width_u16 = (U16)(draw_width_u16 - src_x_u16);
                x_start_s16 = 0;
            }

            if (y_start_s16 < 0)
            {
                src_y_u16 = (U16)(0 - y_start_s16);
                draw_height_u16 = (U16)(draw_height_u16 - src_y_u16);
                y_start_s16 = 0;
            }

            if (((U16)x_start_s16 + draw_width_u16) >
                lcd_pst->width_u16)
            {
                draw_width_u16 = (U16)(
                    lcd_pst->width_u16 - (U16)x_start_s16);
            }

            if (((U16)y_start_s16 + draw_height_u16) >
                lcd_pst->height_u16)
            {
                draw_height_u16 = (U16)(
                    lcd_pst->height_u16 - (U16)y_start_s16);
            }

            for (row_u16 = 0u; row_u16 < draw_height_u16; row_u16++)
            {
                byte_offset_u32 = (U32)(src_y_u16 + row_u16);
                byte_offset_u32 *= (U32)width_u16;
                byte_offset_u32 += (U32)src_x_u16;
                byte_offset_u32 *= (U32)ST77916_WORD_BYTES_U16;

                ST77916_set_window(
                    lcd_pst,
                    (U16)x_start_s16,
                    (U16)(y_start_s16 + (S16)row_u16),
                    (U16)(x_start_s16 + (S16)draw_width_u16 - 1),
                    (U16)(y_start_s16 + (S16)row_u16));
                ST77916_write_pixels(
                    lcd_pst,
                    &pixels_pu8[byte_offset_u32],
                    (U16)(draw_width_u16 * ST77916_WORD_BYTES_U16));
            }
        }
    }

    return status_e;
}

ST77916_status_e ST77916_draw_char(ST77916_st * lcd_pst,
                                   S16 x_pos_s16,
                                   S16 y_pos_s16,
                                   char character_c,
                                   U16 fg_colour_u16,
                                   U16 bg_colour_u16)
{
    ST77916_status_e status_e;
    U8 glyph_au8[ST77916_CFG_FONT_WIDTH_U8];
    U8 col_u8;
    U8 row_u8;
    U16 colour_u16;
    char glyph_c;

    status_e = ST77916_check_bus(lcd_pst);

    if (status_e == ST77916_STATUS_OK)
    {
        glyph_c = character_c;

        if ((glyph_c >= 'a') && (glyph_c <= 'z'))
        {
            glyph_c = (char)(glyph_c - ('a' - 'A'));
        }

        ST77916_get_font(glyph_c, glyph_au8);

        for (col_u8 = 0u; col_u8 < ST77916_CFG_FONT_WIDTH_U8; col_u8++)
        {
            for (row_u8 = 0u; row_u8 < ST77916_CFG_FONT_HEIGHT_U8; row_u8++)
            {
                colour_u16 = bg_colour_u16;

                if ((glyph_au8[col_u8] & (U8)(1u << row_u8)) != 0u)
                {
                    colour_u16 = fg_colour_u16;
                }

                if (((x_pos_s16 + (S16)col_u8) >= 0) &&
                    ((y_pos_s16 + (S16)row_u8) >= 0))
                {
                    ST77916_draw_pixel(
                        lcd_pst,
                        (U16)(x_pos_s16 + (S16)col_u8),
                        (U16)(y_pos_s16 + (S16)row_u8),
                        colour_u16);
                }
            }
        }
    }

    return status_e;
}

ST77916_status_e ST77916_draw_string(ST77916_st * lcd_pst,
                                     S16 x_pos_s16,
                                     S16 y_pos_s16,
                                     const char * text_pc,
                                     U16 fg_colour_u16,
                                     U16 bg_colour_u16)
{
    ST77916_status_e status_e;
    S16 cursor_x_s16;

    status_e = ST77916_check_bus(lcd_pst);
    cursor_x_s16 = x_pos_s16;

    if (status_e == ST77916_STATUS_OK)
    {
        if (text_pc == NULL)
        {
            status_e = ST77916_STATUS_NULL;
        }
        else
        {
            while (*text_pc != '\0')
            {
                status_e = ST77916_draw_char(
                    lcd_pst,
                    cursor_x_s16,
                    y_pos_s16,
                    *text_pc,
                    fg_colour_u16,
                    bg_colour_u16);
                cursor_x_s16 = (S16)(
                    cursor_x_s16 +
                    ST77916_CFG_FONT_WIDTH_U8 +
                    ST77916_CFG_FONT_SPACING_U8);
                text_pc++;
            }
        }
    }

    return status_e;
}
