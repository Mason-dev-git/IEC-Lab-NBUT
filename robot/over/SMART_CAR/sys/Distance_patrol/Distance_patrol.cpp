#include "Distance_patrol.hpp"

Distance_patrol::Distance_patrol(/* args */)
{
}

Distance_patrol::~Distance_patrol()
{
}
/*位置巡线初始化,参数（目标值，积分限幅，积分分离）*/
void Distance_patrol::Init(float targetValue, float IntegralLimitValue, float I_BandValue)
{
    PIDpositional.Init(targetValue,IntegralLimitValue,I_BandValue);
}
/*（函数重载）编码器初始化*/
void Distance_patrol::Init(void)
{
    Control_Encoder.Encoder_Init();
}

/*设置位置巡线参数（kp,ki,kd）*/
void Distance_patrol::Set_Kp_Ki_Kd(float kp, float ki, float kd)
{
    PIDpositional.Set_Kp_Ki_Kd(kp,ki,kd);
}

/*设置电机运动位置参数并使能编码器（目标距离(编码值)，积分限幅，积分分离）*/
void Distance_patrol::Move_Distance(int32_t Distance,float IntegralLimitValue, float I_BandValue)
{
    Init(Distance,IntegralLimitValue,I_BandValue);
    Control_Encoder.Encoder_Init();
}

/*得到加速补偿速度*/
int16_t Distance_patrol::Get_CalculateSpeed(void)//计算速度
{
    if(PIDpositional.Calculate(Control_Encoder.Get_CaptureRightCount())<=5)
    {
        Control_Encoder.Encoder_Disable_Clean();
    }
        return PIDpositional.Calculate(Control_Encoder.Get_CaptureRightCount());
}

/*获得kp*/
float Distance_patrol::Get_kp(void)
{
    return PIDpositional.Get_Kp();
}

/*获得ki*/
float Distance_patrol::Get_ki(void)
{
    return PIDpositional.Get_Ki();
}

/*获得kd*/
float Distance_patrol::Get_kd(void)
{
    return PIDpositional.Get_Kd();
}

/*获得当前左轮编码值*/
int32_t Distance_patrol::Get_CaptureLeftCount(void)
{
    return  Control_Encoder.Get_CaptureLeftCount();
}

/*获得当前右轮编码值*/
int32_t Distance_patrol::Get_CaptureRightCount(void)
{
    return  Control_Encoder.Get_CaptureRightCount();
}

/*行驶固定编码值停止，参数（左轮编码值） 编码器停止时返回1，工作时返回0*/
uint8_t Distance_patrol::Run_Left_Distance(int32_t Encoder_value)
{
  if(Control_Encoder.Get_CaptureLeftCount()>=Encoder_value)
  {
    Control_Motor.Speed(0);
    Control_Motor.Disable(); 
    Control_Encoder.Encoder_Disable_Clean();
    return 1;
  }
	return 0;
}


/*行驶固定编码值停止，参数（右轮编码值） 编码器停止时返回1，工作时返回0*/
uint8_t Distance_patrol::Run_Right_Distance(int32_t Encoder_value)
{
  if(Control_Encoder.Get_CaptureRightCount()>=Encoder_value)
  {
    Control_Motor.Speed(0);
    Control_Motor.Disable(); 
    Control_Encoder.Encoder_Disable_Clean();
    return 1;
  }
	return 0;
}

/*设置最大补偿*/
void Distance_patrol::Set_MaxOutput(float MaxOutputValue)
{
    PIDpositional.Set_MaxOutput(MaxOutputValue);
}
