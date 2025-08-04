// 接线
// 驱动板 --- 单片机
// BIN1 --- PA12
// BIN2 --- PC10
// PWMB --- PD13
// E2A --- PB4
// E2B --- PB5
#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>
#include <stdbool.h>

// 初始化电机，打开pwm
void motor_init(void);
// 让电机停止运行
void motor_stop(void);
// 正代表正转
// 负代表反转
void motor_set_speed(int32_t speed_left, int32_t speed_right);
// 获取编码的数据
int32_t motor_left_encoder(void);
int32_t motor_right_encoder(void);
// 重置编码器的值
void motor_left_encoder_reset(void);
void motor_right_encoder_reset(void);

#endif // MOTOR_H
