#include <mylib/device/device_motor.h>
#include <mylib/utility/debug.h>

void motor_set_compare_val(motor_t* motor, int32_t cmp_val)
{
    motor->current_val = (3200 + cmp_val) % 3200;
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
