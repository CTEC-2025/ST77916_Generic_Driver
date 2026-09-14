/*****************************************************************************
 * Module: ST77916 STM32 HAL port
 * File: ST77916_STM32_HAL.c
 * Description: STM32 HAL SPI adapter implementation for the ST77916 driver.
 *****************************************************************************/

#include "ST77916_STM32_HAL.h"

#define ST77916_STM32_TIMEOUT_U32      (100u)

static ST77916_stm32_hal_st * active_port_pst = NULL;

static void stm32_write_cmd(U8 command_u8)
{
    if (active_port_pst != NULL)
    {
        HAL_GPIO_WritePin(
            active_port_pst->dc_port_pst,
            active_port_pst->dc_pin_u16,
            GPIO_PIN_RESET);
        HAL_GPIO_WritePin(
            active_port_pst->cs_port_pst,
            active_port_pst->cs_pin_u16,
            GPIO_PIN_RESET);
        (void)HAL_SPI_Transmit(
            active_port_pst->spi_pst,
            &command_u8,
            ST77916_BYTE_BYTES_U16,
            ST77916_STM32_TIMEOUT_U32);
        HAL_GPIO_WritePin(
            active_port_pst->cs_port_pst,
            active_port_pst->cs_pin_u16,
            GPIO_PIN_SET);
    }
}

static void stm32_write_data(const U8 * data_pu8, U16 length_u16)
{
    if ((active_port_pst != NULL) && (data_pu8 != NULL))
    {
        HAL_GPIO_WritePin(
            active_port_pst->dc_port_pst,
            active_port_pst->dc_pin_u16,
            GPIO_PIN_SET);
        HAL_GPIO_WritePin(
            active_port_pst->cs_port_pst,
            active_port_pst->cs_pin_u16,
            GPIO_PIN_RESET);
        (void)HAL_SPI_Transmit(
            active_port_pst->spi_pst,
            (U8 *)data_pu8,
            length_u16,
            ST77916_STM32_TIMEOUT_U32);
        HAL_GPIO_WritePin(
            active_port_pst->cs_port_pst,
            active_port_pst->cs_pin_u16,
            GPIO_PIN_SET);
    }
}

static void stm32_delay_ms(U32 delay_ms_u32)
{
    HAL_Delay(delay_ms_u32);
}

static void stm32_reset(U8 level_u8)
{
    GPIO_PinState state_e;

    state_e = GPIO_PIN_RESET;

    if (active_port_pst != NULL)
    {
        if (level_u8 == ST77916_RESET_HIGH_U8)
        {
            state_e = GPIO_PIN_SET;
        }

        HAL_GPIO_WritePin(
            active_port_pst->reset_port_pst,
            active_port_pst->reset_pin_u16,
            state_e);
    }
}

void ST77916_stm32_begin(ST77916_stm32_hal_st * port_pst,
                         SPI_HandleTypeDef * spi_pst,
                         GPIO_TypeDef * cs_port_pst,
                         U16 cs_pin_u16,
                         GPIO_TypeDef * dc_port_pst,
                         U16 dc_pin_u16,
                         GPIO_TypeDef * reset_port_pst,
                         U16 reset_pin_u16,
                         U16 width_u16,
                         U16 height_u16)
{
    if (port_pst != NULL)
    {
        active_port_pst = port_pst;

        port_pst->spi_pst = spi_pst;
        port_pst->cs_port_pst = cs_port_pst;
        port_pst->cs_pin_u16 = cs_pin_u16;
        port_pst->dc_port_pst = dc_port_pst;
        port_pst->dc_pin_u16 = dc_pin_u16;
        port_pst->reset_port_pst = reset_port_pst;
        port_pst->reset_pin_u16 = reset_pin_u16;

        port_pst->lcd_st.bus_st.write_cmd = stm32_write_cmd;
        port_pst->lcd_st.bus_st.write_data = stm32_write_data;
        port_pst->lcd_st.bus_st.delay_ms = stm32_delay_ms;
        port_pst->lcd_st.bus_st.reset_pin = stm32_reset;
        port_pst->lcd_st.width_u16 = width_u16;
        port_pst->lcd_st.height_u16 = height_u16;
        port_pst->lcd_st.rotation_u8 = ST77916_ROTATION_0_U8;

        ST77916_init(&port_pst->lcd_st);
    }
}

ST77916_st * ST77916_stm32_lcd(ST77916_stm32_hal_st * port_pst)
{
    ST77916_st * lcd_pst;

    lcd_pst = NULL;

    if (port_pst != NULL)
    {
        lcd_pst = &port_pst->lcd_st;
    }

    return lcd_pst;
}
