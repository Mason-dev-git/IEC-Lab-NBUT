/*
 *  左右电机同时驱动控制程序
 GTM+8 2022-03-18 14:18:49 GTM+8
 */
#ifndef __MOTOR_HPP
#define __MOTOR_HPP
#include "main.h"
#include "right_motor.hpp"
#include "left_motor.hpp"

class Motor//定义控制电机速度的对象
{
private:
  /* data */
  Left_motor      LeftMotor;       //左马达
  Right_motor     RightMotor;      //右马达
public:
  Motor();
  ~Motor();

  /**********以下是你能使用的函数**********/


  /*电机基础函数*/

  //motor初始化
  void Init(void);
  //设置速度限幅
  void Set_limit_spd(uint16_t Set_limit_spd);
  //motor使能
  void Enable(void);
  //motor失能
  void Disable(void);
  //一个重载函数，参数：1为使能，0为失能
  void Enable(uint8_t enORdis);
  //电机控制状态参数：Reverse =-1,//电机反转  Brake   = 0,//电机制动   Forward = 1,//电机正转
  void Control(Left_motor_Status Lstatus,Right_motor_Status Rstatus);
  //双电机速度控制，参数：-1000~1000
  void Speed(int16_t two_speed);
  /*函数重载 Left_speed控制左轮，Right_speed控制右轮*/
  void Speed(int16_t Left_speed,int16_t Right_speed);
  /*获取左轮实时速度*/
  int16_t Get_Left_Speed(void);
    /*获取右轮实时速度*/
  int16_t Get_Right_Speed(void);


  /*电机控制函数*/

  /*直线前进*/
  void Run_straight(int16_t speed);
  /*停止一段时间*/
  void Run_stoptime(uint16_t time);
  /*后退*/
  void Run_back(int16_t speed);
  /*右转*/
  void turn_right(int16_t speed);
  /*右转延时 参数：速度，时间*/
  void right_delay(int16_t speed,uint16_t times);
  /*左转*/
  void turn_left(int16_t speed);
  /*左转延时 参数：速度，时间*/
  void left_delay(int16_t speed,uint16_t times);
  /*右转，函数重载，转弯，参数：速度，左右轮速度比（建议1到-1之间）*/
  void turn_right(int16_t speed,float proportion);
  /*右转延时，函数重载，转弯，参数：速度，时间，左右轮速度比（建议1到-1之间）*/
  void right_delay(int16_t speed,uint16_t times,float proportion);
  /*左转，函数重载，转弯，参数：速度，左右轮速度比（建议1到-1之间）*/
  void turn_left(int16_t speed,float proportion);
  /*左转延时，函数重载，转弯，参数：速度，时间，左右轮速度比（建议1到-1之间）*/
  void left_delay(int16_t speed,uint16_t times,float proportion);

  /*停止*/
  void Run_stop();
  /*车反向 参数：速度，时间*/
  void backward_delay(int16_t speed,uint16_t times);
  /*普通巡线，巡白线 参数：速度，补偿值*/
  void Patrol_WhiteStraight(int16_t spd,int32_t compensation);
  /*后退巡线，巡白线 参数：速度，补偿值*/
  void Back_WhiteStraight(int16_t spd,int32_t compensation);
  /*巡黑线*/
  void Patrol_BlackStraight(int16_t spd,int32_t compensation);
  /*距离巡线，参数：速度，距离速度补偿值，增量速度补偿值*/
  void Patrol_Distance(int16_t spd,int32_t Distance, int32_t compensation);
  /*距离巡线1（需要使用陀螺仪定位时使用），光电保护，参数：速度，距离速度补偿值，增量速度补偿值，光电补偿量*/
  void Patrol_Distance_Protect(int16_t spd,int32_t Distance,int32_t compensation,int32_t protect);
  /*距离巡线2，光电保护，参数：速度，距离速度补偿值，增量速度补偿值，光电补偿量*/
  void Patrol_Distance_Protect2(int16_t spd,int32_t Distance,int32_t compensation,int32_t protect);
  /*距离巡线3，参数：速度，距离速度补偿值，增量速度补偿值，光电速度补偿值，光电保护量*/
  void Patrol_Distance_Protect3(int16_t spd,int32_t Distance,int32_t compensation,int32_t protect);
};

#endif

