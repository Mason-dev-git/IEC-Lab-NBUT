// 本驱动目前只支持iic驱动jy61p
//
// 使用方法：
//     1. 使用CubeMX初始化一个iic出来
//     2. 调用jy61p_init进行初始化设备
//     3. 调用jy61p_xxx读取数据


#ifndef DEVICE_JY61P_H
#define DEVICE_JY61P_H

#include <mylib_config.h>

#include <stdint.h>

// 如果没有修改，那么默认就是0x50
#define JY61P_DEFAULT_ADDRESS 0x50

// 初始化jy61p
void jy61p_init(void);

// 从jy61p获取数据，调用以后会把数据解析并且计算好，之后存储在内部
void jy61p_get_data(void);

// 从内部获取三维加速度数据，参数为用于存储数据的变量
void jy61p_get_acc(float* ax, float* ay, float* az);

// 从内部获取三维角速度数据，参数为用于存储数据的变量
void jy61p_get_gyro(float* ax, float* ay, float* az);

// 从内部获取三维角度数据，参数为用于存储数据的变量
void jy61p_get_angle(float* ax, float* ay, float* az);


#endif   // !DEVICE_JY61P_H
