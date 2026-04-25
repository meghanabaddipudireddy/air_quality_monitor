#include "w25q32.h"
#include "stm32l4xx_hal_spi.h"

#define FLASH_CS_PORT GPIOA
#define FLASH_CS_PIN  GPIO_PIN_4
#define CS_LOW()  HAL_GPIO_WritePin(FLASH_CS_PORT, FLASH_CS_PIN, GPIO_PIN_RESET)
#define CS_HIGH() HAL_GPIO_WritePin(FLASH_CS_PORT, FLASH_CS_PIN, GPIO_PIN_SET)

static uint32_t write_index = 0;

HAL_StatusTypeDef w25q32_init(SPI_HandleTypeDef *hspi) {
   uint8_t cmd = 0x9F;
   uint8_t id[3];
   CS_LOW();
   HAL_SPI_Transmit(hspi, &cmd, 1, 100);
   HAL_SPI_Receive(hspi, id, 3, 100);
   CS_HIGH();
   if (id[0] == 0xEF && id[1] == 0x40 && id[2] == 0x16) return HAL_OK;
   return HAL_ERROR;
}

HAL_StatusTypeDef w25q32_write_record(SPI_HandleTypeDef *hspi, log_record_t* logs) {
    uint8_t cmd = 0x06;
    uint32_t addr = write_index * 32;
    uint8_t page_cmd[4] = {0x02, addr >> 16, addr >> 8, addr};
    uint8_t status_cmd = 0x05;
    uint8_t status;

    CS_LOW();
    HAL_SPI_Transmit(hspi, &cmd, 1, 100);
    CS_HIGH();

    CS_LOW();
    HAL_SPI_Transmit(hspi, page_cmd, 4, 100);
    HAL_SPI_Transmit(hspi, (uint8_t*)logs, 32, 100);
    CS_HIGH();

    do {
        CS_LOW();
        HAL_SPI_Transmit(hspi, &status_cmd, 1, 100);
        HAL_SPI_Receive(hspi, &status, 1, 100);
        CS_HIGH();
    } while (status & 0x01);

    write_index++;
    return HAL_OK;
}

HAL_StatusTypeDef w25q32_read_record(SPI_HandleTypeDef *hspi,uint32_t record, log_record_t* logs) {
    uint32_t addr = record * 32;
    uint8_t read_cmd[4] = {0x03, addr >> 16, addr >> 8, addr};
    CS_LOW();
    HAL_SPI_Transmit(hspi, read_cmd, 4, 100);
    HAL_SPI_Receive(hspi, (uint8_t*)logs, 32, 100);
    CS_HIGH();
    return HAL_OK;
}
