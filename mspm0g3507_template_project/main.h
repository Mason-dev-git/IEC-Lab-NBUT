#ifndef MAIN_H
#define MAIN_H

#include "ti_msp_dl_config.h"
#include "bsp.h"

#define PID1_KP 30
#define PID1_KI 0.5
#define PID1_KD 0
#define PID1_MAX_OUT 3200
#define PID1_MAX_IOUT 20000

#define PID2_KP 30
#define PID2_KI 0.5
#define PID2_KD 0
#define PID2_MAX_OUT 3200
#define PID2_MAX_IOUT 20000

#define PIDA_KP 1
#define PIDA_KI 0
#define PIDA_KD 0
#define PIDA_MAX_OUT 3200
#define PIDA_MAX_IOUT 20000

// 根据编码器的值计算出速度，单位m/s
inline static float cal_encode_to_speed(int32_t encode)
{
    // 计算方法：https://blog.csdn.net/u011895157/article/details/139609274
    // 速度=(周期内编码器计数的脉冲数)*0.048*3.14/周期
    return (encode*0.048f*3.14f)/(260*0.050);
}

// 根据速度计算编码器
inline static float cal_speed_to_encode(int32_t speed)
{
    // 计算方法：https://blog.csdn.net/u011895157/article/details/139609274
    // 速度=(周期内编码器计数的脉冲数)*0.048*3.14/周期
    return (speed*0.050f*260)/(3.14*0.048);
}

// 50ms 3200占空比 368 356
// 50ms 1600占空比 180 170
// 中间值 3200占空比是362编码器，1600占空比是175编码器

// 根据编码器计算出所需的占空比
inline static int32_t cal_encode_to_duty(int32_t encode)
{
    return encode*(3200-1600)/(362-175);
}

// 根据占空比计算编码器
inline static int32_t cal_duty_to_encode(int32_t duty)
{
    return duty*(362-175)/(3200-1600);
}




static void delay_ms(uint16_t ms)
{
	while(ms--)
		delay_cycles(CPUCLK_FREQ/1000);
}

#endif // !MAIN_H