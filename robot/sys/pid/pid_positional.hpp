#ifndef __PID_POSITIONAL_H
#define __PID_POSITIONAL_H
#include "pid.hpp"
//#include "main.h"

class PID_POSITIONAL
{
private:
  /* data */

  float Kp;	//比例系数
  float Ki; 	//积分系数
  float Kd; 	//微分系数
  float p_out;	//比例输出
  float i_out;	//积分输出
  float d_out;	//微分输出
  float total_out;//总输出
  float lastError; //最后的误差值
  float prevError; //上次的误差值   
  float MaxOutput;	    //PID最大输出限制
  void limit(float *a1, float a2);//限幅(比大小函数)

  float target;		//目标值
  float present;	//当前值
  float IntegralLimit;	//积分限幅
  float I_Band;		//积分分离
public:
  PID_POSITIONAL();
  ~PID_POSITIONAL();

  
  /* 以下是你能使用的方法*/
  void Init(float targetValue, float IntegralLimitValue, float I_BandValue);
  void Set_Kp(float kp);
  void Set_Ki(float ki);
  void Set_Kd(float kd);
  void Set_Kp_Ki_Kd(float kp, float ki, float kd);
  void Set_target(float targetValue);//设置目标值
  void Set_MaxOutput(float MaxOutputValue);//设置最大输出值TODO:
  /*获取ki*/
  float Get_Ki(void);
  float Get_Kp(void);  //获取kp
  float Get_Kd(void); //获取kd
  
  void  Clear_i_out(void);//清积分
  float Calculate(float presentValue);
  float Get_total_out(void);
  /*获取目标值*/
  float Get_target(void);
};

#endif
