#include "i2c.h"
#include "main.h"
#include "stm32f4xx_hal.h"

#define ADS7828_Adress (0x48 << 1) // 7位地址，STM32 HAL要求左移1位
extern I2C_HandleTypeDef hi2c1;

uint8_t data_first[8] = {0};

void ADS7828_I2C_Init(void)
{
    MX_I2C3_Init(); // 初始化I2C
}

uint16_t ADS7828_read_onedata(uint8_t channel)
{
    uint8_t command = 0x48 | (channel << 4); // 单端 + 通道 + 参考开启 通道地址
    uint8_t buffer[2];   //2字节数据

    HAL_I2C_Master_Transmit(&hi2c1, ADS7828_Adress, &command, 1, HAL_MAX_DELAY);
    HAL_Delay(5); // 等待转换完成（建议至少1ms，或根据实际调整）
    HAL_I2C_Master_Receive(&hi2c1, ADS7828_Adress, buffer, 2, HAL_MAX_DELAY);

    uint16_t result = ((buffer[0] & 0x0F) << 8) | buffer[1];
    return result;
}

void ADS7828_read_Alldate(void)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        data_first[i] = ADS7828_read_onedata(i);
    }
		
		//轮询取完
}
