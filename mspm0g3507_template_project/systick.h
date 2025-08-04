#ifndef MYLIB_DRIVER_SYSTICK_H
#define MYLIB_DRIVER_SYSTICK_H

#    include "bsp.h"
#include <stdint.h>


#if PLATFORM_MSPM0

typedef struct
{
    volatile float    last_time;
    volatile float    current_time;
    volatile float    period;
    volatile uint16_t period_int;   // ??λms
} systime_t;


uint32_t micros(void);
uint32_t millis(void);
void     systick_delay_ms(uint32_t x);
void     delay_us(uint32_t x);


void     get_systime(systime_t* sys);
float    get_systime_ms(void);
uint32_t get_systick_ms(void);

#elif PLATFORM_STM32

// hal库实现


void systick_wait(void);
// 延时us
void systick_delay_us(volatile uint32_t nTime);
// 延时ms
void systick_delay_ms(volatile uint32_t nTime);
// 延时s
void systick_delay_s(volatile uint32_t nTime);
// 设置倒计时(非阻塞式)
void systick_countdown_begin(volatile uint32_t nTime);
// 撤销倒计时
void systick_countdown_cancel(void);
// 判断倒计时是否超时
uint8_t systick_countdown_is_timeout(void);
// 设置系统定时器中断的回调函数
void systick_iqr_handler(void);

//// 系统定时器初始化
// void SysTick_Init(void);

//// 延时
//// 延时 us
// void SysTick_DelayUs(__IO u32 nTime);
//// 延时 ms
// void SysTick_DelayMs(__IO u32 nTime);
//// 延时 s
// void SysTick_DelayS(__IO u32 nTime);

//// 倒计时
//// 设置倒计时(非阻塞式)
// void SysTick_CountdownBegin(__IO u32 nTime);
//// 撤销倒计时
// void SysTick_CountdownCancel(void);
//// 判断倒计时是否超时
// uint8_t SysTick_CountdownIsTimeout(void);


// 标准库
#    if 0
#        include "stm32f10x.h"

/* 初始化系统定时器 */
void systick_init(void);
/* 中断实现的定时，单位为10us */
void systick_delay_us_isr(__IO uint32_t us);
/* 中断实现的定时，单位为1ms */
void systick_delay_ms_isr(__IO uint32_t ms);
/* 查询实现的定时，单位为10us */
void systick_delay_us(__IO uint32_t us);
/* 查询实现的定时，单位为1ms */
void systick_delay_ms(__IO uint32_t ms);
/* 获取节拍函数，只应该在SysTick_Handler中被调用 */
void timing_delay_decrement(void);

#    endif

#endif

#endif   // !MYLIB_DRIVER_SYSTICK_H
