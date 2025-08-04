//
// 用于调试时，串口打印有问题的代码
// 解决重定向fputc后，直接调用printf导致这些调试代码不容易删除的问题
// 使用方法：1. 初始化串口相关内容，（这通常是使用HAL库或者是手动调用标准固件库实现）
//           2. 调用debug_init来初始化调试模块，（这用于确定信息输出到哪个串口）
 //           3. 把DEBUG_INFO宏当做printf来打印信息，\
               （注意，这需要定义USE_DEBUG_MODE宏，这样的目的是，在非DEBUG模式下，可以把相关调试代码替换为空）
//
//

#ifndef MYLIB_BASE_DEBUG_H
#define MYLIB_BASE_DEBUG_H

#define PLATFORM_STM32 1

#define USE_DEBUG_MODE 1

#include "bsp.h"

#if PLATFORM_STM32
#    ifdef USE_DEBUG_MODE
#        define DEBUG_INFO(fmt, ...) \
            debug_print("[info][%s][%d]:" fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#    else
#        define DEBUG_INFO(fmt, ...)
#    endif   // USE_DEBUG_MODE
#endif

void debug_init(UART_HandleTypeDef* huart);

void debug_print(const char* fmt, ...);

void debug_print_easyhex_s32(int32_t* data);

#endif   // !MYLIB_BASE_DEBUG_H
