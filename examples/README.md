# Examples

These examples show how to connect the generic ST77916 driver to common MCU
families. They are intentionally small adapter skeletons: update the SPI,
GPIO, clock, and pin definitions for your board before building.

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
