#include "HC_R04.h"
#include "bsp_SysTick.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include <stdio.h>


uint32_t pulse_width_us = 0;
uint32_t distance_cm    = 0;

void HCSR04_Init(void)
{
    // 定义GPIO的结构体变量
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);   // 使能超声波的GPIO对应的时钟

    /* Trig引脚 */
    GPIO_InitStructure.GPIO_Pin   = HC_R04_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // 引脚输出速度设置为快

    // 初始化引脚配置
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /*Echo引脚*/
    GPIO_InitStructure.GPIO_Pin   = HC_R04_PIN;              // 选择超声波的引脚
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;   // 设置为双边沿触发外部中断
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        // 引脚输出速度设置为快

    // 初始化引脚配置
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    NVIC_InitStructure.NVIC_IRQChannel                   = TIM4_IRQn;   //选择TIM4中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;           //抢占优先级为0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;           //子优先级为1
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;      //使能中断通道
    NVIC_Init(&NVIC_InitStructure);

    /*配置定时器4为us级定时器*/
    RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;         //时钟分频因子
    TIM_TimeBaseInitStructure.TIM_CounterMode       = TIM_CounterMode_Up;   //向上计数器
    TIM_TimeBaseInitStructure.TIM_Period            = 0xFFFF;               //自动装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler         = 83;                   //分频系数
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
}

//外部中断处理函数
void EXTI9_5_IRQHandler(void)
{
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);
}

void SR04_Trigger(void)
{
    // 发送10us的高电平脉冲
    Trig_ON;
    Delay_us(10);
    Trig_OFF;
}

void GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static int count = 0;
    if (GPIO_Pin == HC_R04_PIN) {
        if (GPIO_ReadInputDataBit(GPIOC, HC_R04_PIN)) {
            TIM_Cmd(TIM4, ENABLE);     // 启动定时器
            TIM_SetCounter(TIM4, 0);   //清空定时器的值
        }
        else if (GPIO_ReadInputDataBit(GPIOC, HC_R04_PIN) == 0) {
            TIM_Cmd(TIM4, DISABLE);               // 停止定时器
            count       = TIM_GetCounter(TIM4);   //获取当前计数值
            distance_cm = count * 340 / 2 * 0.000001 * 100;
            printf("distance_cm is %d\r\n", distance_cm);
            count = 0;
        }
    }
}

/*返回距离*/
uint32_t Get_Distance(void)
{
    return distance_cm;
}
