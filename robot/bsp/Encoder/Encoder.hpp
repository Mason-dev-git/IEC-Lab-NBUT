/*
 * 编码器控制函数
 * ps:尽量不用右轮的编码器
 * 因左右轮正反转相反,所以编写编码器二者也要相反,或者硬件连接AB相相反
 * GTM+8 2022-03-29 14:07:12 GTM+8
 */
#ifndef __ENCODER_HPP
#define __ENCODER_HPP
#include "main.h"
#include "tim.h"
#include "test.h"

/*Time_Value的值为（1/定时器中断时长（单位（秒）））*/
#define	Time_Value					5

extern int16_t Encoder_Left_Overflow_Count;		 //编码器捕获值溢出位
extern int16_t Encoder_Right_Overflow_Count;	 //编码器捕获值溢出位

class Encoder
{
private:
   /*左电机转向状态*/
    volatile char Motor_Left_Direction;		
    /*（必须是有符号型)左轴捕获值*/
    volatile int32_t Capture_Left_Count;	
    /*（必须是有符号型)上一次左轴捕获值*/ 
    volatile int32_t Last_Left_Count;	
    /*左轴转速*/		 
    volatile float Shaft_Left_Speed;				
   /*右电机转向状态*/
    volatile char Motor_Right_Direction;	
    /*（必须是有符号型)右轴捕获值*/
    volatile int32_t Capture_Right_Count;		 
    /*（必须是有符号型)上一次右轴捕获值*/
    volatile int32_t Last_Right_Count;
    /*右轴转速*/    
    volatile float Shaft_Right_Speed;
	
public:
    Encoder();
    ~Encoder();
    /*编码器初始化并清除数据*/
    void Encoder_Init(void);
    /*编码器停止*/
    void Encoder_Disable_Clean(void);
    /*编码器只清除数据*/
    void Encoder_Clean(void);
    /*左电机转向状态(函数返回1反转)还是向上计数(函数返回0正转)*/
    char Get_MotorLeftDirection(void);
    /*右电机转向状态(函数返回1正转)还是向上计数(函数返回0反转)*/
    char Get_MotorRightDirection(void);
    /*获取左轴捕获值*/
    int32_t Get_CaptureLeftCount(void);
    /*获取右轴捕获值*/
    int32_t Get_CaptureRightCount(void);
    /*获取左转轴速度（转/秒）*/
    float Get_ShaftLeftSpeed(void);
    /*获取右转轴速度（转/秒）*/
    float Get_ShaftRightSpeed(void);
    /*获取左输出轴速度（转/秒）*/
    float Get_OutputShaftLeftSpeed(void);
    /*获取右输出轴速度（转/秒）*/
    float Get_OutputShaftRightSpeed(void);

};

#endif

