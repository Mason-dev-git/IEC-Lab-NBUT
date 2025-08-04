/**
 * @file pid.c
 * @brief pid算法，包含位置式和增量式pid计算
 * @version 0.1
 *
 */

#include "pid.h"
#include "math_util.h"

/**
 * @brief pid输出限幅
 * 
 * @param output 计算输出值
 * @param max_output 最大输出值
 * @return float 实际输出值
 */
float pid_out_clamp(float output, float max_output)
{
    if (abs(output) > max_output) {
        if (output > 0) {
            return max_output;
        }
        else {
            return -max_output;
        }
    }
    else {
        return output;
    }
}


/**
 * @brief 位置式pid控制对象初始化
 * 
 * @param pid 位置式pid控制对象
 * @param kp 比例项（推荐值：）
 * @param ki 积分项（推荐值：）
 * @param kd 微分项（推荐值：）
 * @param target 目标值
 * @param isep 积分分离阈值（推荐值：）
 * @param max_output 输出限幅（推荐值：）
 */
void pid_position_init(pid_position_t* pid, float kp, float ki, float kd, float target, float isep, float max_output)
{
    pid->target = target;
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->last_error = 0.0;
    pid->sum_error = 0.0;
    pid->isep = isep;
    pid->max_out_kp = max_output;
    pid->max_out_ki = max_output;
    pid->max_out_kd = max_output;
    pid->max_out_total = max_output;
}

/**
 * @brief 位置式pid设置目标值
 * 
 * @param pid 位置式pid对象
 * @param target 目标值
 */
void set_position_target(pid_position_t *pid, float target)
{
    pid->target = target;
}

/**
 * @brief 位置式pid设置参数
 * 
 * @param pid 位置式pid对象
 * @param kp 比例项参数
 * @param ki 积分项参数
 * @param kd 微分项参数
 */
void set_position_pid(pid_position_t *pid, float kp, float ki, float kd)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

/**
 * @brief 位置式pid设置输出限幅
 * 
 * @param pid 位置式pid对象
 * @param max_out_kp 比例项输出限幅
 * @param max_out_ki 积分项输出限幅
 * @param max_out_kd 微分项输出限幅
 * @param max_out_total 输出总限幅
 */
void set_position_max_output(pid_position_t *pid, float max_out_kp, float max_out_ki, float max_out_kd, float max_out_total)
{
    pid->max_out_kp = max_out_kp;
    pid->max_out_ki = max_out_ki;
    pid->max_out_kd = max_out_kd;
    pid->max_out_total = max_out_total;
}

/**
 * @brief 位置式pid设置积分分离阈值
 * 
 * @param pid 位置式pid对象
 * @param isep 积分分离阈值
 */
void set_position_isep(pid_position_t *pid, float isep)
{
    pid->isep = isep;
}

/**
 * @brief 位置式pid计算
 * 
 * @param pid 位置式pid控制对象
 * @param current_value 当前值
 * @return float 输出控制量
 */
float pid_position_calculate(pid_position_t* pid, float current_value)
{
    float error, d_error, kp_out, ki_out, kd_out, output;

    error = pid->target - current_value;   // 当前误差 = 目标值 - 当前值
    pid->sum_error += error;               // 误差积分
    d_error = error - pid->last_error;     // 误差微分

    kp_out = pid_out_clamp(pid->kp * error, pid->max_out_kp);   // 比例项输出
    if (error < pid->isep) {
        ki_out = 0;   // 积分分离
    }
    else {
        ki_out = pid_out_clamp(pid->ki * pid->sum_error, pid->max_out_ki);   // 积分项输出
    }
    kd_out = pid_out_clamp(pid->kd * d_error, pid->max_out_kd);             // 微分项输出
    output = pid_out_clamp(kp_out + ki_out + kd_out, pid->max_out_total);   // 总输出

    pid->last_error = error;   // 误差数据更新

    return output;
}



/**
 * @brief 位置式pid计算yaw(传入误差)
 * 
 * @param pid 位置式pid控制对象
 * @param error 当前误差
 * @return float 输出控制量
 */
float pid_position_calculate_yaw(pid_position_t* pid, float error)
{
    float d_error, kp_out, ki_out, kd_out, output;

    pid->sum_error += error;               // 误差积分
    d_error = error - pid->last_error;     // 误差微分

    kp_out = pid_out_clamp(pid->kp * error, pid->max_out_kp);   // 比例项输出
    
        ki_out = pid_out_clamp(pid->ki * pid->sum_error, pid->max_out_ki);   // 积分项输出

    kd_out = pid_out_clamp(pid->kd * d_error, pid->max_out_kd);             // 微分项输出
    output = pid_out_clamp(kp_out + ki_out + kd_out, pid->max_out_total);   // 总输出

    pid->last_error = error;   // 误差数据更新

    return output;
}




/**
 * @brief 增量式pid控制对象初始化
 * 
 * @param pid 增量式pid控制对象
 * @param kp 比例项（推荐值：0.15）
 * @param ki 积分项（推荐值：0.5）
 * @param kd 微分项（推荐值：0）
 * @param target 目标值
 * @param max_output 输出限幅（推荐值：1000）
 */
void pid_incremental_init(pid_incremental_t* pid, float kp, float ki, float kd, float target, float max_output)
{
    pid->target = target;
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->last_error = 0.0;
    pid->prev_error = 0.0;
    pid->max_out_kp = max_output;
    pid->max_out_ki = max_output;
    pid->max_out_kd = max_output;
    pid->max_out_total = max_output;
}

/**
 * @brief 增量式pid设置目标值
 * 
 * @param pid 位置式pid对象
 * @param target 目标值
 */
void set_incremental_target(pid_incremental_t *pid, float target)
{
    pid->target = target;
}

/**
 * @brief 增量式pid设置参数
 * 
 * @param pid 位置式pid对象
 * @param kp 比例项参数
 * @param ki 积分项参数
 * @param kd 微分项参数
 */
void set_incremental_pid(pid_incremental_t *pid, float kp, float ki, float kd)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

/**
 * @brief 增量式pid设置输出限幅
 * 
 * @param pid 位置式pid对象
 * @param max_out_kp 比例项输出限幅
 * @param max_out_ki 积分项输出限幅
 * @param max_out_kd 微分项输出限幅
 * @param max_out_total 输出总限幅
 */
void set_incremental_max_output(pid_incremental_t *pid, float max_out_kp, float max_out_ki, float max_out_kd, float max_out_total)
{
    pid->max_out_kp = max_out_kp;
    pid->max_out_ki = max_out_ki;
    pid->max_out_kd = max_out_kd;
    pid->max_out_total = max_out_total;
}

/**
 * @brief 增量式pid计算
 * 
 * @param pid 增量式pid控制对象
 * @param current_value 当前值
 * @return float 输出控制增量
 */
float pid_incremental_calculate(pid_incremental_t* pid, float current_value)
{
    float error, d_error, kp_out, ki_out, kd_out, output;

    error = pid->target - current_value;   // 当前误差 = 目标值 - 当前值

    kp_out = pid_out_clamp(pid->kp * (2*error - pid->last_error), pid->max_out_kp);   // Kp[e(k)-e(k-1)]
#if USE_13_MOTOR
    ki_out = pid_out_clamp(pid->ki * error, pid->max_out_ki);                       // Ki*e(k)
#else
    ki_out = abs(error) < 5 ? 0 : pid_out_clamp(pid->ki * error, pid->max_out_ki);   // Ki*e(k)
#endif
    kd_out = pid_out_clamp(pid->kd * (error - 2 * pid->last_error + pid->prev_error),pid->max_out_kd);// Kd[e(k)-2e(k-1)+e(k-2)]
    output = pid_out_clamp(kp_out + ki_out + kd_out, pid->max_out_total);   // 总输出限幅

    pid->prev_error = pid->last_error;   // 误差数据更新
    pid->last_error = error;

    return output;
}








