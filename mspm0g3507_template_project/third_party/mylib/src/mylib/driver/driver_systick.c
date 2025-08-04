#include <mylib/driver/driver_systick.h>

#if PLATFORM_MSPM0

#    include "ti_msp_dl_config.h"

volatile uint32_t g_sysTickUptime;

// 返回us
uint32_t micros(void)
{
    uint32_t systick_period = CPUCLK_FREQ / 1000U;
    return g_sysTickUptime * 1000 + (1000 * (systick_period - SysTick->VAL)) / systick_period;
}

// 返回ms
uint32_t millis(void)
{
    return micros() / 1000UL;
}

// 延时us
void delayMicroseconds(uint32_t us)
{
    uint32_t start = micros();
    while ((int32_t)(micros() - start) < us) {
        // Do nothing
    };
}

void delay(uint32_t ms)
{
    delayMicroseconds(ms * 1000UL);
}

void systick_delay_ms(uint32_t x)
{
    delay(x);
}

void delay_us(uint32_t x)
{
    delayMicroseconds(x);
}

void get_systime(systime_t* sys)
{
    sys->last_time    = sys->current_time;
    sys->current_time = micros() / 1000.0f;   //单位ms
    sys->period       = sys->current_time - sys->last_time;
    sys->period_int   = (uint16_t)(sys->period + 0.5f);   //四舍五入
}

float get_systime_ms(void)
{
    return millis();   //单位ms
}

uint32_t get_systick_ms(void)
{
    return (uint32_t)(2 * g_sysTickUptime);   //单位ms
}


#elif PLATFORM_STM32


// 记录系统的时间 计时器
// __IO = volatile
static volatile uint32_t g_systick_counter;


// 等待计时完成
void systick_wait(void)
{
    // 定时器使能
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    // 等待直到计时器变为0
    while (g_systick_counter > 0)
        ;
    // 定时器失能
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

// 延时us
void systick_delay_us(volatile uint32_t nTime)
{
    // 设置时钟中断为us级
    SysTick_Config(SystemCoreClock / 1000000);
    g_systick_counter = nTime;
    // 等待计时完成
    systick_wait();
    // 重新设置系统中断为ms级
    SysTick_Config(SystemCoreClock / 1000);
    // 定时器失能
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

// 延时ms
void systick_delay_ms(volatile uint32_t nTime)
{
    g_systick_counter = nTime;
    systick_wait();
}

// 延时s
void systick_delay_s(volatile uint32_t nTime)
{
    systick_delay_ms(nTime * 1000);
}

// 设置倒计时(非阻塞式)
void systick_countdown_begin(volatile uint32_t nTime)
{
    // 这里设置为1ms中断一次
    g_systick_counter = nTime;
    // 定时器使能
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

// 撤销倒计时
void systick_countdown_cancel(void)
{
    // 重置嘀嗒计时器的计数值
    g_systick_counter = 0;
    // systick 定时器失能
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

// 判断倒计时是否超时
uint8_t systick_countdown_is_timeout(void)
{
    return g_systick_counter == 0;
}

// 设置系统定时器中断的回调函数
void systick_iqr_handler(void)
{
    if (g_systick_counter > 0) { g_systick_counter--; }
    else {
        g_systick_counter = 0;
    }
}

#    if 0
#        include "driver_systick.h"
#        include "core_cm3.h"
#        include "misc.h"
#        include <stdint.h>

static __IO uint32_t TimingDelay;

/**
 * @brief  启动系统滴答定时器 SysTick
 * @param  无
 * @retval 无
 */
void systick_init(void)
{
    /* SystemFrequency / 1000    1ms中断一次
     * SystemFrequency / 100000	 10us中断一次
     * SystemFrequency / 1000000 1us中断一次
     */
    //	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0库版本
    if (SysTick_Config(SystemCoreClock / 100000))   // ST3.5.0库版本
    {
        /* Capture error */
        while (1)
            ;
    }
}

/**
 * @brief   us延时程序,10us为一个单位
 * @param
 *		@arg nTime: Delay_us( 1 ) 则实现的延时为 1 * 10us = 10us
 * @retval  无
 */
void systick_delay_us_isr(__IO uint32_t nTime)
{
    TimingDelay = nTime;

    // 使能滴答定时器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    while (TimingDelay != 0)
        ;
}

/**
 * @brief   ms延时程序,1ms为一个单位
 * @param
 *		@arg nTime: Delay_us( 1 ) 则实现的延时为 1 * 10us = 10us
 * @retval  无
 */
void systick_delay_ms_isr(__IO uint32_t nTime)
{
    TimingDelay = nTime * 100;

    // 使能滴答定时器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    while (TimingDelay != 0)
        ;
}


/**
 * @brief  获取节拍程序
 * @param  无
 * @retval 无
 * @attention  在 SysTick 中断函数 SysTick_Handler()调用
 */
void timing_delay_decrement(void)
{
    if (TimingDelay != 0x00) { TimingDelay--; }
}

#        if 0
// 这个 固件库函数 在 core_cm3.h中
static __INLINE uint32_t SysTick_Config(uint32_t ticks)
{
    // reload 寄存器为24bit，最大值为2^24
    if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);

    // 配置 reload 寄存器的初始值
    SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;

    // 配置中断优先级为 1<<4-1 = 15，优先级为最低
    NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);

    // 配置 counter 计数器的值
    SysTick->VAL   = 0;

    // 配置systick 的时钟为 72M
    // 使能中断
    // 使能systick
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                     SysTick_CTRL_TICKINT_Msk   |
                     SysTick_CTRL_ENABLE_Msk;
    return (0);
}
#        endif

// couter 减1的时间 等于 1/systick_clk
// 当counter 从 reload 的值减小到0的时候，为一个循环，如果开启了中断则执行中断服务程序，
// 同时 CTRL 的 countflag 位会置1
// 这一个循环的时间为 reload * (1/systick_clk)

void systick_delay_us(__IO uint32_t us)
{
    uint32_t i;
    SysTick_Config(SystemCoreClock / 1000000);

    for (i = 0; i < us; i++) {
        // 当计数器的值减小到0的时候，CRTL寄存器的位16会置1
        while (!((SysTick->CTRL) & (1 << 16)))
            ;
    }
    // 关闭SysTick定时器
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void systick_delay_ms(__IO uint32_t ms)
{
    uint32_t i;
    SysTick_Config(SystemCoreClock / 1000);

    for (i = 0; i < ms; i++) {
        // 当计数器的值减小到0的时候，CRTL寄存器的位16会置1
        // 当置1时，读取该位会清0
        while (!((SysTick->CTRL) & (1 << 16)))
            ;
    }
    // 关闭SysTick定时器
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

#    endif

#endif

/*********************************************END OF FILE**********************/
