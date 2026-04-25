#include "eink.h"

#define EINK_CS_PORT   GPIOA
#define EINK_CS_PIN    GPIO_PIN_8
#define EINK_DC_PORT   GPIOB
#define EINK_DC_PIN    GPIO_PIN_0
#define EINK_RST_PORT  GPIOB
#define EINK_RST_PIN   GPIO_PIN_1
#define EINK_BUSY_PORT GPIOB
#define EINK_BUSY_PIN  GPIO_PIN_2

#define CS_LOW()   HAL_GPIO_WritePin(EINK_CS_PORT, EINK_CS_PIN, GPIO_PIN_RESET)
#define CS_HIGH()  HAL_GPIO_WritePin(EINK_CS_PORT, EINK_CS_PIN, GPIO_PIN_SET)
#define DC_LOW()   HAL_GPIO_WritePin(EINK_DC_PORT, EINK_DC_PIN, GPIO_PIN_RESET)
#define DC_HIGH()  HAL_GPIO_WritePin(EINK_DC_PORT, EINK_DC_PIN, GPIO_PIN_SET)



static void eink_send_cmd(SPI_HandleTypeDef *hspi, uint8_t cmd) {
    DC_LOW();
    CS_LOW();
    HAL_SPI_Transmit(hspi, &cmd, 1, 100);
    CS_HIGH();
}

static void eink_send_data(SPI_HandleTypeDef *hspi, uint8_t data) {
    DC_HIGH();
    CS_LOW();
    HAL_SPI_Transmit(hspi, &data, 1, 100);
    CS_HIGH();
}

HAL_StatusTypeDef eink_init(SPI_HandleTypeDef *hspi) {
    uint8_t cmd = 0x12;

    HAL_GPIO_WritePin(EINK_RST_PORT, EINK_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(10);

    HAL_GPIO_WritePin(EINK_RST_PORT, EINK_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(10);

    while (HAL_GPIO_ReadPin(EINK_BUSY_PORT, EINK_BUSY_PIN) == GPIO_PIN_SET);

    eink_send_cmd(hspi, cmd);

    while (HAL_GPIO_ReadPin(EINK_BUSY_PORT, EINK_BUSY_PIN) == GPIO_PIN_SET);

    return HAL_OK;
}


HAL_StatusTypeDef eink_display(SPI_HandleTypeDef *hspi, log_record_t* logs) {
    uint8_t framebuf[5000] = {0xFF};

    eink_send_cmd(hspi, 0x24);
    
    DC_HIGH();
    CS_LOW();
    HAL_SPI_Transmit(hspi, framebuf, 5000, 5000);
    CS_HIGH();

    eink_send_cmd(hspi, 0x22);
    eink_send_data(hspi, 0xF7);
    eink_send_cmd(hspi, 0x20);

    while (HAL_GPIO_ReadPin(EINK_BUSY_PORT, EINK_BUSY_PIN) == GPIO_PIN_SET);

    return HAL_OK;
}
