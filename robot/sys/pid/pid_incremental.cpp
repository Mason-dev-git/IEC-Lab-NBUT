#include "pid_incremental.hpp"

PID_INCREMENTAL::PID_INCREMENTAL():pid(INCREMENTAL)
{
  Kp_yaw=0;	// 偏航角比例系数
  Kd_yaw=0; // 偏航角微分系数
  lastError_yaw=0;	// 偏航角误差值
  prevError_yaw=0; // 偏航角上次的误差

  p_out_yaw=0;
  d_out_yaw=0;
  total_out_yaw=0;
  MaxOutput_yaw=200;
  MaxOutput=90;
}

PID_INCREMENTAL::~PID_INCREMENTAL()
{
}

/**
 * @description: 计算并返回一次输出结果
 * @param {float} new_error
 * @return {*}
 * @author: ZhangRiven
 */
float PID_INCREMENTAL::Calculate(float newError)
{
	/*传参赋值*/
  lastError = newError;
  /*比例*/
  p_out = Kp * lastError;//比例输出
  /*微分*/
  d_out = Kd * (lastError - prevError);
  /*总和*/
  total_out = p_out/*  + i_out */ + d_out;
  /*总输出限幅*/
  limit(&(total_out), MaxOutput);
  /*新旧值变更*/
  prevError = lastError;
  return total_out;
}

/**
 * @description: 设置偏航角最大输出
 * @return {*}
 */
void PID_INCREMENTAL::Set_MaxOutput_yaw(float Set_MaxOutput)
{
  MaxOutput_yaw=Set_MaxOutput;
}

/**
 * @description: 设置偏航角比例系数
 * @param {float} Kp_yaw
 * @return {*}
 */
void PID_INCREMENTAL::Set_Kp_yaw(float kp_yaw)
{
  Kp_yaw=kp_yaw;
}

/**
 * @description: 设置偏航角微分系数
 * @param {float} Kd_yaw
 * @return {*}
 */
void PID_INCREMENTAL::Set_Kd_yaw(float kd_yaw)
{
  Kd_yaw=kd_yaw;
}

/**
 * @description: 设置偏航角的Kp和Kd
 * @param {float} Kp_yaw
 * @param {float} Kd_yaw
 * @return {*}
 */
void PID_INCREMENTAL::Set_Kp_Kd_yaw(float Kp_yaw, float Kd_yaw)
{
  Set_Kp_yaw(Kp_yaw);
  Set_Kd_yaw(Kd_yaw);
}

/**
 * @description: 获得上一次计算的偏航角输出
 * @param {*}
 * @return {*}
 */
float PID_INCREMENTAL::Get_total_out_yaw(void)
{
  return total_out_yaw;
}

float PID_INCREMENTAL::Get_Kp_yaw(void)
{
  return Kp_yaw;
}
float PID_INCREMENTAL::Get_Kd_yaw(void)
{
  return Kd_yaw;
}


float PID_INCREMENTAL::Calculate_yaw(float newError)
{
	/*传参赋值*/
  lastError_yaw = newError;
  /*比例*/
  p_out_yaw = Kp_yaw * lastError_yaw;//比例输出
  /*微分*/
  d_out_yaw = Kd_yaw * (lastError_yaw - prevError_yaw);
  /*总和*/
  total_out_yaw = p_out_yaw+ d_out_yaw;
  /*总输出限幅*/
  limit(&(total_out_yaw), MaxOutput_yaw);
  /*新旧值变更*/
  prevError_yaw = lastError_yaw;
  return total_out_yaw;
}


