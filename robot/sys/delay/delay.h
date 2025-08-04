#ifndef _DELAY_H
#define _DELAY_H
#include <sys.h>	  
//////////////////////////////////////////////////////////////////////////////////  
//本程序仅用于学习使用，未经授权，不得用于商业用途;
//ALIENTEK STM32F429开发板
//使用SysTick定时器的普通模式实现延时函数(支持ucosii)
//包含delay_us,delay_ms函数
//作者@ALIENTEK
//论坛:www.openedv.com
//创建日期:2015/6/10
//版本:V1.0
//版权所有，盗版必究
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//********************************************************************************
//修改说明
////////////////////////////////////////////////////////////////////////////////// 

void delay_init(uint8_t SYSCLK);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);
#endif
