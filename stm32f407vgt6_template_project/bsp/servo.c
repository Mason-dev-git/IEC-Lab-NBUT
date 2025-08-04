#include "servo.h"

void servo_motor_enable(servo_motor_t* servo)
{
    HAL_TIM_PWM_Start(servo->htim, servo->channel);
}

void servo_motor_disable(servo_motor_t* servo)
{
    HAL_TIM_PWM_Stop(servo->htim, servo->channel);
}

void servo_motor_set_angle(servo_motor_t* servo, float angle)
{
    if (angle < 0) { angle = 0; }
    if (angle > 180) { angle = 180; }

    switch (servo->channel) {
    case TIM_CHANNEL_1: servo->htim->Instance->CCR1 = 500 + (angle * (2000.0f / 180.0f)); break;
    case TIM_CHANNEL_2: servo->htim->Instance->CCR2 = 500 + (angle * (2000.0f / 180.0f)); break;
    case TIM_CHANNEL_3: servo->htim->Instance->CCR3 = 500 + (angle * (2000.0f / 180.0f)); break;
    case TIM_CHANNEL_4: servo->htim->Instance->CCR4 = 500 + (angle * (2000.0f / 180.0f)); break;
    default: break;
    }

    servo->angle = angle;
}

void servo_motor_set_output(servo_motor_t* servo, u32 compare)
{
    switch (servo->channel) {
    case TIM_CHANNEL_1: servo->htim->Instance->CCR1 = compare; break;
    case TIM_CHANNEL_2: servo->htim->Instance->CCR2 = compare; break;
    case TIM_CHANNEL_3: servo->htim->Instance->CCR3 = compare; break;
    case TIM_CHANNEL_4: servo->htim->Instance->CCR4 = compare; break;
    default: break;
    }
}