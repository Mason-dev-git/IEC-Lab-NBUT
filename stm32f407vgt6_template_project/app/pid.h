/**
 * @file pid.h
 * @brief pid算法，包含位置式和增量式pid计算
 * 使用方法：
 * 1. 创建pid对象，并初始化
 *   pid_position_t pid;
 *   pid_position_init(&pid, 0.1, 0.01, 0.001, 100);
 * 2. pid计算，并应用于系统（这里以电机设置占空比为例）
 *   int duty = pid_position_calculate(&pid, current_value);
 *   motor_set_duty(duty);
 *
 * @version 0.1
 *
 */

#ifndef PID_H
#define PID_H

/**
 * @brief 位置式pid对象
 *
 */
typedef struct
{
    float target;          // 设定目标值
    float kp;              // 比例系数
    float ki;              // 积分系数
    float kd;              // 微分系数
    float last_error;      // 最后一次的误差
    float sum_error;       // 误差和
    float isep;            // 积分分离
    float max_out_kp;      // kp输出限幅
    float max_out_ki;      // ki输出限幅
    float max_out_kd;      // kd输出限幅
    float max_out_total;   // 总输出限幅
} pid_position_t;


/**
 * @brief 增量式pid对象
 *
 */
typedef struct
{
    float target;       // 设定目标值
    float kp;           // 比例系数
    float ki;           // 积分系数 ki=kp*T/Ti
    float kd;           // 微分系数 kd=kp*Td/T
    float last_error;   // 最后一次的误差
    float prev_error;   // 上一次误差
    float max_out_kp;   // kp输出限幅
    float max_out_ki;   // ki输出限幅
    float max_out_kd;   // kd输出限幅
    float max_out_total;   // 输出限幅
} pid_incremental_t;



void pid_position_init(pid_position_t* pid, float kp, float ki, float kd, float target, float isep, float max_output);
void set_position_target(pid_position_t *pid, float target);
void set_position_pid(pid_position_t *pid, float kp, float ki, float kd);
void set_position_max_output(pid_position_t *pid, float max_out_kp, float max_out_ki, float max_out_kd, float max_out_total);
void set_position_isep(pid_position_t *pid, float isep);
float pid_position_calculate(pid_position_t* pid, float current_value);

void pid_incremental_init(pid_incremental_t* pid, float kp, float ki, float kd, float target, float max_output);
void set_incremental_target(pid_incremental_t *pid, float target);
void set_incremental_pid(pid_incremental_t *pid, float kp, float ki, float kd);
void set_incremental_max_output(pid_incremental_t *pid, float max_out_kp, float max_out_ki, float max_out_kd, float max_out_total);
float pid_incremental_calculate(pid_incremental_t* pid, float current_value);

/**
 * @brief 位置式pid计算yaw(传入误差)
 * 
 * @param pid 位置式pid控制对象
 * @param error 当前误差
 * @return float 输出控制量
 */
float pid_position_calculate_yaw(pid_position_t* pid, float error);



#endif   // !PID_H
