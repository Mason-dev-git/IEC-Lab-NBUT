/*
 *左电机驱动函数
 *由定时器2通道1，定时器2通道2控制
 *2022-03-18 13:18:27
*/
#include "left_motor.hpp"
#include "tim.h"
#include <cmath>

Left_motor::Left_motor()
{
  en              = 0;
  spd             = 0;
  limit_spd       = 1000;
  dir             = Left_Brake;
}

Left_motor::~Left_motor()
{
//   printf("Bsp_motor Deleted OK!\n");
}

/**
 * @description: Left_motor初始化
 * @param {*}
 * @return {*}
 */
void Left_motor::Init(void)
{
  Disable();
  Control(Left_Brake);
  Speed(0);
}
/**
 * @description: 设置速度限幅
 * @param {*}
 * @return {*}
 */
void Left_motor::Set_limit_spd(uint16_t Set_limit_spd)
{
  limit_spd=Set_limit_spd;
}
/**
 * @description: Left_motor使能
 * @param {*}
 * @return {*}
 */
void Left_motor::Enable(void)
{
  en = 1;
  /* Enable PWM */
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);	
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
}

/**
 * @description: Left_motor失能
 * @param {*}
 * @return {*}
 */
void Left_motor::Disable(void)
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
void Left_motor::Enable(uint8_t enORdis)
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
 * @param {Left_motor_Status} status  :\
 *        Reverse =-1,//电机反转  \
 *        Brake   = 0,//电机制动  \
 *        Forward = 1,//电机正转
 * @return {*}
 */
void Left_motor::Control(Left_motor_Status status)
{
  dir = status;
  switch (status)
  {
  case Left_Reverse:
    TIM2->CCR3 =  spd;
	TIM2->CCR4 =  0;			
    break;

  case Left_Brake:
    TIM2->CCR3 =  0;
	TIM2->CCR4 =  0;			
    break;

  case Left_Forward:
    TIM2->CCR3 =  0;
	TIM2->CCR4 =  spd;   
    break;

  default:
    break;
  }
}

/**
 * @description: 电机速度控制
 * @param {uint16_t} dC，参数范围：0~1000
 * @return {*}
 */
void Left_motor::Speed(int16_t dC)
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
    dir=Left_Forward;
  }
  else if(dC<0)
  {
    dir=Left_Reverse;
  }
  else if(dC==0)
  {
    dir=Left_Brake;
  }
  
  Control(dir);
}
/*获取实时速度*/
int16_t Left_motor::Get_speed(void)
{
  if(dir==Left_Forward)
  {
    return spd;
  }
  if(dir==Left_Reverse)
  {
    return -spd;
  }
  if(dir==Left_Brake)
  {
    return 0;
  }
	return 0;
}


