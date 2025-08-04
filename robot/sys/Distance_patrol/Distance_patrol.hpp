#ifndef __DISTANCE_PATROL_HPP
#define __DISTANCE_PATROL_HPP
#include "main.h"
#include "Encoder.hpp"
#include "pid_positional.hpp"
#include "Motor.hpp"

class Distance_patrol
{
private:
    PID_POSITIONAL  PIDpositional;   //位置式PID控制器 
    Encoder         Control_Encoder; //控制编码器出值
    Motor           Control_Motor;   //控制电机速度
public:
    Distance_patrol();
    ~Distance_patrol();

    /*位置巡线初始化,参数（目标值，积分限幅，积分分离）*/
    void Init(float targetValue, float IntegralLimitValue,float I_BandValue);
    /*（函数重载）编码器初始化*/
    void Init(void);
    /*设置位置巡线参数（kp,ki,kd）*/
    void Set_Kp_Ki_Kd(float kp, float ki, float kd);
    /*设置电机运动位置参数并使能编码器（目标距离(编码值)，积分限幅，积分分离）*/
    void Move_Distance(int32_t Distance,float IntegralLimitValue, float I_BandValue);
    /*得到加速补偿速度*/
    int16_t Get_CalculateSpeed(void);
    /*获得kp*/
    float Get_kp(void);
    /*获得ki*/
    float Get_ki(void);
    /*获得kd*/
    float Get_kd(void);
    /*获得当前左轮编码值*/
    int32_t Get_CaptureLeftCount(void);
    /*获得当前右轮编码值*/
    int32_t Get_CaptureRightCount(void);
    /*行驶固定编码值停止，参数（左轮编码值） 编码器停止时返回1，工作时返回0*/
    uint8_t Run_Left_Distance(int32_t Encoder_value);
    /*行驶固定编码值停止，参数（右轮编码值） 编码器停止时返回1，工作时返回0*/
    uint8_t Run_Right_Distance(int32_t Encoder_value);
    /*设置最大补偿*/
    void Set_MaxOutput(float MaxOutputValue);


};

#endif
