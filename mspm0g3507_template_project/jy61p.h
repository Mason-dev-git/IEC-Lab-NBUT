// 本驱动目前只支持iic驱动jy61p
//
// 使用方法：
//     1. 使用CubeMX初始化一个串口出来
//     2. 调用jy61p_init进行初始化设备
//     3. 调用jy61p_xxx读取数据

// 配置串口3
// 填写9600波特率、8位帧长、无校验位、1位停止位
// 高级设置里面的通行方向选择RX only
// PinMux选择UART3外设，RX引脚选择PA13

// 使用方法
#if 0

    jy61p_get_acc(&g_acc_x, &g_acc_y, &g_acc_z);
    jy61p_get_gyro(&g_gyro_x, &g_gyro_y, &g_gyro_z);
    jy61p_get_angle(&g_angle_x, &g_angle_y, &g_angle_z);
    
    DEBUG_INFO("g_acc_x=%.3f, g_acc_y=%.3f, g_acc_z=%.3f", g_acc_x, g_acc_y, g_acc_z);
    DEBUG_INFO("g_gyro_x=%.3f, g_gyro_y=%.3f, g_gyro_z=%.3f", g_gyro_x, g_gyro_y, g_gyro_z);
    DEBUG_INFO("g_angle_x=%.3f, g_angle_y=%.3f, g_angle_z=%.3f", g_angle_x, g_angle_y, g_angle_z);

#endif

#ifndef MYLIB_DEVICE_JY61P_H
#    define MYLIB_DEVICE_JY61P_H

#    include "bsp.h"

#    include <stdint.h>

// 如果没有修改，那么默认就是0x50
#    define JY61P_DEFAULT_ADDRESS 0x50

// 初始化jy61p
void jy61p_init(UART_Regs* huart);

// 从jy61p获取数据，调用以后会把数据解析并且计算好，之后存储在内部
void jy61p_get_data(void);

// 从内部获取三维加速度数据，参数为用于存储数据的变量
void jy61p_get_acc(float* ax, float* ay, float* az);

// 从内部获取三维角速度数据，参数为用于存储数据的变量
void jy61p_get_gyro(float* gx, float* gy, float* gz);

// 从内部获取三维角度数据，参数为用于存储数据的变量
void jy61p_get_angle(float* ax, float* ay, float* az);

// 中断函数，需要在串口中断里调用
void jy61p_irq_handler(void);

// 将偏航角Yaw置零
void jy61p_clear_yaw(void);

#endif   // !MYLIB_DEVICE_JY61P_H
