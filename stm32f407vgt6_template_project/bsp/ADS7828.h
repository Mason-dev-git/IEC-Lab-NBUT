#ifndef __ADS7828_H
#define __ADS7828_H


#include "stm32f4xx_hal.h"
#include "i2c.h"

extern I2C_HandleTypeDef hi2c2;
extern uint8_t  data_first[8];

void ADS7828_I2C_Init(void);
uint8_t ADS7828_read_onedata(uint8_t channel);




#endif
