#include "pid_positional.hpp"
#include <cmath>

PID_POSITIONAL::PID_POSITIONAL()
{
  Kp=0;	//比例系数
  Kd=0; //微分系数
  lastError=0;		//误差值
  prevError=0; //上次的误差
  p_out=0;
  d_out=0;
  total_out=0;

  MaxOutput   =1000; //PID最大输出限制
  Ki          =0; //积分系数
  i_out       =0;	//积分输出
  target 			=0; //目标值
  present     =0; //当前值
  IntegralLimit=0;//积分限幅 就是最大积分补偿
  I_Band      =0;	//积分分离 就是离目标距离还有多少开始补偿积分项
}

PID_POSITIONAL::~PID_POSITIONAL()
{
}

/**
 * @description: 限幅函数
 * @param {float} *a1
 * @param {float} a2
 * @return {*}
 */
void PID_POSITIONAL::limit(float *a1, float a2)
{
  if(*a1<0)
  {
     if(__fabs(*a1)>a2) *a1=-a2;
  }
  if(*a1>=0)
  {
     if(__fabs(*a1)>a2) *a1=a2;
  }
}

/**
 * @description: 设置最大输出
 * @return {*}
 */
void PID_POSITIONAL::Set_MaxOutput(float Set_MaxOutput)
{
  MaxOutput=Set_MaxOutput;
}

/**
 * @description: 设置kp
 * @param {float} kp
 * @return {*}
 */
void PID_POSITIONAL::Set_Kp(float kp)
{
  Kp=kp;
}

/**
 * @description:设置kd
 * @param {float} kd
 * @return {*}
 */
void PID_POSITIONAL::Set_Kd(float kd)
{
  Kd=kd;
}

/**
 * @description: 获得上一次计算的输出
 * @param {*}
 * @return {*}
 */
float PID_POSITIONAL::Get_total_out(void)
{
  return total_out;
}

float PID_POSITIONAL::Get_Kp(void)
{
  return Kp;
}
float PID_POSITIONAL::Get_Kd(void)
{
  return Kd;
}

void PID_POSITIONAL::Init(float targetValue, float IntegralLimitValue, float I_BandValue)
{
  target = targetValue;
  IntegralLimit = IntegralLimitValue;
  I_Band = I_BandValue;
  Clear_i_out();
}

/**
 * @description: 设置ki值
 * @param {float} ki
 * @return {*}
 * @author: ZhangRiven
 */
void PID_POSITIONAL::Set_Ki(float ki)
{
  Ki=ki;
}

/**
 * @description: 设置Kp、Ki、Kd
 * @param {float} kp
 * @param {float} ki
 * @param {float} kd
 * @return {*}
 * @author: ZhangRiven
 */
void PID_POSITIONAL::Set_Kp_Ki_Kd(float kp, float ki, float kd)
{
  Set_Kp(kp);
  Set_Ki(ki);
  Set_Kd(kd);
}

/**
 * @description: 设置目标值
 * @param {float} targetValue
 * @return {*}
 * @author: ZhangRiven
 */
void PID_POSITIONAL::Set_target(float targetValue)
{
  target = targetValue;
}

/*获取ki*/
float PID_POSITIONAL::Get_Ki(void)
{
  return Ki;
}

void PID_POSITIONAL::Clear_i_out(void)
{
  i_out=0;
}
/**
 * @description: 计算并返回一次输出结果
 * @param {float} new_error
 * @return {*}
 */
float PID_POSITIONAL::Calculate(float present)
{
  lastError = target - present;
  /*比例*/
  p_out = Kp * lastError;//比例输出
  /*积分*/
  if(Ki!=0)
  {
    if(__fabs(present) < I_Band)
    {
      i_out = Ki * present;
      limit(&(i_out), IntegralLimit);
    }
    else
    {
      // i_out=0;
      i_out =Ki*I_Band*(float)(lastError/I_Band);
    }    
  }
  /*微分*/
  d_out = Kd * (lastError - prevError);
  /*总和*/
  total_out = p_out + i_out + d_out;
  /*总输出限幅*/
  limit(&(total_out), MaxOutput);
  /*新旧值变更*/
  prevError = lastError;
  /*以下比较防止输出错误*/
  if(target<=present)
  {
    return 0;
  }
  else
  {
    return total_out;
  }
  
}

/*获取目标值*/
float PID_POSITIONAL::Get_target(void)
{
    return target;
}
