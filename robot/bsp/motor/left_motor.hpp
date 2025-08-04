/*
 *左电机驱动函数
 *由定时器2通道1，定时器2通道2控制
 *2022-03-18 13:18:27
*/
#ifndef __LEFT_MOTOR_HPP
#define __LEFT_MOTOR_HPP

#include "main.h"

typedef enum{
  Left_Reverse =-1,//电机反转
  Left_Brake   = 0,//电机制动
  Left_Forward = 1,//电机正转
} Left_motor_Status;

// simple base class
class Left_motor
{
private:
  /* data */
  uint8_t en;                                  //使能
  uint16_t spd;                                //速度
  uint16_t limit_spd;                          //速度限幅
  Left_motor_Status dir;                       //方向

public:
  Left_motor();
  ~Left_motor();

  /**********以下是你能使用的函数**********/

  void Init(void);                             //Bsp_motor初始化
  void Set_limit_spd(uint16_t Set_limit_spd);  //设置速度限幅
  void Enable(void);                           //Bsp_motor使能
  void Disable(void);                          //Bsp_motor失能 
  void Enable(uint8_t enORdis);                //一个重载函数，参数；1为使能，0为失能
  void Control(Left_motor_Status status);      //电机状态控制
  void Speed(int16_t dC);                      //电机速度控制，参数：0~1000
  int16_t Get_speed(void);                     //获取实时速度
};



#endif


