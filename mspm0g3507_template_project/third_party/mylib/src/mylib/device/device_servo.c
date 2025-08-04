#include "device_servo.h"

#ifdef __STM32F1xx_HAL_H   // HALåº“

void servo_motor_enable(servo_motor_t* motor)
{
    HAL_TIM_PWM_Start(motor->htim, motor->TIM_CHANNEL_x);
}

void servo_motor_disable(servo_motor_t* motor)
{
    HAL_TIM_PWM_Stop(motor->htim, motor->TIM_CHANNEL_x);
}

void servo_motor_set_angle(servo_motor_t* motor, float angle)
{
    if (angle < 0) { angle = 0; }
    if (angle > 180) { angle = 180; }

    switch (motor->TIM_CHANNEL_x) {
    case TIM_CHANNEL_1: hduoji->htim->Instance->CCR1 = 500 + (angle * (2000.0f / 180.0f)); break;
    case TIM_CHANNEL_2: hduoji->htim->Instance->CCR2 = 500 + (angle * (2000.0f / 180.0f)); break;
    case TIM_CHANNEL_3: hduoji->htim->Instance->CCR3 = 500 + (angle * (2000.0f / 180.0f)); break;
    case TIM_CHANNEL_4: hduoji->htim->Instance->CCR4 = 500 + (angle * (2000.0f / 180.0f)); break;

    default: break;
    }

    motor->angle = angle;
}

#endif   // __STM32F1xx_HAL_H

#ifdef __STM32F10x_H

void servo_motor_enable(servo_motor_t* motor) {}

void servo_motor_disable(servo_motor_t* motor) {}

void servo_motor_set_angle(servo_motor_t* motor, float angle)
{
    if (angle < 0) { angle = 0; }
    if (angle > 180) { angle = 180; }



    motor->angle = angle;
}

#endif   // __STM32F1xx_HAL_H