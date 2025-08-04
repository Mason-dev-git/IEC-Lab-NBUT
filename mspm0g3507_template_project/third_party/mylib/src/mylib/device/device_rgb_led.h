/******************************************************************************
 * @file    device_rgb_led.h
 * @author  canrad
 * @version v1.0
 * @date    2024-07-28
 * @brief   rgb led驱动
 ******************************************************************************/

#ifndef DEVICE_RGB_LED_H
#define DEVICE_RGB_LED_H

#include <mylib/mylib_config.h>
#include <stdbool.h>

typedef struct
{
    GPIO_Regs* port_r;
    uint32_t   pin_r;
    GPIO_Regs* port_g;
    uint32_t   pin_g;
    GPIO_Regs* port_b;
    uint32_t   pin_b;
    int8_t     valid;   // 标明这个灯是什么时候亮，如果为VALID_ON_HIGH_LEVEL，说明是高电平为有效
} rgb_led_t;

void rgb_led_set(rgb_led_t* led, bool r, bool g, bool b);

#endif   // !DEVICE_RGB_LED_H
