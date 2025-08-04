#ifndef MYLIB_DEVICE_MOTOR_ENCODER_H
#define MYLIB_DEVICE_MOTOR_ENCODER_H

#include <mylib/mylib_config.h>
#include <stdint.h>

typedef struct
{
    GPIO_Regs* port_a;
    int32_t    pin_a;
    GPIO_Regs* port_b;
    int32_t    pin_b;
    int32_t    count;
} motor_encoder_t;

// 添加一个编码器
void motor_encoder_add(motor_encoder_t* encoder);

// 读取编码器的count
int32_t motor_encoder_get_count(motor_encoder_t* encoder);

// 清空编码器的count
void motor_encoder_clear_count(motor_encoder_t* encoder);

// 需要在外部中断里调用
void motor_encoder_irq_handler(void);


#endif   // !MYLIB_DEVICE_MOTOR_ENCODER_H
