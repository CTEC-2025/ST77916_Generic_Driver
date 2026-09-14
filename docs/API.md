# API Reference

This reference covers the public ST77916 driver functions. All drawing helpers
use RGB565 colour values.

## Colours

The header provides common RGB565 colour constants:

- `ST77916_COLOUR_BLACK_U16`
- `ST77916_COLOUR_WHITE_U16`
- `ST77916_COLOUR_RED_U16`
- `ST77916_COLOUR_GREEN_U16`
- `ST77916_COLOUR_BLUE_U16`
- `ST77916_COLOUR_YELLOW_U16`
- `ST77916_COLOUR_CYAN_U16`
- `ST77916_COLOUR_MAGENTA_U16`

### `ST77916_rgb565`

Converts 8-bit red, green, and blue values into one RGB565 colour value.

```c
U16 ST77916_rgb565(U8 red_u8, U8 green_u8, U8 blue_u8);
```

## Setup

### `ST77916_init`

Initializes the display using the configured bus callbacks. The function resets
the panel, applies the configured rotation, exits sleep mode, and turns the
display on.

```c
void ST77916_init(ST77916_st * lcd_pst);
```

### `ST77916_reset`

Resets the display. If `reset_pin` is provided, the hardware reset sequence is
used. Otherwise, the software reset command is sent.

```c
void ST77916_reset(ST77916_st * lcd_pst);
```

### `ST77916_set_rotation`

Sets the panel memory-access rotation. Valid values are
`ST77916_ROTATION_0_U8`, `ST77916_ROTATION_90_U8`,
`ST77916_ROTATION_180_U8`, and `ST77916_ROTATION_270_U8`.

```c
void ST77916_set_rotation(ST77916_st * lcd_pst, U8 rotation_u8);
```

## Low-Level Drawing

### `ST77916_set_window`

Selects the rectangular display memory area used by the next pixel write.

```c
void ST77916_set_window(ST77916_st * lcd_pst,
                        U16 x_start_u16,
                        U16 y_start_u16,
                        U16 x_end_u16,
                        U16 y_end_u16);
```

### `ST77916_write_pixels`

Writes raw RGB565 pixel bytes into the currently selected memory area.

```c
void ST77916_write_pixels(ST77916_st * lcd_pst,
                          const U8 * pixels_pu8,
                          U16 length_u16);
```

### `ST77916_fill_colour`

Writes one RGB565 colour repeatedly into the currently selected memory area.

```c
void ST77916_fill_colour(ST77916_st * lcd_pst,
                         U16 colour_u16,
                         U32 pixel_count_u32);
```

## Drawing Helpers

### `ST77916_fill_screen`

Fills the whole configured display area.

```c
void ST77916_fill_screen(ST77916_st * lcd_pst, U16 colour_u16);
```

### `ST77916_draw_pixel`

Draws one pixel. Coordinates outside the configured display are ignored.

```c
void ST77916_draw_pixel(ST77916_st * lcd_pst,
                        U16 x_pos_u16,
                        U16 y_pos_u16,
                        U16 colour_u16);
```

### `ST77916_fill_rect`

Fills a rectangle. The rectangle is clipped to the configured display size.

```c
void ST77916_fill_rect(ST77916_st * lcd_pst,
                       U16 x_pos_u16,
                       U16 y_pos_u16,
                       U16 width_u16,
                       U16 height_u16,
                       U16 colour_u16);
```

### `ST77916_draw_hline`

Draws a horizontal line. The line is clipped to the configured display size.

```c
void ST77916_draw_hline(ST77916_st * lcd_pst,
                        S16 x_pos_s16,
                        S16 y_pos_s16,
                        U16 length_u16,
                        U16 colour_u16);
```

### `ST77916_draw_vline`

Draws a vertical line. The line is clipped to the configured display size.

```c
void ST77916_draw_vline(ST77916_st * lcd_pst,
                        S16 x_pos_s16,
                        S16 y_pos_s16,
                        U16 length_u16,
                        U16 colour_u16);
```

### `ST77916_draw_line`

Draws a line between two points. Pixels outside the display are ignored.

```c
void ST77916_draw_line(ST77916_st * lcd_pst,
                       S16 x_start_s16,
                       S16 y_start_s16,
                       S16 x_end_s16,
                       S16 y_end_s16,
                       U16 colour_u16);
```

### `ST77916_draw_rect`

Draws a rectangle outline using horizontal and vertical lines.

```c
void ST77916_draw_rect(ST77916_st * lcd_pst,
                       S16 x_pos_s16,
                       S16 y_pos_s16,
                       U16 width_u16,
                       U16 height_u16,
                       U16 colour_u16);
```

### `ST77916_draw_circle`

Draws a circle outline. Pixels outside the display are ignored.

```c
void ST77916_draw_circle(ST77916_st * lcd_pst,
                         S16 x_pos_s16,
                         S16 y_pos_s16,
                         U16 radius_u16,
                         U16 colour_u16);
```

### `ST77916_fill_circle`

Draws a filled circle using clipped vertical spans.

```c
void ST77916_fill_circle(ST77916_st * lcd_pst,
                         S16 x_pos_s16,
                         S16 y_pos_s16,
                         U16 radius_u16,
                         U16 colour_u16);
```
