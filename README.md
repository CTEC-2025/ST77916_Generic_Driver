# ST77916 Generic Driver

A small, platform-neutral C driver for ST77916 display controllers.

The driver is built around callback functions supplied by the host application, so it can be adapted to SPI, parallel, bit-banged, RTOS, or bare-metal targets without pulling hardware-specific code into the driver.

## Repository Contents

- `DEFS.h` - common fixed-width type aliases and boolean definitions.
- `ST77916.h` - public driver types, command values, and declarations.
- `ST77916.c` - reset, initialization, window, rotation, and pixel writes.
- `.gitattributes` - line-ending normalization for Git.
- `.editorconfig` - shared editor formatting defaults.
- `.gitignore` - common generated files ignored for C and embedded projects.

## Integration

Add `ST77916.c`, `ST77916.h`, and `DEFS.h` to your firmware project, then provide the bus callbacks used by `ST77916_bus_st`:

```c
ST77916_st lcd_st = {
    .bus_st = {
        .write_cmd = platform_write_cmd,
        .write_data = platform_write_data,
        .delay_ms = platform_delay_ms,
        .reset_pin = platform_reset_pin,
    },
    .width_u16 = 320u,
    .height_u16 = 385u,
    .rotation_u8 = 0u,
};

ST77916_init(&lcd_st);
```

`reset_pin` may be set to `NULL` if the target does not expose a hardware reset pin. In that case the driver sends a software reset command instead.

## Drawing

Select a drawing area, then write pixels as RGB565 bytes:

```c
ST77916_set_window(&lcd_st, 0u, 0u, 319u, 384u);
ST77916_write_pixels(&lcd_st, image_pu8, image_length_u16);
```

For simple test output, fill the current memory-write area with one colour:

```c
ST77916_set_window(&lcd_st, 0u, 0u, 319u, 384u);
ST77916_fill_colour(&lcd_st, 0xF800u, 123200u);
```

Rotation can be set with `ST77916_set_rotation`. Valid rotation values are
`ST77916_ROTATION_0_U8`, `ST77916_ROTATION_90_U8`,
`ST77916_ROTATION_180_U8`, and `ST77916_ROTATION_270_U8`.

## Notes

This repository contains a minimal initialization flow and basic RGB565 drawing
helpers. Panel-specific command tables can be added as the target hardware
requirements are confirmed.

The source is arranged to support MISRA C:2025-oriented review: numeric command values are named, callbacks are checked before use, and source lines are kept short.

## License

This project is licensed under the MIT License. See `LICENSE` for details.
