#ifndef DEVICE_SERVO_H
#define DEVICE_SERVO_H

// MG996R

// 舵机驱动

//    使用方法:
//    0. 使用 CubeMX 配置定时器 设置: 预分频(时钟频率/1000000 -1); 周期(20000 -1);
//    并选择对应通道配置成 PWM输出模式


//    1. 创建 servo_duoji_HandleTypeDef 句柄并赋 PWM引脚 对应的 定时器 及 通道 初值, 例如
//         servo_duoji_HandleTypeDef duoji1 = {.htim = &htim2, .TIM_CHANNEL_x = TIM_CHANNEL_3};
//         servo_duoji_HandleTypeDef duoji2 = {.htim = &htim2, .TIM_CHANNEL_x = TIM_CHANNEL_4};

//    2. 调用 API 函数实现对应功能, 传入舵机句柄及参数, 例如
//         servo_motor_enable(&duoji1);
//         servo_motor_enable(&duoji2);
//         servo_motor_set_angle(&duoji1, 90);
//         servo_motor_set_angle(&duoji2, 45);

#include <stdint.h>
#include "stm32f10x.h"

#ifdef __STM32F1xx_HAL_H   // HAL库

typedef struct
{
    TIM_HandleTypeDef* htim;
    uint32_t           TIM_CHANNEL_x;
    float              angle;
} servo_motor_t;

#endif   // __STM32F1xx_HAL_H

#ifdef __STM32F10x_H

typedef struct
{
    GPIO_TypeDef* dir_gpio;
    uint32_t      dir_pin;   //转向引脚
    uint32_t      dir_rcc;
    GPIO_TypeDef* pul_gpio;
    uint32_t      pul_pin;   // pwm脉冲引脚
    uint32_t      pul_rcc;
    TIM_TypeDef*  tim;
    uint32_t      tim_rcc;
    IRQn_Type     tim_iqr;
    uint8_t       tim_channel;
    uint16_t      tim_ocmode;
    uint16_t      tim_compare;   // 占空比
    uint32_t      TIM_CHANNEL_x;
    float         angle;
} servo_motor_t;


#endif   // __STM32F1xx_HAL_H

void servo_motor_enable(servo_motor_t* motor);

void servo_motor_disable(servo_motor_t* motor);

void servo_motor_set_angle(servo_motor_t* motor, float angle);

#endif   // !DEVICE_SERVO_H
