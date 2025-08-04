#ifndef MYLIB_UTILITY_MATH_H
#define MYLIB_UTILITY_MATH_H

#include "datatype.h"

// base的exp次方
int32_t pow32i(int32_t base, int32_t exp);

uint16_t crc16_modbus_calculate(uint8_t* data, uint16_t len);

// 将第bit位的位置上为1
// 实现思路：将1左移bit位，然后和var做或运算
// 例如：BITS_SET_BIT_1(x, 2);
// 1. 1左移2位，得到0100，前面都是0
// 2. 0100和x做或运算，设置x的第2位为1
#define BITS_SET_BIT_1(var, bit) ((var) |= (1 << (bit))) 

// 将第bit位的位置上为0
// 实现思路：将1左移bit位，然后取反，再和var做与运算
// 例如：BITS_SET_BIT_0(x, 2);
// 1. 1左移2位，得到0100，前面都是0
// 2. 0100取反，得到1011，前面都是1
// 3. 1011和x做与运算，设置x的第2位为0
#define BITS_SET_BIT_0(var, bit) ((var) &= ~(1 << (bit))) 

// 获取变量var的第bit位的值
// 实现思路：将1左移bit位，然后和var做与运算
// 例如：BITS_GET_BIT(x, 2);    
// 1. 1左移2位，得到0100，前面都是0
// 2. 0100和x做与运算，得到x的第2位 
#define BITS_GET_BIT(var, bit) (((var) >> (bit)) & 0x01)

// 一个字是两个字节
// 这个宏的作用是获取低字节
#define BITS_GET_LOW_BIT_IN_WORD(word) ((uint8_t)word)

// 获取高字节
#define BITS_GET_HIGH_BIT_IN_WORD(word) ((uint8_t)(word >> 8)) 





#endif   // !MYLIB_UTILITY_MATH_H
