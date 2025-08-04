#include "motor.h"
#include "bsp.h"

// 初始化电机，打开pwm
void motor_init(void)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);
}

int32_t my_abs(int32_t x)
{
    return (x < 0) ? -x : x;
}

// 让电机停止运行
void motor_stop(void)
{
    HAL_GPIO_WritePin(MB_IN1_GPIO_Port, MB_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MB_IN2_GPIO_Port, MB_IN2_Pin, GPIO_PIN_RESET);
    motor_set_speed(0, 0);
}

// 正代表正转
// 负代表反转
void motor_set_speed(int32_t speed_left, int32_t speed_right)
{
    if (speed_left > 0) {
        HAL_GPIO_WritePin(MA_IN1_GPIO_Port, MA_IN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MA_IN2_GPIO_Port, MA_IN2_Pin, GPIO_PIN_RESET);
    }
    else if (speed_left < 0) {
        HAL_GPIO_WritePin(MA_IN1_GPIO_Port, MA_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MA_IN2_GPIO_Port, MA_IN2_Pin, GPIO_PIN_SET);
    }
    else {
        HAL_GPIO_WritePin(MA_IN1_GPIO_Port, MA_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MA_IN2_GPIO_Port, MA_IN2_Pin, GPIO_PIN_RESET);
    }

    if (speed_right > 0) {
        HAL_GPIO_WritePin(MB_IN1_GPIO_Port, MB_IN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MB_IN2_GPIO_Port, MB_IN2_Pin, GPIO_PIN_RESET);
    }
    else if (speed_right < 0) {
        HAL_GPIO_WritePin(MB_IN1_GPIO_Port, MB_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MB_IN2_GPIO_Port, MB_IN2_Pin, GPIO_PIN_SET);
    }
    else {
        HAL_GPIO_WritePin(MB_IN1_GPIO_Port, MB_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MB_IN2_GPIO_Port, MB_IN2_Pin, GPIO_PIN_RESET);
    }

    __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, my_abs(speed_left));
    __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, my_abs(speed_right));
}

// 获取编码的数据
int32_t motor_left_encoder(void)
{
    return __HAL_TIM_GET_COUNTER(&htim2);
}
int32_t motor_right_encoder(void)
{
    return __HAL_TIM_GET_COUNTER(&htim3);
}
// 重置编码器的值
void motor_left_encoder_reset(void)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);
}
void motor_right_encoder_reset(void)
{
    __HAL_TIM_SET_COUNTER(&htim3, 0);
}
