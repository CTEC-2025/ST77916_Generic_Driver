/*****************************************************************************
 * Module: ST77916 STM32 HAL port
 * File: ST77916_STM32_HAL.h
 * Description: STM32 HAL SPI adapter interface for the ST77916 driver.
 *****************************************************************************/

#ifndef ST77916_STM32_HAL_H
#define ST77916_STM32_HAL_H

#include "../../ST77916.h"
#include "stm32xxxx_hal.h"

typedef struct
{
    ST77916_st lcd_st;
    SPI_HandleTypeDef * spi_pst;
    GPIO_TypeDef * cs_port_pst;
    U16 cs_pin_u16;
    GPIO_TypeDef * dc_port_pst;
    U16 dc_pin_u16;
    GPIO_TypeDef * reset_port_pst;
    U16 reset_pin_u16;
} ST77916_stm32_hal_st;

void ST77916_stm32_begin(ST77916_stm32_hal_st * port_pst,
                         SPI_HandleTypeDef * spi_pst,
                         GPIO_TypeDef * cs_port_pst,
                         U16 cs_pin_u16,
                         GPIO_TypeDef * dc_port_pst,
                         U16 dc_pin_u16,
                         GPIO_TypeDef * reset_port_pst,
                         U16 reset_pin_u16,
                         U16 width_u16,
                         U16 height_u16);
ST77916_st * ST77916_stm32_lcd(ST77916_stm32_hal_st * port_pst);

#endif
