#ifndef __PATROLLINE_HPP
#define __PATROLLINE_HPP

#include "pid_incremental.hpp"
#include "pack.h"
#include "adc.h"
#include <stdlib.h>

extern uint16_t ad_now[8];//adc采样当前值
extern uint16_t ad_max[8];//adc采样最大值
extern uint16_t ad_min[8];//adc采样最小值
extern uint16_t ad_avr[8];//adc采样平均值


class PatrolLine//定义所有产生pid误差的对象的类
{
private:
  uint8_t compare_ad_left,compare_ad_right;
  PID_INCREMENTAL PatrolLinePID;//PID控制器
  int32_t compensation;//Compensation补偿

public:
	PatrolLine(/* args */);
	~PatrolLine();

  /* 以下是你能使用的方法 */
  void Init(void);//ADC DMA初始化
  void Value_Handling(void);//巡线值最大最小处理
   
  void SetPID_MaxOutput(float Set_MaxOutput);//PID设置最大输出
  void SetPID_Kp_Kd(float kp, float kd);//设置光敏二极管PID参数
  void SetPID_Kp_Kd_yaw(float kp, float kd);//设置陀螺仪PID参数
  
  void SetcompareAD(uint8_t left,uint8_t right);/*设置比较AD通道*/
  int32_t Get_error();//获取误差
  float Get_yaw_compensation();
  void set_yaw_compensation();
  float Get_error_yaw();
  int32_t Get_ADmin_error();/*获取AD最小值差值用以补偿两光电误差*/
  int32_t Get_Compensation_error();/*获取补偿后的误差*/
  int32_t Calcu_Compensation();//计算光敏二极管误差并返回补偿
  int32_t Yaw_Compensation();  //计算陀螺仪误差并返回补偿
  int32_t Read_Compensation();//仅读补偿
  int32_t LCDshow_Compensation(); //仅给LCD读补偿
  
  uint16_t Get_ad_now(uint8_t num);
  float GetPID_Kp(void);  //获取kp
  float GetPID_Kd(void); //获取kd
  uint8_t compare_ad();   //判断往左还是往右 0：左边高 1：右边高
  uint8_t ad_if_white(uint8_t ad_value);/*判断灰度口是否上白线 参数：ad口的值 return 1：（上线）0：（未上线） */
  int32_t ad_compensation();/*ad补偿值*/
};




#endif
