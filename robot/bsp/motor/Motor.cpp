/*
 * 电机驱动控制程序
 GTM+8 2022-03-18 14:18:49 GTM+8
 */
#include "Motor.hpp"

Motor::Motor()
{

}

Motor::~Motor()
{

}

/**
 * @description: //motor初始化
 * @param {*}
 * @return {*}
 */
void Motor::Init(void)
{
  LeftMotor.Init();
  RightMotor.Init();
}
/**
 * @description: 设置速度限幅
 * @param {*}
 * @return {*}
 */
void Motor::Set_limit_spd(uint16_t Set_limit_spd)
{
  LeftMotor.Set_limit_spd(Set_limit_spd);
  RightMotor.Set_limit_spd(Set_limit_spd);
}
/**
 * @description: motor使能
 * @param {*}
 * @return {*}
 */
void Motor::Enable(void)
{
  LeftMotor.Enable();
  RightMotor.Enable();
}

/**
 * @description: motor失能
 * @param {*}
 * @return {*}
 */
void Motor::Disable(void)
{
  LeftMotor.Disable();
  RightMotor.Disable();
}

/**
 * @description: 一个重载函数，参数；1为使能，0为失能
 * @param {u8} enORdis
 * @return {*}
 */
void Motor::Enable(uint8_t enORdis)
{
  LeftMotor.Enable(enORdis);
  RightMotor.Enable(enORdis);
}

/**
 * @description: 电机控制状态
 * @param {_Bsp_motor_Status} status  :\
 *        Reverse =-1,//电机反转  \
 *        Brake   = 0,//电机制动  \
 *        Forward = 1,//电机正转
 * @return {*}
 */
void Motor::Control(Left_motor_Status Lstatus,Right_motor_Status Rstatus)
{
  LeftMotor.Control(Lstatus);
  RightMotor.Control(Rstatus);
}

/**
 * @description: 双电机速度控制
 * @param {u8} two_speed，参数范围：-1000~1000
 * @return {*}
 */
void Motor::Speed(int16_t two_speed)
{
   LeftMotor.Speed(two_speed);
   RightMotor.Speed(two_speed); 
}
/*函数重载 Left_speed控制左轮，Right_speed控制右轮*/
void Motor::Speed(int16_t Left_speed,int16_t Right_speed)
{
    LeftMotor.Speed(Left_speed);
    RightMotor.Speed(Right_speed); 
}
/*获取左轮实时速度*/
int16_t Motor::Get_Left_Speed(void)
{
   return LeftMotor.Get_speed();
}
/*获取右轮实时速度*/
int16_t Motor::Get_Right_Speed(void)
{
   return RightMotor.Get_speed();
}


/*直线前进*/
void Motor::Run_straight(int16_t speed)
{
  LeftMotor.Speed(speed);
  RightMotor.Speed(speed);
}
/*停止一段时间*/
void Motor::Run_stoptime(uint16_t time)
{
  LeftMotor.Speed(0);
  RightMotor.Speed(0);
  HAL_Delay(time);
}
/*后退*/
void Motor::Run_back(int16_t speed)
{
  LeftMotor.Speed(-speed);
  RightMotor.Speed(-speed);
}
/*右转*/
void Motor::turn_right(int16_t speed)
{
  LeftMotor.Speed(speed);
  RightMotor.Speed(-speed);
}
/*右转延时 参数：速度，时间*/
void Motor::right_delay(int16_t speed,uint16_t times)
{
  LeftMotor.Speed(speed);
  RightMotor.Speed(-speed);	
	HAL_Delay(times);	
}
/*左转*/
void Motor::turn_left(int16_t speed)
{
  LeftMotor.Speed(-speed-0*speed);
  RightMotor.Speed(speed-0*speed);
}
/*左转延时 参数：速度，时间*/
void Motor::left_delay(int16_t speed,uint16_t times)
{
  LeftMotor.Speed(-speed);
  RightMotor.Speed(speed);	
	HAL_Delay(times);	
}


/*右转，函数重载，转弯，参数：速度，左右轮速度比（建议1到-1之间）*/
void Motor::turn_right(int16_t speed,float proportion)
{
  LeftMotor.Speed(speed);
  RightMotor.Speed(proportion * speed);
}
/*右转延时，函数重载，转弯，参数：速度，时间，左右轮速度比（建议1到-1之间）*/
void Motor::right_delay(int16_t speed,uint16_t times,float proportion)
{
  LeftMotor.Speed(speed);
  RightMotor.Speed(proportion * speed);	
	HAL_Delay(times);	
}
/*左转，函数重载，转弯，参数：速度，左右轮速度比（建议1到-1之间）*/
void Motor::turn_left(int16_t speed,float proportion)
{
  LeftMotor.Speed(proportion * speed);
  RightMotor.Speed(speed);
}
/*左转延时，函数重载，转弯，参数：速度，时间，左右轮速度比（建议1到-1之间）*/
void Motor::left_delay(int16_t speed,uint16_t times,float proportion)
{
  LeftMotor.Speed(proportion * speed);
  RightMotor.Speed(speed);	
	HAL_Delay(times);	
}


/*停止*/
void Motor::Run_stop()
{
  LeftMotor.Speed(0);
  RightMotor.Speed(0);
}
/*车反向 参数：速度，时间*/
void Motor::backward_delay(int16_t speed,uint16_t times)
{
  right_delay(-speed,times);
  left_delay(speed,times);
}

/*普通巡线，巡白线 参数：速度，补偿值*/
void Motor::Patrol_WhiteStraight(int16_t spd,int32_t compensation)
{
  Speed(spd - compensation, spd + compensation);
}

/*后退巡线，巡白线 参数：速度，补偿值*/
void Motor::Back_WhiteStraight(int16_t spd,int32_t compensation)
{
  Speed(-spd - compensation, -spd + compensation);
}

/*巡黑线*/
void Motor::Patrol_BlackStraight(int16_t spd,int32_t compensation)
{
  Speed(spd - compensation, spd + compensation);
}

/*距离巡线，参数：速度，距离速度补偿值，增量速度补偿值*/
void Motor::Patrol_Distance(int16_t spd,int32_t Distance, int32_t compensation)
{
  Speed(spd + Distance + compensation, spd + Distance - compensation);
	// Speed(spd + Distance - compensation,spd + Distance + compensation);
}

/*距离巡线1（需要使用陀螺仪定位时使用），光电保护，参数：速度，距离速度补偿值，增量速度补偿值，光电补偿量*/
void Motor::Patrol_Distance_Protect(int16_t spd,int32_t Distance, int32_t compensation,int32_t protect)
{
  Speed(spd + Distance + compensation - protect, spd + Distance - compensation + protect);
	// Speed(spd + Distance - compensation - protect,spd + Distance + compensation + protect);
}
/*距离巡线2，光电保护，参数：速度，距离速度补偿值，增量速度补偿值，光电补偿量*/
void Motor::Patrol_Distance_Protect2(int16_t spd,int32_t Distance, int32_t compensation,int32_t protect)
{
  Speed(spd - Distance + compensation - protect, spd - Distance - compensation + protect);//巡逻距离保护
	// Speed(spd + Distance + compensation - protect,spd + Distance - compensation + protect);
}


/*距离巡线3，参数：速度，距离速度补偿值，增量速度补偿值，光电速度补偿值，光电保护量*/
void Motor::Patrol_Distance_Protect3(int16_t spd,int32_t Distance,int32_t compensation,int32_t protect)
{
  Speed(spd - Distance - compensation + protect , spd - Distance + compensation - protect );
}


