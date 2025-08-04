#ifndef BSP_INFRARED_SENSOR_H
#define BSP_INFRARED_SENSOR_H

#include "stm32f10x.h"

#define BSP_INFRARED_RCC RCC_APB2Periph_GPIOA

#define BSP_OLED_GPIO GPIOB
#define BSP_OLED_PIN_SCL GPIO_Pin_6
#define BSP_OLED_PIN_SDA GPIO_Pin_7
// 红外传感器驱动


void INFRARED_Init(void);
void read_sensor(void);
#endif   // !BSP_INFRARED_SENSOR_H
