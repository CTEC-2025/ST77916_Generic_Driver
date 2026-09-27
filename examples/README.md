# Examples

These examples show how to connect the generic ST77916 driver to common MCU
families. Each example initializes the bus callbacks, checks the v0.5 status
API, fills the display, draws a small RGB565 bitmap, and writes a text label.
They remain board templates: configure the listed clock, SPI, GPIO, and pin
mux settings for the exact target before building or attaching a display.

## Included Targets

- `arduino/ST77916_Arduino.ino` - Arduino SPI sketch.
- `pic/pic_xc8_spi.c` - PIC XC8 MSSP-style adapter.
- `avr/atmega_spi.c` - ATmega hardware SPI adapter.
- `attiny/attiny_bitbang_spi.c` - ATtiny bit-banged SPI adapter.
- `samd/samd21_sercom_spi.c` - SAMD21 SERCOM-style adapter.
- `stm32/stm32_hal_spi.c` - STM32 HAL SPI adapter.

## Notes

Each example includes the driver through a relative path:

```c
#include "../../ST77916.h"
```

When copying an example into your own project, either keep the same relative
layout or replace the include path with the location used by your build system.

### Target Setup

- **Arduino:** Install the board package and provide the pins defined in the
  sketch. Add the driver C source and headers to the sketch build.
- **PIC:** Select the exact device in MPLAB X, configure MSSP/SPI and the
  `LATC` pins, and set `_XTAL_FREQ` to the configured oscillator frequency.
- **AVR:** Select the exact ATmega and set `F_CPU` to the configured clock.
  Confirm the `PORTB` pins match the package and SPI pin mapping.
- **ATtiny:** Select a device that provides the `PORTB` pins used here and set
  `F_CPU`. Confirm the bit-banged clock polarity and pin availability.
- **SAMD21:** Enable and clock SERCOM0 in SPI master mode, configure its
  peripheral pin mux, configure SysTick at 48 MHz, and set the GPIO pins.
- **STM32:** Replace `stm32xxxx_hal.h` with the device HAL header, configure
  `hspi1` and GPIO through CubeMX or equivalent initialization, and confirm
  the SPI mode and pin assignments.

All examples assume a 320 by 385 panel and use blocking transfers. The
controller module's initialization sequence may need adjustment for a
particular display board. Verify voltage levels and panel pin labels before
connecting hardware.
