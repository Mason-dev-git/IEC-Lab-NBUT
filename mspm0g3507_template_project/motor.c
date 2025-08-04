#include "motor.h"

void motor_set_compare_val(motor_t* motor, int32_t cmp_val)
{
    motor->current_val = cmp_val;
    DL_TimerA_setCaptureCompareValue(motor->timer, motor->current_val, motor->channel);
}

void motor_set_duty(motor_t* motor, int32_t duty)
{
    if (duty < 0) {
        // 避免负占空比
        duty = -duty;
    }
    motor->current_val = motor->max_peiod - duty;
    //    DEBUG_INFO("motor->current_val = %d", motor->current_val);
    //    DEBUG_INFO("motor->timer = %d", (int)motor->timer);
    //    DEBUG_INFO("motor->channel = %d", motor->channel);
    DL_TimerA_setCaptureCompareValue(motor->timer, motor->current_val, motor->channel);
}

// 设置电机的转向
void motor_set_direction(motor_t* motor, motor_direction dir)
{
    if (motor_direction_corotation == dir) {
        // 如果是正转
        if (motor->rotate == MOTOR_DIRECTION_COROTATION_IN1) {
            // IN1为正转
            DL_GPIO_setPins(motor->port_in1, motor->pin_in1);
            DL_GPIO_clearPins(motor->port_in2, motor->pin_in2);
        }
        else {
            // IN2为正转
            DL_GPIO_setPins(motor->port_in2, motor->pin_in2);
            DL_GPIO_clearPins(motor->port_in1, motor->pin_in1);
        }
    }
    else {
        // 如果是反转
        if (motor->rotate == MOTOR_DIRECTION_COROTATION_IN1) {
            // IN2为正转
            DL_GPIO_setPins(motor->port_in2, motor->pin_in2);
            DL_GPIO_clearPins(motor->port_in1, motor->pin_in1);
        }
        else {
            // IN1为正转
            DL_GPIO_setPins(motor->port_in1, motor->pin_in1);
            DL_GPIO_clearPins(motor->port_in2, motor->pin_in2);
        }
    }
}

// 关闭电机
void motor_off(motor_t* motor)
{
    DL_GPIO_clearPins(motor->port_in1, motor->pin_in1);
    DL_GPIO_clearPins(motor->port_in2, motor->pin_in2);
}

int32_t left_e = 0;
int32_t right_e = 0;
// 获取编码的数据
int32_t motor_left_encoder(void)
{
    return left_e;
}
int32_t motor_right_encoder(void)
{
    return right_e;
}
// 重置编码器的值
void motor_left_encoder_reset(void)
{
    left_e = 0;
}
void motor_right_encoder_reset(void)
{
    right_e = 0;
}

// 需要在外部中断里调用
void motor_encoder_irq_handler(void)
{
    // 处理左轮
    uint32_t gpio = DL_GPIO_getEnabledInterruptStatus(GPIO_GRP_ENCODER_PIN_E1A_PORT, GPIO_GRP_ENCODER_PIN_E1A_PIN);
    if (gpio & GPIO_GRP_ENCODER_PIN_E1A_PIN) {
        if (DL_GPIO_readPins(GPIO_GRP_ENCODER_PIN_E1B_PORT, GPIO_GRP_ENCODER_PIN_E1B_PIN))
            left_e++;
        else
            left_e--;
        DL_GPIO_clearInterruptStatus(GPIO_GRP_ENCODER_PIN_E1A_PORT, GPIO_GRP_ENCODER_PIN_E1A_PIN);
    }

    // 处理右轮
    gpio = DL_GPIO_getEnabledInterruptStatus(GPIO_GRP_ENCODER_PIN_E2A_PORT, GPIO_GRP_ENCODER_PIN_E2A_PIN);
    if (gpio & GPIO_GRP_ENCODER_PIN_E2A_PIN) {
        if (DL_GPIO_readPins(GPIO_GRP_ENCODER_PIN_E2B_PORT, GPIO_GRP_ENCODER_PIN_E2B_PIN))
            right_e--;
        else
            right_e++;
        DL_GPIO_clearInterruptStatus(GPIO_GRP_ENCODER_PIN_E2A_PORT, GPIO_GRP_ENCODER_PIN_E2A_PIN);
    }
}

// 前右轮电机 pwmB->PA6,in1->PB13,in2->PB15
motor_t g_motor_fl = {.timer = PWM_FRONT_MOTOR_INST,
                      .channel = GPIO_PWM_FRONT_MOTOR_C1_IDX,
                      .max_peiod = 10000,
                      .current_val = 0,
                      .port_in1 = GPIO_GRP_MOTOR_PORT,
                      .pin_in1 = GPIO_GRP_MOTOR_PIN_FRONT_LEFT_A_PIN,
                      .port_in2 = GPIO_GRP_MOTOR_PORT,
                      .pin_in2 = GPIO_GRP_MOTOR_PIN_FRONT_LEFT_B_PIN,
                      .rotate = MOTOR_DIRECTION_COROTATION_IN1};


// 前左轮电机 pwmA->PA5,in1->PB14,in2->PB16
motor_t g_motor_fr = {.timer = PWM_FRONT_MOTOR_INST,
                      .channel = GPIO_PWM_FRONT_MOTOR_C0_IDX,
                      .max_peiod = 10000,
                      .current_val = 0,
                      .port_in1 = GPIO_GRP_MOTOR_PORT,
                      .pin_in1 = GPIO_GRP_MOTOR_PIN_FRONT_RIGHT_A_PIN,
                      .port_in2 = GPIO_GRP_MOTOR_PORT,
                      .pin_in2 = GPIO_GRP_MOTOR_PIN_FRONT_RIGHT_B_PIN,
                      .rotate = MOTOR_DIRECTION_COROTATION_IN1};


void motor_set_speed(int32_t speed_left, int32_t speed_right)
{
    if (speed_left > 0) {
        DL_GPIO_setPins(GPIO_GRP_MOTOR_PORT, GPIO_GRP_MOTOR_PIN_FRONT_LEFT_A_PIN);
        DL_GPIO_clearPins(GPIO_GRP_MOTOR_PORT, GPIO_GRP_MOTOR_PIN_FRONT_LEFT_B_PIN);
    }
    else {
        DL_GPIO_clearPins(GPIO_GRP_MOTOR_PORT, GPIO_GRP_MOTOR_PIN_FRONT_LEFT_A_PIN);
        DL_GPIO_setPins(GPIO_GRP_MOTOR_PORT, GPIO_GRP_MOTOR_PIN_FRONT_LEFT_B_PIN);
    }
    if (speed_right > 0) {
        DL_GPIO_setPins(GPIO_GRP_MOTOR_PORT, GPIO_GRP_MOTOR_PIN_FRONT_RIGHT_A_PIN);
        DL_GPIO_clearPins(GPIO_GRP_MOTOR_PORT, GPIO_GRP_MOTOR_PIN_FRONT_RIGHT_B_PIN);
    }
    else {
        DL_GPIO_clearPins(GPIO_GRP_MOTOR_PORT, GPIO_GRP_MOTOR_PIN_FRONT_RIGHT_A_PIN);
        DL_GPIO_setPins(GPIO_GRP_MOTOR_PORT, GPIO_GRP_MOTOR_PIN_FRONT_RIGHT_B_PIN);
    }

    motor_set_compare_val(&g_motor_fr, abs(speed_right));
    motor_set_compare_val(&g_motor_fl, abs(speed_left));
}
