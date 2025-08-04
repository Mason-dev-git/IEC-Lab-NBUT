// 使用方法：
#if 0

// 前左轮电机 pwmA->PA12,in1->PB14,in2->PB16
motor_t g_motor_fl = {
    .timer = PWM_FRONT_MOTOR_INST,
    .channel = GPIO_PWM_FRONT_MOTOR_C0_IDX,
    .max_peiod = 3200,
    .current_val = 0,
    .port_in1 = GPIO_GRP_MOTOR_PORT,
    .pin_in1 = GPIO_GRP_MOTOR_PIN_FRONT_LEFT_A_PIN,
    .port_in2 = GPIO_GRP_MOTOR_PORT,
    .pin_in2 = GPIO_GRP_MOTOR_PIN_FRONT_LEFT_B_PIN,
    .rotate = MOTOR_DIRECTION_COROTATION_IN1
};

// 前右轮电机 pwmB->PA13,in1->PB13,in2->PB15
motor_t g_motor_fr = {
    .timer = PWM_FRONT_MOTOR_INST,
    .channel = GPIO_PWM_FRONT_MOTOR_C1_IDX,
    .max_peiod = 3200,
    .current_val = 0,
    .port_in1 = GPIO_GRP_MOTOR_PORT,
    .pin_in1 = GPIO_GRP_MOTOR_PIN_FRONT_RIGHT_A_PIN,
    .port_in2 = GPIO_GRP_MOTOR_PORT,
    .pin_in2 = GPIO_GRP_MOTOR_PIN_FRONT_RIGHT_B_PIN,
    .rotate = MOTOR_DIRECTION_COROTATION_IN1
};

    // 设置占空比为0.3和0.6
    motor_set_duty(&g_motor_fl, 30);
    motor_set_duty(&g_motor_fr, 60);
    // 设置方向为正转
    motor_set_direction(&g_motor_fl, motor_direction_corotation);
    motor_set_direction(&g_motor_fr, motor_direction_corotation);
    
    delay_cycles(DELAY*5);
    
    // 关闭电机
    motor_off(&g_motor_fl);
    motor_off(&g_motor_fr);
    
    delay_cycles(DELAY*5);
    
    // 设置占空比为0.6和0.3
    motor_set_duty(&g_motor_fl, 60);
    motor_set_duty(&g_motor_fr, 30);
    // 设置方向为反转
    motor_set_direction(&g_motor_fl, motor_direction_inversion);
    motor_set_direction(&g_motor_fr, motor_direction_inversion);

#endif

#ifndef DEVICE_MOTOR_H
#    define DEVICE_MOTOR_H

#    include "bsp.h"

// 标记电机正转所需的in
// 例如现在是IN1为高电平时候正转，那么就填MOTOR_DIRECTION_COROTATION_IN1
#    define MOTOR_DIRECTION_COROTATION_IN1 1
#    define MOTOR_DIRECTION_COROTATION_IN2 2

typedef enum
{
    motor_direction_corotation,
    motor_direction_inversion
} motor_direction;

typedef struct
{
    GPTIMER_Regs*     timer;
    DL_TIMER_CC_INDEX channel;
    int32_t           current_val;
    int32_t           max_peiod;
    GPIO_Regs*        port_in1;
    int32_t           pin_in1;
    GPIO_Regs*        port_in2;
    int32_t           pin_in2;
    int8_t            rotate;   // 转向，使用MOTOR_DIRECTION_xxx宏
} motor_t;

// 设置compare值
void motor_set_compare_val(motor_t* motor, int32_t cmp_val);
// 设置占空比，duty是占空比，最大是3200。
void motor_set_duty(motor_t* motor, int32_t duty);
// 设置电机的转向
void motor_set_direction(motor_t* motor, motor_direction dir);
// 关闭电机
void motor_off(motor_t* motor);


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

int32_t motor_left_encoder(void);
int32_t motor_right_encoder(void);
// 重置编码器的值
void motor_left_encoder_reset(void);
void motor_right_encoder_reset(void);

////////////////////////////////////////////////////////


// 前右轮电机 pwmB->PA6,in1->PB13,in2->PB15
extern motor_t g_motor_fl;

// 前左轮电机 pwmA->PA5,in1->PB14,in2->PB16
extern motor_t g_motor_fr;

// 编码器，e1a->B0,e1b->B1
extern motor_encoder_t g_encoder_e1;

// 编码器，e2a->B2,e2b->B3
extern motor_encoder_t g_encoder_e2;

void motor_set_speed(int32_t speed_left, int32_t speed_right);

#endif   // !DEVICE_MOTOR_H
