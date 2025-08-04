#ifndef BSP_ULTRASONIC_SENSOR_H
#define BSP_ULTRASONIC_SENSOR_H

#include "bsp_motor.h"
#include "stm32f10x.h"
// 超声波传感器驱动
// 距离传感器
/*
    Trig(发射引脚)
    Echo(接收引脚)
*/


#define BSP_ULTRASONIC_TIM_RCC RCC_APB1Periph_TIM3
#define BSP_ULTRASONIC_TIM TIM3

#define Trig_RCC RCC_APB2Periph_GPIOA
#define Trig_Pin GPIO_Pin_1
#define Trig_Port GPIOA

#define Echo_RCC RCC_APB2Periph_GPIOA
#define Echo_Pin GPIO_Pin_2
#define Echo_Port GPIOA

#define Trig PAin(1)
#define Echo PAin(2)




void  ULTRASONIC_Init(void);
float Get_distance(void);

#endif   // !BSP_ULTRASONIC_SENSOR_H
