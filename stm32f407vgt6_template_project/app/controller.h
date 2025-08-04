#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "bsp.h"
#include "pid.h"

typedef enum{
    // 停止状态
    STOP,
    // 巡线状态
    LINE_CONTROL,
    // 走直线状态
    DIRECT_CONTROL,
    DISTANCE_CONTROL,
    SPEED_CONTROL,
    STEERING_CONTROL
}CurrentControlMode;

void Controller_Init(void);
void Controller_IRQHandler(void);

void LineControl(float speed);
void LineControlInit(void);
void LineControl_IRQHandler(void);

void DirectControl(float speed);
void DirectControlInit(void);
void DirectControl_IRQHandler(void);

/**
 * @brief 速度环控制，输入目标的速度，自动设置电机的占空比
 * 
 * @param ltarget 左轮的目标速度，以编码器脉冲数为单位
 * @param rtarget 右轮的目标速度，以编码器脉冲数为单位
 */
void speed_loop_control(float ltarget, float rtarget);

/**
 * @brief 巡线控制
 * 
 */
void track_control(void);

#endif
