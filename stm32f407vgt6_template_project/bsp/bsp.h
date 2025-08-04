#ifndef BSP_H
#define BSP_H

// 如果使用测试，定义为1，否则为0
#define USE_TEST 0
// 13线编码器电机
#define USE_13_MOTOR 1

// 标准库
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

// hal库
#include "stm32f4xx_hal.h"
// 硬件配置
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

// 辅助
#include "binary.h"
#include "time_util.h"
#include "debug.h"
#include "datatype.h"

// 模块驱动
#include "wt901c.h"
#include "oled.h"
#include "lcd.h"
#include "ADS7828.h"
#include "grayscale.h"
#include "gw_grayscale.h"
#include "motor.h"
#include "Emm_V5.h"
#include "tofx00f.h"
#include "misc.h"
#include "servo.h"
// sfud
#include "sfud.h"
#include "sfud_cfg.h"

#endif // !BSP_H
