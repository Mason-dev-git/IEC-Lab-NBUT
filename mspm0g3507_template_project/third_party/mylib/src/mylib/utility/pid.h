#ifndef MYLIB_BASE_PID_H
#define MYLIB_BASE_PID_H

// 位置式pid
typedef struct
{
    float set_point;  /* 设定目标值 */
    float kp;         /* 比例系数 */
    float ki;         /* 积分系数 */
    float kd;         /* 微分系数 */
    float last_error; /* 上一次的误差 */
    float prev_error; /* 上上次误差 */
    float sum_error;  /* 误差和 */
    float min_output; /* 输出最小值，如果不需要就让min_output和max_output都为0 */
    float max_output; /* 输出最大值 */
} pid_position_t;

// 位置式pid结构体初始化
void pid_position_init(pid_position_t* pid, float kp, float ki, float kd, float min_output,
                       float max_output);

// 位置式pid计算
float pid_position_calculate(pid_position_t* pid, float current_value);


// 增量式pid
typedef struct
{
    float kp;          /* 比例系数 */
    float ki;          /* 积分系数 */
    float kd;          /* 微分系数 */
    float last_error;  /* 上一次的误差 */
    float prev_error;  /* 上上次误差 */
    float last_output; /* 上一次输出 */
    float min_output;  /* 输出最小值，如果不需要就让min_output和max_output都为0 */
    float max_output;  /* 输出最大值 */
} pid_incremental_t;

// 增量式pid结构体初始化
void pid_incremental_init(pid_incremental_t* pid, float kp, float ki, float kd, float min_output,
                          float max_output);

// 增量式pid计算
float pid_incremental_calculate(pid_incremental_t* pid, float set_point, float current_value);


#endif   // !MYLIB_BASE_PID_H
