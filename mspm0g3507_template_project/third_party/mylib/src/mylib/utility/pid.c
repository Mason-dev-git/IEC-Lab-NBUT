#include <mylib/utility/pid.h>
#include <float.h>

void pid_position_init(pid_position_t* pid, float kp, float ki, float kd, float min_output,
                       float max_output)
{
    pid->set_point  = 0.0;
    pid->kp         = kp;
    pid->ki         = ki;
    pid->kd         = kd;
    pid->last_error = 0.0;
    pid->prev_error = 0.0;
    pid->sum_error  = 0.0;
    pid->min_output = min_output;
    pid->max_output = max_output;
}

float pid_position_calculate(pid_position_t* pid, float current_value)
{
    float error = pid->set_point - current_value; /* 偏差 */
    pid->sum_error += error;                      /* 积分 */
    float d_error = error - pid->last_error;      /* 微分 */

    float output = pid->kp * error + pid->ki * pid->sum_error + pid->kd * d_error;

    pid->prev_error = pid->last_error;
    pid->last_error = error;

    if (pid->min_output != 0 && pid->max_output != 0) {
        // 限制输出
        if (output > pid->max_output) { output = pid->max_output; }
        else if (output < pid->min_output) {
            output = pid->min_output;
        }
    }

    return output;
}

void pid_incremental_init(pid_incremental_t* pid, float kp, float ki, float kd, float min_output,
                          float max_output)
{
    pid->kp          = kp;
    pid->ki          = ki;
    pid->kd          = kd;
    pid->last_error  = 0.0;
    pid->prev_error  = 0.0;
    pid->last_output = 0.0;
    pid->min_output  = min_output;
    pid->max_output  = max_output;
}

float pid_incremental_calculate(pid_incremental_t* pid, float set_point, float current_value)
{
    float error   = set_point - current_value; /* 当前偏差 */
    float d_error = error - pid->last_error;   /* 增量偏差 */
    float output  = pid->kp * d_error + pid->ki * error +
                   pid->kd * (error - 2 * pid->last_error + pid->prev_error);

    pid->prev_error = pid->last_error;
    pid->last_error = error;
    pid->last_output += output; /* 上一次的输出加上增量输出 */

    if (pid->min_output != 0 && pid->max_output != 0) {
        // 限制输出
        if (output > pid->max_output) { output = pid->max_output; }
        else if (output < pid->min_output) {
            output = pid->min_output;
        }
    }

    return pid->last_output;
}
