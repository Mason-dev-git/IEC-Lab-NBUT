#include "pid.hpp"
#include <cmath>

pid::pid(PID_TYPE type)
{
  Kp = 0;    // 比例系数
  Kd = 0;    // 微分系数
  lastError = 0;        // 误差值
  prevError = 0;        // 上次的误差

  p_out = 0;
  d_out = 0;
  total_out = 0;
  MaxOutput = 1000;
}

pid::~pid()
{
}

/**
 * @description: 设置最大输出
 * @return {*}
 */
void pid::Set_MaxOutput(float Set_MaxOutput)
{
  MaxOutput = Set_MaxOutput;
}

/**
 * @description: 限幅函数
 * @param {float} *a1
 * @param {float} a2
 * @return {*}
 */
void pid::limit(float *a1, float a2)
{
  if (*a1 < 0)
  {
    if (__fabs(*a1) > a2) *a1 = -a2;
  }
  if (*a1 >= 0)
  {
    if (__fabs(*a1) > a2) *a1 = a2;
  }
}

/**
 * @description: 设置Kp值
 * @param {float} kp
 * @return {*}
 */
void pid::Set_Kp(float kp)
{
  Kp = kp;
}

/**
 * @description: 设置Kd
 * @param {float} kd
 * @return {*}
 */
void pid::Set_Kd(float kd)
{
  Kd = kd;
}

/**
 * @description: 设置Kp和Kd
 * @param {float} kp
 * @param {float} kd
 * @return {*}
 */
void pid::Set_Kp_Kd(float kp, float kd)
{
  Set_Kp(kp);
  Set_Kd(kd);
}

/**
 * @description: 获得上一次计算的输出
 * @param {*}
 * @return {*}
 */
float pid::Get_total_out(void)
{
  return total_out;
}

float pid::Get_Kp(void)
{
  return Kp;
}

float pid::Get_Kd(void)
{
  return Kd;
}


