# Quick Start

This guide shows the fastest route from an empty firmware project to a simple
screen test.

## 1. Add Driver Files

Add these files to your project:

- `DEFS.h`
- `ST77916.h`
- `ST77916.c`

For Arduino or STM32 HAL projects, you can also add the matching adapter from
`ports/`.

## 2. Choose Your Setup Style

### Generic Callback Setup

Use this style on any MCU when you already have SPI, GPIO, and delay functions.

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
    .rotation_u8 = ST77916_ROTATION_0_U8,
};

ST77916_init(&lcd_st);
ST77916_fill_screen(&lcd_st, ST77916_COLOUR_BLACK_U16);
```

### Arduino Adapter Setup

Add `ports/arduino/ST77916_Arduino.h` and
`ports/arduino/ST77916_Arduino.cpp` to your Arduino sketch folder.

```cpp
#include "ST77916_Arduino.h"

static ST77916_arduino_st lcd_port_st;

void setup(void)
{
    ST77916_arduino_begin(&lcd_port_st, 10u, 9u, 8u, 320u, 385u);
    ST77916_fill_screen(
        ST77916_arduino_lcd(&lcd_port_st),
        ST77916_COLOUR_BLUE_U16);
}
```

### STM32 HAL Adapter Setup

Add `ports/stm32_hal/ST77916_STM32_HAL.h` and
`ports/stm32_hal/ST77916_STM32_HAL.c` to your STM32Cube project.

```c
#include "ST77916_STM32_HAL.h"

static ST77916_stm32_hal_st lcd_port_st;

void app_display_init(void)
{
    ST77916_stm32_begin(
        &lcd_port_st,
        &hspi1,
        GPIOA,
        GPIO_PIN_4,
        GPIOA,
        GPIO_PIN_3,
        GPIOA,
        GPIO_PIN_2,
        320u,
        385u);
    ST77916_fill_screen(
        ST77916_stm32_lcd(&lcd_port_st),
        ST77916_COLOUR_GREEN_U16);
}
```

## 3. Draw Something

```c
ST77916_fill_screen(&lcd_st, ST77916_COLOUR_BLACK_U16);
ST77916_draw_rect(&lcd_st, 20, 20, 100u, 60u, ST77916_COLOUR_WHITE_U16);
ST77916_fill_circle(&lcd_st, 160, 192, 40u, ST77916_COLOUR_RED_U16);
```

## 4. Common Checks

- Confirm `CS`, `DC`, and `RST` pins match your wiring.
- Confirm the display and MCU use compatible logic voltage levels.
- Confirm SPI mode, clock polarity, and clock phase match your hardware.
- If the screen stays blank, try a lower SPI clock first.
