#include "debug.h"
#include <stdio.h>
#include <stdio.h>    // 包含 vsprintf 和 vprintf
#include <stdarg.h>   // 包含 va_list, va_start, va_end
#include <string.h>   // 包含 strlen

// 定义一个缓冲区大小
#define DEBUG_BUFFER_SIZE 256

static UART_HandleTypeDef* g_huart = NULL;

static void usart_send_string(const char* str)
{
    unsigned int k = 0;
    do {
        HAL_UART_Transmit(g_huart, (uint8_t*)(str + k), 1, HAL_MAX_DELAY);
        k++;
    } while (*(str + k) != '\0');
}

void debug_init(UART_HandleTypeDef* huart)
{
    g_huart = huart;
}

// 平台无关，无需修改
void debug_print(const char* fmt, ...)
{
    va_list args;                     // 定义一个 va_list 类型的变量，用来存放参数
    char buffer[DEBUG_BUFFER_SIZE];   // 定义一个字符数组，用来存放格式化后的字符串

    va_start(args, fmt);   // 初始化 args，fmt 是最后一个固定参数

    // 使用 vsprintf 将参数格式化到 buffer 中
    vsprintf(buffer, fmt, args);

    va_end(args);   // 清理 args

    // 输出格式化后的字符串
    // 这里是发送到串口
    usart_send_string(buffer);
}


void debug_print_easyhex_s32(int32_t* data)
{
    uint8_t buffer[4];
    uint8_t frametail[4] = 0x80000000;//easyhex协议传输int32数据帧尾
    for (uint8_t i = 0; i < 4; i++)
    {
        buffer[i] = (uint8_t)(*data >> (i * 8));
    }
    HAL_UART_Transmit(g_huart, buffer, 4, HAL_MAX_DELAY);
    HAL_UART_Transmit(g_huart, frametail, 4, HAL_MAX_DELAY);
}
