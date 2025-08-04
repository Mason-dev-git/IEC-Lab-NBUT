/**
 * @file tofxxf.h
 * @brief tof050f、tof200f、tof200f的驱动，基于直接的串口数据解析
 * 使用方法：
 * 1. 初始化tof
 *   tofxxf_init(&huart6);
 * 2. 配置中断处理函数
 *   在中断处理函数内调用tofxxf_irq_handler();
 * 3.请求数据 
 *   tofxxf_requst();
 * 4.解析获取最新距离数据，如果没有成功，返回-1
 *   int32_t dis = tofxxf_get_distance();
 *
 * @version 0.1
 * 
 */
#ifndef TOFX00F_H
#define TOFX00F_H

#include "bsp.h"

// 串口的初始化要求
// 波特率 115200
// 数据位 8
// 停止位 1
// 校验位 无
// 流控制 无

// 初始化tofxxf
void tofxxf_init(UART_HandleTypeDef* huart);
// 请求获取测量值
void tofxxf_requst(void);
// 获取测量值
int32_t tofxxf_get_distance(void);
// 中断解析数据
void tofxxf_irq_handler(void);

// 以下接口暂未实现
#if 0 

// 扫描tofxxf设备，通过变量返回设备地址，如果成功则address就是地址，且返回true。失败返回false，且address不会写入任何值
bool tofxxf_scan_device(uint8_t* address);
// 恢复默认参数
void tofxxf_reset(void);
// 重启设备
void tofxxf_restart(void);
// 测试通讯
void tofxxf_test(void);
// 设置量程模式，0为默认，1为高精度，2位长距离，3为高速
void tofxxf_set_range_mode(uint8_t mode);

#endif 

#endif   // !TOFX00F_H
