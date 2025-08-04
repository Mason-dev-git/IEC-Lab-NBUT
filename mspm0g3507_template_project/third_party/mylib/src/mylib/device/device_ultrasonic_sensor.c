#include "bsp_ultrasonic_sensor.h"
#include "HC_R04.h"
#include "bsp_SysTick.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include <stdio.h>




void ULTRASONIC_Init(void)
{
    // 定义GPIO的结构体变量
    GPIO_InitTypeDef        GPIO_InitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    // NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(Trig_RCC | Echo_RCC, ENABLE);   // 使能超声波的GPIO对应的时钟
    RCC_APB2PeriphClockCmd(BSP_ULTRASONIC_TIM_RCC, ENABLE);   //配置定时器4为us级定时器


    /* Trig引脚 */
    GPIO_InitStructure.GPIO_Pin   = Trig_Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // 设置引脚输出速度
    GPIO_Init(Trig_Port, &GPIO_InitStructure);          // 初始化引脚配置
    GPIO_ResetBits(Trig_Port, Trig_Pin);                //引脚置低

    /*Echo引脚*/
    GPIO_InitStructure.GPIO_Pin   = Echo_Pin;           // 选择超声波的引脚
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;      // 设置为下拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // 设置引脚输出速度


    //    NVIC_InitStructure.NVIC_IRQChannel                   = TIM4_IRQn;   //选择TIM4中断通道
    //    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;           //抢占优先级为0
    //    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;           //子优先级为1
    //    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;      //使能中断通道
    //    NVIC_Init(&NVIC_InitStructure);


    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //定时器时钟不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //向上计数器
    TIM_TimeBaseInitStructure.TIM_Period        = 6000;                 //自动装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler     = 72 - 1;               //分频系数

    TIM_TimeBaseInit(BSP_ULTRASONIC_TIM, &TIM_TimeBaseInitStructure);
}


float Get_distance()
{
    float TIME_distance;
    Trig = 0;
    Delay_us(10);
    Trig = 1;
    TIM_Cmd(BSP_ULTRASONIC_TIM, ENABLE);   //定时器4使能
    while (!Echo)                          //等待高电平出现
    {
        if (TIM_GetCounter(BSP_ULTRASONIC_TIM) > 40000)   // 200ms内未接收到返回信号
        {
            TIM_Cmd(BSP_ULTRASONIC_TIM, DISABLE);    //定时器4失能
            TIM_SetCounter(BSP_ULTRASONIC_TIM, 0);   //计时器清零
            return ERROR;
        }
    }
    TIM_SetCounter(BSP_ULTRASONIC_TIM, 0);   //接收到高电平，开始计时
    while (Echo)
        ;                                   //等待低电平出现
    TIM_Cmd(BSP_ULTRASONIC_TIM, DISABLE);   //定时器4失能
    TIME_distance = ((float)TIM_GetCounter(BSP_ULTRASONIC_TIM) / 1000.0) * 340 / 2.0;   //计算距离
    TIM_SetCounter(BSP_ULTRASONIC_TIM, 0);   //计时器清零
    return TIME_distance;                    //返回距离，单位mm
}
