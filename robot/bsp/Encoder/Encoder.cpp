#include "Encoder.hpp"

int16_t Encoder_Left_Overflow_Count=0;		 //编码器捕获值溢出位
int16_t Encoder_Right_Overflow_Count=0;		 //编码器捕获值溢出位
Encoder::Encoder()
{
    /*左轮初始化*/
    Motor_Left_Direction=0;
    Capture_Left_Count=0;
    Last_Left_Count=0;
    Encoder_Left_Overflow_Count=0;

    /*右轮初始化*/
    Motor_Right_Direction=0;
    Capture_Right_Count=0;
    Last_Right_Count=0;
    Encoder_Right_Overflow_Count=0;
	
}

Encoder::~Encoder()
{
}

/*编码器初始化并清除数据*/
void Encoder::Encoder_Init(void)
{
    /*左轮清除数据*/
    Motor_Left_Direction=0;
    Capture_Left_Count=0;
    Last_Left_Count=0;
    Encoder_Left_Overflow_Count=0;
    TIM3->CNT =0;  //清除定时器3中存放的值

    /*右轮清除数据*/
    Motor_Right_Direction=0;
    Capture_Right_Count=0;
    Last_Right_Count=0;
    Encoder_Right_Overflow_Count=0;
    TIM4->CNT =0;  //清除定时器4中存放的值

     /* 清零中断标志位 */
    __HAL_TIM_CLEAR_IT(&htim3,TIM_IT_UPDATE);
    /* 使能定时器的更新事件中断 */
    __HAL_TIM_ENABLE_IT(&htim3,TIM_IT_UPDATE);
    /* 使能编码器接口 */
    HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);

    /* 清零中断标志位 */
    __HAL_TIM_CLEAR_IT(&htim4,TIM_IT_UPDATE);
    /* 使能定时器的更新事件中断 */
    __HAL_TIM_ENABLE_IT(&htim4,TIM_IT_UPDATE);
    /* 使能编码器接口 */
    HAL_TIM_Encoder_Start_IT(&htim4, TIM_CHANNEL_ALL);
}
/*编码器停止*/
void Encoder::Encoder_Disable_Clean(void)
{
    HAL_TIM_Encoder_Stop(&htim3, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Stop(&htim4, TIM_CHANNEL_ALL);
}
/*编码器只清除数据*/
void Encoder::Encoder_Clean(void)
{
    /*左轮清除数据*/
    Motor_Left_Direction=0;
    Capture_Left_Count=0;
    Last_Left_Count=0;
    Encoder_Left_Overflow_Count=0;
    TIM3->CNT =0;  //清除定时器3中存放的值

    /*右轮清除数据*/
    Motor_Right_Direction=0;
    Capture_Right_Count=0;
    Last_Right_Count=0;
    Encoder_Right_Overflow_Count=0;
    TIM4->CNT =0;  //清除定时器4中存放的值
}

/*左电机转向状态(函数返回1反转)还是向上计数(函数返回0正转)*/
char Encoder::Get_MotorLeftDirection(void)
{
    Motor_Left_Direction=__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3);
    return  Motor_Left_Direction;
}

/*右电机转向状态(函数返回1正转)还是向上计数(函数返回0反转)*/
char Encoder::Get_MotorRightDirection(void)
{
    Motor_Right_Direction=__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4);
    return  Motor_Right_Direction;
}
/*获取左轴捕获值*/
int32_t Encoder::Get_CaptureLeftCount(void)
{
    Capture_Left_Count=(__HAL_TIM_GetCounter(&htim3)+(Encoder_Left_Overflow_Count*65535));
    return  Capture_Left_Count;
}
/*获取右轴捕获值*/
int32_t Encoder::Get_CaptureRightCount(void)
{
    Capture_Right_Count=(__HAL_TIM_GetCounter(&htim4)+(Encoder_Right_Overflow_Count*65535));
    return  Capture_Right_Count;
}

/*获取左转轴速度（转/秒）*/
float Encoder::Get_ShaftLeftSpeed(void)
{
    Get_CaptureLeftCount();
    Shaft_Left_Speed=((float)(Capture_Left_Count-Last_Left_Count)/ENCODER_READ_RESOLUTION*Time_Value);
    Last_Left_Count=Capture_Left_Count;
    return  Shaft_Left_Speed;
}

/*获取右转轴速度（转/秒）*/
float Encoder::Get_ShaftRightSpeed(void)
{
    Get_CaptureRightCount();
    Shaft_Right_Speed=((float)(Capture_Right_Count-Last_Right_Count)/ENCODER_READ_RESOLUTION*Time_Value);
    Last_Right_Count=Capture_Right_Count;
    return  Shaft_Right_Speed;
}

/*获取左输出轴速度（转/秒）*/
float Encoder::Get_OutputShaftLeftSpeed(void)
{
    float Output_Shaft_Left_Speed;
    Output_Shaft_Left_Speed=(Get_ShaftLeftSpeed()/REDUCTION_RATIO);
    return  Output_Shaft_Left_Speed;
}

/*获取右输出轴速度（转/秒）*/
float Encoder::Get_OutputShaftRightSpeed(void)
{
    float Output_Shaft_Right_Speed;
    Output_Shaft_Right_Speed=(Get_ShaftRightSpeed()/REDUCTION_RATIO);
    return  Output_Shaft_Right_Speed;
}

