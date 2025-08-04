#ifndef MYLIB_DEVICE_SERVO_H
#define MYLIB_DEVICE_SERVO_H

// MG996R

// 舵机驱动

//    使用方法:
//    0. 使用 CubeMX 配置定时器 设置: 预分频(时钟频率/1000000 -1); 周期(20000 -1);
//    并选择对应通道配置成 PWM输出模式


//    1. 创建 servo_motor_t 句柄并赋 PWM引脚 对应的 定时器 及 通道 初值, 例如
//         servo_motor_t duoji1 = {.htim = &htim2, .channel = TIM_CHANNEL_3};
//         servo_motor_t duoji2 = {.htim = &htim2, .channel = TIM_CHANNEL_4};

//    2. 调用 API 函数实现对应功能, 传入舵机句柄及参数, 例如
//         servo_motor_enable(&duoji1);
//         servo_motor_enable(&duoji2);
//         servo_motor_set_angle(&duoji1, 90);
//         servo_motor_set_angle(&duoji2, 45);


#include "bsp.h"

typedef struct
{
    TIM_HandleTypeDef* htim;
    uint32_t           channel;
    float              angle;
} servo_motor_t;

void servo_motor_enable(servo_motor_t* servo);

void servo_motor_disable(servo_motor_t* servo);

void servo_motor_set_angle(servo_motor_t* servo, float angle);

void servo_motor_set_output(servo_motor_t* servo, u32 compare);

#endif   // !MYLIB_DEVICE_SERVO_H
