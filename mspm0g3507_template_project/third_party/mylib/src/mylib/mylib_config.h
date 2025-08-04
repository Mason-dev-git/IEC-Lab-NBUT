#ifndef MYLIB_CONFIG_H
#define MYLIB_CONFIG_H

// 使用debug需要定义这个宏
#define USE_DEBUG_MODE 1


#ifdef __MSPM0G3507__

// mspm0g3507
#    define PLATFORM_MSPM0 1

#endif   // __MSPM0G3507__

#ifdef STM32F103xB

// stm32f103
#    define PLATFORM_STM32 1



#endif   // !STM32F103xB


#ifdef PLATFORM_MSP

// stm32f401
//#define STM32_VERSION 401

// msp430
//#define MSP_VERSION 430

// msp432
#    define MSP_VERSION 432

#endif   // !PLATFORM_STM32


// stm32使用hal库
#if PLATFORM_STM32
#    include "stm32f1xx_hal.h"
#endif

// mspm0使用下面这个头文件
#if PLATFORM_MSPM0

#    include <ti/devices/msp/msp.h>
#    include <ti/driverlib/driverlib.h>
#    include <ti/driverlib/m0p/dl_core.h>

#endif

// 低电平有效
#define VALID_ON_LOW_LEVEL 0
// 高电平有效
#define VALID_ON_HIGH_LEVEL 1

#endif   // !MYLIB_CONFIG_H
