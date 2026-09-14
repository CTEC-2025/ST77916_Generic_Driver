# Porting Guide

The ST77916 core driver needs four operations from the target platform:

- send one command byte
- send one or more data bytes
- wait for a number of milliseconds
- optionally drive the reset pin

## Core Bus Interface

```c
typedef struct
{
    ST77916_write_cmd_t write_cmd;
    ST77916_write_data_t write_data;
    ST77916_delay_ms_t delay_ms;
    ST77916_reset_pin_t reset_pin;
} ST77916_bus_st;
```

The command callback should drive the display `DC` pin low, transmit one byte,
then release chip select if your bus design requires it.

The data callback should drive `DC` high and transmit all bytes in the provided
buffer. The driver never takes ownership of the buffer.

## Adapter Pattern

An adapter stores the hardware handles and fills in `ST77916_bus_st` for the
application. The application can then use normal drawing calls without writing
function pointers directly.

```c
typedef struct
{
    ST77916_st lcd_st;
    void * spi_handle_pv;
} my_port_st;
```

The adapter setup function should:

- store the hardware handles and pin numbers
- configure the bus callbacks
- set display width, height, and rotation
- call `ST77916_init`

## Existing Adapters

- `ports/arduino/` hides Arduino `SPI` and pin setup.
- `ports/stm32_hal/` hides `SPI_HandleTypeDef` and GPIO pin handling.

For PIC, AVR, ATtiny, and SAMD, start from the matching file in `examples/`.
Those families vary by part number, so the examples show the exact pieces that
usually need project-specific adjustment.
