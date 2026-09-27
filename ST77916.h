/*****************************************************************************
 * Module: ST77916
 * File: ST77916.h
 * Description: Public interface for the ST77916 display driver.
 * Notes: Written for embedded C use with MISRA C:2025 review in mind.
 *****************************************************************************/

#ifndef ST77916_H
#define ST77916_H

#include "DEFS.h"
#include "ST77916_CFG.h"

#define ST77916_VERSION_MAJOR_U8       (0u)
#define ST77916_VERSION_MINOR_U8       (5u)
#define ST77916_VERSION_PATCH_U8       (0u)

#define ST77916_SWRESET_U8             (0x01u)
#define ST77916_SLPOUT_U8              (0x11u)
#define ST77916_DISPON_U8              (0x29u)
#define ST77916_CASET_U8               (0x2Au)
#define ST77916_RASET_U8               (0x2Bu)
#define ST77916_RAMWR_U8               (0x2Cu)
#define ST77916_MADCTL_U8              (0x36u)

#define ST77916_RESET_LOW_U8           (0u)
#define ST77916_RESET_HIGH_U8          (1u)

#define ST77916_ROTATION_0_U8          (0u)
#define ST77916_ROTATION_90_U8         (1u)
#define ST77916_ROTATION_180_U8        (2u)
#define ST77916_ROTATION_270_U8        (3u)
#define ST77916_ROTATION_COUNT_U8      (4u)

#define ST77916_MADCTL_0_U8            (0x00u)
#define ST77916_MADCTL_90_U8           (0x60u)
#define ST77916_MADCTL_180_U8          (0xC0u)
#define ST77916_MADCTL_270_U8          (0xA0u)

#define ST77916_DELAY_PULSE_MS_U32     (5u)
#define ST77916_DELAY_RESET_MS_U32     (20u)
#define ST77916_DELAY_READY_MS_U32     (120u)
#define ST77916_DELAY_DISPLAY_MS_U32   (20u)

#define ST77916_BYTE_BYTES_U16         (1u)
#define ST77916_WORD_BYTES_U16         (2u)
#define ST77916_ADDR_BYTES_U16         (4u)
#define ST77916_FILL_PIXELS_U16        ST77916_CFG_FILL_PIXELS_U16
#define ST77916_FILL_BYTES_U16         (ST77916_FILL_PIXELS_U16 * 2u)
#define ST77916_LOW_BYTE_MASK_U16      (0x00FFu)

#define ST77916_RED_MASK_U8            (0xF8u)
#define ST77916_GREEN_MASK_U8          (0xFCu)
#define ST77916_BLUE_MASK_U8           (0xF8u)
#define ST77916_RED_SHIFT_U8           (8u)
#define ST77916_GREEN_SHIFT_U8         (3u)
#define ST77916_BLUE_SHIFT_U8          (3u)

#define ST77916_COLOUR_BLACK_U16       (0x0000u)
#define ST77916_COLOUR_WHITE_U16       (0xFFFFu)
#define ST77916_COLOUR_RED_U16         (0xF800u)
#define ST77916_COLOUR_GREEN_U16       (0x07E0u)
#define ST77916_COLOUR_BLUE_U16        (0x001Fu)
#define ST77916_COLOUR_YELLOW_U16      (0xFFE0u)
#define ST77916_COLOUR_CYAN_U16        (0x07FFu)
#define ST77916_COLOUR_MAGENTA_U16     (0xF81Fu)

typedef void (*ST77916_write_cmd_t)(U8 command_u8);
typedef void (*ST77916_write_data_t)(const U8 * data_pu8, U16 length_u16);
typedef void (*ST77916_delay_ms_t)(U32 delay_ms_u32);
typedef void (*ST77916_reset_pin_t)(U8 level_u8);

typedef enum
{
    ST77916_STATUS_OK = 0,
    ST77916_STATUS_NULL,
    ST77916_STATUS_BUS,
    ST77916_STATUS_BOUNDS,
    ST77916_STATUS_PARAM
} ST77916_status_e;

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
U16 ST77916_rgb565(U8 red_u8, U8 green_u8, U8 blue_u8);
ST77916_status_e ST77916_init_ex(ST77916_st * lcd_pst);
ST77916_status_e ST77916_reset_ex(ST77916_st * lcd_pst);
ST77916_status_e ST77916_set_rotation_ex(ST77916_st * lcd_pst,
                                          U8 rotation_u8);
ST77916_status_e ST77916_set_window_ex(ST77916_st * lcd_pst,
                                       U16 x_start_u16,
                                       U16 y_start_u16,
                                       U16 x_end_u16,
                                       U16 y_end_u16);
ST77916_status_e ST77916_write_pixels_ex(ST77916_st * lcd_pst,
                                         const U8 * pixels_pu8,
                                         U16 length_u16);
ST77916_status_e ST77916_fill_colour_ex(ST77916_st * lcd_pst,
                                        U16 colour_u16,
                                        U32 pixel_count_u32);
ST77916_status_e ST77916_fill_screen_ex(ST77916_st * lcd_pst,
                                        U16 colour_u16);
ST77916_status_e ST77916_draw_pixel_ex(ST77916_st * lcd_pst,
                                       U16 x_pos_u16,
                                       U16 y_pos_u16,
                                       U16 colour_u16);
ST77916_status_e ST77916_fill_rect_ex(ST77916_st * lcd_pst,
                                      U16 x_pos_u16,
                                      U16 y_pos_u16,
                                      U16 width_u16,
                                      U16 height_u16,
                                      U16 colour_u16);
ST77916_status_e ST77916_draw_bitmap(ST77916_st * lcd_pst,
                                     S16 x_pos_s16,
                                     S16 y_pos_s16,
                                     U16 width_u16,
                                     U16 height_u16,
                                     const U8 * pixels_pu8);
ST77916_status_e ST77916_draw_char(ST77916_st * lcd_pst,
                                   S16 x_pos_s16,
                                   S16 y_pos_s16,
                                   char character_c,
                                   U16 fg_colour_u16,
                                   U16 bg_colour_u16);
ST77916_status_e ST77916_draw_string(ST77916_st * lcd_pst,
                                     S16 x_pos_s16,
                                     S16 y_pos_s16,
                                     const char * text_pc,
                                     U16 fg_colour_u16,
                                     U16 bg_colour_u16);
void ST77916_set_rotation(ST77916_st * lcd_pst, U8 rotation_u8);
void ST77916_set_window(ST77916_st * lcd_pst,
                        U16 x_start_u16,
                        U16 y_start_u16,
                        U16 x_end_u16,
                        U16 y_end_u16);
void ST77916_write_pixels(ST77916_st * lcd_pst,
                          const U8 * pixels_pu8,
                          U16 length_u16);
void ST77916_fill_colour(ST77916_st * lcd_pst,
                         U16 colour_u16,
                         U32 pixel_count_u32);
void ST77916_fill_screen(ST77916_st * lcd_pst, U16 colour_u16);
void ST77916_draw_pixel(ST77916_st * lcd_pst,
                        U16 x_pos_u16,
                        U16 y_pos_u16,
                        U16 colour_u16);
void ST77916_fill_rect(ST77916_st * lcd_pst,
                       U16 x_pos_u16,
                       U16 y_pos_u16,
                       U16 width_u16,
                       U16 height_u16,
                       U16 colour_u16);
void ST77916_draw_hline(ST77916_st * lcd_pst,
                        S16 x_pos_s16,
                        S16 y_pos_s16,
                        U16 length_u16,
                        U16 colour_u16);
void ST77916_draw_vline(ST77916_st * lcd_pst,
                        S16 x_pos_s16,
                        S16 y_pos_s16,
                        U16 length_u16,
                        U16 colour_u16);
void ST77916_draw_line(ST77916_st * lcd_pst,
                       S16 x_start_s16,
                       S16 y_start_s16,
                       S16 x_end_s16,
                       S16 y_end_s16,
                       U16 colour_u16);
void ST77916_draw_rect(ST77916_st * lcd_pst,
                       S16 x_pos_s16,
                       S16 y_pos_s16,
                       U16 width_u16,
                       U16 height_u16,
                       U16 colour_u16);
void ST77916_draw_circle(ST77916_st * lcd_pst,
                         S16 x_pos_s16,
                         S16 y_pos_s16,
                         U16 radius_u16,
                         U16 colour_u16);
void ST77916_fill_circle(ST77916_st * lcd_pst,
                         S16 x_pos_s16,
                         S16 y_pos_s16,
                         U16 radius_u16,
                         U16 colour_u16);

#endif
