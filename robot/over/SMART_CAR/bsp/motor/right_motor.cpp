/*
 *右电机驱动函数
 *由定时器2通道3，定时器2通道4控制
 *GTM+8 2022-03-18 14:02:08 GTM+8
 */
#include "right_motor.hpp"
#include "tim.h"
#include <cmath>

Right_motor::Right_motor()
{
  en              = 0;
  spd             = 0;
  limit_spd       = 1000;
  dir             = Right_Brake;
}

Right_motor::~Right_motor()
{
//   printf("Bsp_motor Deleted OK!\n");
}

/**
 * @description: Right_motor初始化
 * @param {*}
 * @return {*}
 */
void Right_motor::Init(void)
{
  Disable();
  Control(Right_Brake);
  Speed(0);
}
/**
 * @description: 设置速度限幅
 * @param {*}
 * @return {*}
 */
void Right_motor::Set_limit_spd(uint16_t Set_limit_spd)
{
  limit_spd=Set_limit_spd;
}
/**
 * @description: Right_motor使能
 * @param {*}
 * @return {*}
 */
void Right_motor::Enable(void)
{
  en = 1;
  /* Enable PWM */
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);	
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
}

/**
 * @description: Right_motor失能
 * @param {*}
 * @return {*}
 */
void Right_motor::Disable(void)
{
  en = 0;
  /* Disable PWM */
  HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_3);	
  HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_4);
}

/**
 * @description: 一个重载函数，参数；1为使能，0为失能
 * @param {uint8_t} enORdis
 * @return {*}
 */
void Right_motor::Enable(uint8_t enORdis)
{
  if(enORdis)
  {
    en = 1;
    /* Enable PWM */
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);	
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
  }else{
    en = 0;
    /* Disable PWM */
    HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_3);	
    HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_4);
  }
}

/**
 * @description: 电机控制状态
 * @param {Right_motor_Status} status  :\
 *        Reverse =-1,//电机反转  \
 *        Brake   = 0,//电机制动  \
 *        Forward = 1,//电机正转
 * @return {*}
 */
void Right_motor::Control(Right_motor_Status status)
{
  dir = status;
  switch (status)
  {
  case Right_Reverse:
    TIM2->CCR3 =  spd;
	TIM2->CCR4 =  0;			
    break;

  case Right_Brake:
    TIM2->CCR3 =  0;
  	TIM2->CCR4 =  0;			
    break;

  case Right_Forward:
    TIM2->CCR3 =  0;
  	TIM2->CCR4 =  spd;   
    break;

  default:
    break;
  }
}

/**
 * @description: 电机速度控制
 * @param {uint8_t} dC，参数范围：0~1000
 * @return {*}
 */
void Right_motor::Speed(int16_t dC)
{
  int16_t dC_temp=0;
  if(dC>0)
    dC_temp = dC;
  else
    dC_temp = -dC;
    
  if(dC_temp<limit_spd)
    spd = dC_temp;
  else
    spd = limit_spd;

  if(dC>0)
  {
    dir=Right_Forward;
  }
  else if(dC<0)
  {
    dir=Right_Reverse;
  }
  else if(dC==0)
  {
    dir=Right_Brake;
  }
  
  Control(dir);
}
/*获取实时速度*/
int16_t Right_motor::Get_speed(void)
{
  if(dir==Right_Forward)
  {
    return spd;
  }
  else if(dir==Right_Reverse)
  {
    return -spd;
  }
  else if(dir==Right_Brake)
  {
    return 0;
  }
	return 0;
}





