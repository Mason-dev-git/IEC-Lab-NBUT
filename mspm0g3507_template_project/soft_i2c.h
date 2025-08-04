/******************************************************************************
 * @file    driver_soft_i2c.h
 * @author  canrad
 * @version v1.1
 * @date    2024-05-12
 * @brief   软件iic驱动
 ******************************************************************************/

#ifndef MTLIB_DRIVER_SOFT_I2C_H
#define MTLIB_DRIVER_SOFT_I2C_H

#    include "bsp.h"
#include <stdint.h>

// 写控制bit
#define SOFT_I2C_WR 0
// 读控制bit
#define SOFT_I2C_RD 1



#if PLATFORM_STM32

#    ifdef __STM32F10x_H
// 标准固件库
typedef struct
{
    GPIO_TypeDef* scl_gpio;
    GPIO_TypeDef* sda_gpio;
    uint32_t      scl_rcc;
    uint32_t      sda_rcc;
    uint16_t      scl_pin;
    uint16_t      sda_pin;
} soft_i2c_t;
#    else
// HAL库
typedef struct
{
    GPIO_TypeDef* scl_gpio;
    GPIO_TypeDef* sda_gpio;
    uint16_t      scl_pin;
    uint16_t      sda_pin;
} soft_i2c_t;
#    endif
#elif PLATFORM_MSPM0
typedef struct
{
    GPIO_Regs* scl_gpio;
    GPIO_Regs* sda_gpio;
    uint32_t   scl_pin;
    uint32_t   sda_pin;
    int32_t    sda_iomux;   // IOMUX_PINCM1
} soft_i2c_t;
#endif

void soft_i2c_init(soft_i2c_t* soft_i2c);

// 在访问I2C设备前，请先调用 soft_i2c_check_device()
// 检测I2C设备是否正常，该函数会自动调用配置GPIO的函数
uint8_t soft_i2c_check_device(soft_i2c_t* soft_i2c, uint8_t address);

// 发送一个字节
void soft_i2c_send_byte(soft_i2c_t* soft_i2c, uint8_t byte);

// 读取一个字节
uint8_t soft_i2c_read_byte(soft_i2c_t* soft_i2c, uint8_t ack);

// 发送一个起始信号
void soft_i2c_start(soft_i2c_t* soft_i2c);

// 发送一个停止信号
void soft_i2c_stop(soft_i2c_t* soft_i2c);

// 等待ACK
uint8_t soft_i2c_wait_ack(soft_i2c_t* soft_i2c);

// 发送ACK
void soft_i2c_ack(soft_i2c_t* soft_i2c);

// 发送NACK
void soft_i2c_nack(soft_i2c_t* soft_i2c);

#endif   // !MTLIB_DRIVER_SOFT_I2C_H
