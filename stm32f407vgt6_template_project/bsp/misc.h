#ifndef LED_H
#define LED_H

#include "bsp.h"

// 全局变量声明
extern uint8_t led_status_1;
extern uint8_t led_status_2;

// led
void led_on(void);
void led_off(void);
void led_toggle(void);
// 自动关闭的led，times是多少久，以10ms为单位
void led_open_auto_close(u8 times);
// 中断
void led_irq_handler(void);
// 蜂鸣器
void buzzer_on(void);
void buzzer_off(void);
void buzzer_toggle(void);
// 自动关闭的蜂鸣器，times是多少久，以10ms为单位
void buzzer_open_auto_close(u8 times);
// 中断
void buzzer_irq_handler(void);

// 激光笔
void laser_on(void);
void laser_off(void);
// 获取电源电压
f32 get_power_voltage(void);
// 获取角度传感器的数据
f32 get_angle_data(void);

struct key_t{
    s32 keyState;
    s32 judgeState;
    s32 singleFlag;
    s32 longFlag;
    s32 time;
};
// 公开key
extern struct key_t keys[5];

// 按键初始化
void key_init(void);
// 按键扫描
void scan_key(void);

#endif /* LED_H */
