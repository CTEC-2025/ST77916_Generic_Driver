#include "stm32xxxx_hal.h"
#include "../../ST77916.h"

#define ST77916_WIDTH_U16              (320u)
#define ST77916_HEIGHT_U16             (385u)
#define ST77916_PIXEL_COUNT_U32        (123200u)
#define ST77916_HAL_TIMEOUT_U32        (100u)

#define ST77916_CS_PORT                GPIOA
#define ST77916_CS_PIN                 GPIO_PIN_4
#define ST77916_DC_PORT                GPIOA
#define ST77916_DC_PIN                 GPIO_PIN_3
#define ST77916_RST_PORT               GPIOA
#define ST77916_RST_PIN                GPIO_PIN_2

extern SPI_HandleTypeDef hspi1;

static ST77916_st lcd_st;

static void stm32_write_cmd(U8 command_u8)
{
    HAL_GPIO_WritePin(ST77916_DC_PORT, ST77916_DC_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ST77916_CS_PORT, ST77916_CS_PIN, GPIO_PIN_RESET);
    (void)HAL_SPI_Transmit(
        &hspi1,
        &command_u8,
        ST77916_BYTE_BYTES_U16,
        ST77916_HAL_TIMEOUT_U32);
    HAL_GPIO_WritePin(ST77916_CS_PORT, ST77916_CS_PIN, GPIO_PIN_SET);
}

static void stm32_write_data(const U8 * data_pu8, U16 length_u16)
{
    if (data_pu8 != NULL)
    {
        HAL_GPIO_WritePin(ST77916_DC_PORT, ST77916_DC_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ST77916_CS_PORT, ST77916_CS_PIN, GPIO_PIN_RESET);
        (void)HAL_SPI_Transmit(
            &hspi1,
            (U8 *)data_pu8,
            length_u16,
            ST77916_HAL_TIMEOUT_U32);
        HAL_GPIO_WritePin(ST77916_CS_PORT, ST77916_CS_PIN, GPIO_PIN_SET);
    }
}

static void stm32_delay_ms(U32 delay_ms_u32)
{
    HAL_Delay(delay_ms_u32);
}

static void stm32_reset(U8 level_u8)
{
    if (level_u8 == ST77916_RESET_HIGH_U8)
    {
        HAL_GPIO_WritePin(ST77916_RST_PORT, ST77916_RST_PIN, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(ST77916_RST_PORT, ST77916_RST_PIN, GPIO_PIN_RESET);
    }
}

void app_display_init(void)
{
    lcd_st.bus_st.write_cmd = stm32_write_cmd;
    lcd_st.bus_st.write_data = stm32_write_data;
    lcd_st.bus_st.delay_ms = stm32_delay_ms;
    lcd_st.bus_st.reset_pin = stm32_reset;
    lcd_st.width_u16 = ST77916_WIDTH_U16;
    lcd_st.height_u16 = ST77916_HEIGHT_U16;
    lcd_st.rotation_u8 = ST77916_ROTATION_0_U8;

    ST77916_init(&lcd_st);
    ST77916_fill_screen(&lcd_st, ST77916_COLOUR_BLACK_U16);
    ST77916_fill_rect(&lcd_st, 20u, 20u, 80u, 40u,
                      ST77916_COLOUR_RED_U16);
    ST77916_draw_rect(&lcd_st, 18, 18, 84u, 44u,
                      ST77916_COLOUR_WHITE_U16);
    ST77916_draw_line(&lcd_st, 0, 384, 319, 0,
                      ST77916_COLOUR_GREEN_U16);
    ST77916_draw_circle(&lcd_st, 160, 192, 48u,
                        ST77916_COLOUR_BLUE_U16);
    ST77916_fill_circle(&lcd_st, 160, 192, 24u,
                        ST77916_COLOUR_YELLOW_U16);
}
