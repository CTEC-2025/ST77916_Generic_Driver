# Port Adapters

The core driver is still platform-neutral, but the files in this folder provide
friendlier setup layers for common environments.

## Available Adapters

- `arduino/` - Arduino SPI adapter.
- `stm32_hal/` - STM32 HAL SPI adapter.

The PIC, AVR, ATtiny, and SAMD folders in `examples/` show direct adapter code
for those families. They are kept as examples because pin and peripheral setup
varies heavily between parts.

## Why Adapters Exist

The core driver uses callbacks so it can run on almost any MCU. Adapters hide
those callbacks behind setup functions, so application code can stay smaller
and easier to read.
