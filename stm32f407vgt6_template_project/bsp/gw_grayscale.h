/*
 * Copyright (c) 2022 感为智能科技(济南)
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 */

#ifndef INC_GW_GRAYSCALE_SENSOR_H_
#define INC_GW_GRAYSCALE_SENSOR_H_

#include <stdint.h>
#include "bsp.h"


/* 默认地址 */
#define GW_GRAY_ADDR_DEF 0x4C
#define GW_GRAY_PING 0xAA
#define GW_GRAY_PING_OK 0x66
#define GW_GRAY_PING_RSP GW_GRAY_PING_OK

/* 开启开关数据模式 */
#define GW_GRAY_DIGITAL_MODE 0xDD

/* 开启连续读取模拟数据模式 */
#define GW_GRAY_ANALOG_BASE_ 0xB0
#define GW_GRAY_ANALOG_MODE  (GW_GRAY_ANALOG_BASE_ + 0)

/* 传感器归一化寄存器(v3.6及之后的固件) */
#define GW_GRAY_ANALOG_NORMALIZE 0xCF

/* 循环读取单个探头模拟数据 n从1开始到8 */
#define GW_GRAY_ANALOG(n) (GW_GRAY_ANALOG_BASE_ + (n))

/* 黑色滞回比较参数操作 */
#define GW_GRAY_CALIBRATION_BLACK 0xD0
/* 白色滞回比较参数操作 */
#define GW_GRAY_CALIBRATION_WHITE 0xD1

// 设置所需探头的模拟信号(CE: channel enable)
#define GW_GRAY_ANALOG_CHANNEL_ENABLE 0xCE
#define GW_GRAY_ANALOG_CH_EN_1 (0x1 << 0)
#define GW_GRAY_ANALOG_CH_EN_2 (0x1 << 1)
#define GW_GRAY_ANALOG_CH_EN_3 (0x1 << 2)
#define GW_GRAY_ANALOG_CH_EN_4 (0x1 << 3)
#define GW_GRAY_ANALOG_CH_EN_5 (0x1 << 4)
#define GW_GRAY_ANALOG_CH_EN_6 (0x1 << 5)
#define GW_GRAY_ANALOG_CH_EN_7 (0x1 << 6)
#define GW_GRAY_ANALOG_CH_EN_8 (0x1 << 7)
#define GW_GRAY_ANALOG_CH_EN_ALL (0xFF)

/* 读取错误信息 */
#define GW_GRAY_ERROR 0xDE

/* 设备软件重启 */
#define GW_GRAY_REBOOT 0xC0

/* 读取固件版本号 */
#define GW_GRAY_FIRMWARE 0xC1


/**
 * @brief 从I2C得到的8位的数字信号的数据 读取第n位的数据
 * @param sensor_value_8 数字IO的数据
 * @param n 第1位从1开始, n=1 是传感器的第一个探头数据, n=8是最后一个
 * @return
 */
#define GET_NTH_BIT(sensor_value, nth_bit) (((sensor_value) >> ((nth_bit)-1)) & 0x01)


/**
 * @brief 从一个变量分离出所有的bit
 */
#define SEP_ALL_BIT8(sensor_value, val1, val2, val3, val4, val5, val6, val7, val8) \
do {                                                                              \
val1 = GET_NTH_BIT(sensor_value, 1);                                              \
val2 = GET_NTH_BIT(sensor_value, 2);                                              \
val3 = GET_NTH_BIT(sensor_value, 3);                                              \
val4 = GET_NTH_BIT(sensor_value, 4);                                              \
val5 = GET_NTH_BIT(sensor_value, 5);                                              \
val6 = GET_NTH_BIT(sensor_value, 6);                                              \
val7 = GET_NTH_BIT(sensor_value, 7);                                              \
val8 = GET_NTH_BIT(sensor_value, 8);                                              \
} while(0)

/* 设置设备I2C地址 */
#define GW_GRAY_CHANGE_ADDR 0xAD

/* 广播重置地址所需要发的数据 */
#define GW_GRAY_BROADCAST_RESET "\xB8\xD0\xCE\xAA\xBF\xC6\xBC\xBC"

#define Offset 0x88
#if defined (ESP_PLATFORM)
/* ESP32 */


#endif


unsigned char Ping(void);
unsigned char IIC_Get_Digtal(void);
unsigned char IIC_Get_Anolog(unsigned char * Result,unsigned char len);
unsigned char IIC_Get_Single_Anolog(unsigned char Channel);
unsigned char IIC_Anolog_Normalize(uint8_t Normalize_channel);
unsigned short IIC_Get_Offset(void );

#endif /* INC_GW_GRAYSCALE_SENSOR_H_ */

// 例子
#if 0

unsigned char Digtal;
unsigned char Anolog[8]={0};
unsigned char rx_buff[256]={0};
unsigned char Normal[8]={0};


Digtal=IIC_Get_Digtal();
sprintf((char *)rx_buff,"Digtal %d-%d-%d-%d-%d-%d-%d-%d\r\n",(Digtal>>0)&0x01,(Digtal>>1)&0x01,(Digtal>>2)&0x01,(Digtal>>3)&0x01,(Digtal>>4)&0x01,(Digtal>>5)&0x01,(Digtal>>6)&0x01,(Digtal>>7)&0x01);
uart1_send_string((char *)rx_buff);
memset(rx_buff,0,256);

//获取传感器模拟量结果
if(IIC_Get_Anolog(Anolog,8)){
    sprintf((char *)rx_buff,"Anolog %d-%d-%d-%d-%d-%d-%d-%d\r\n",Anolog[0],Anolog[1],Anolog[2],Anolog[3],Anolog[4],Anolog[5],Anolog[6],Anolog[7]);
    uart1_send_string((char *)rx_buff);
    memset(rx_buff,0,256);
}

//获取传感器归一化结果
IIC_Anolog_Normalize(0xff);//所有通道归一化都打开
HAL_Delay(10);//设置完，需要等上一会。stm8的运算速度没stm32快，等一下，让传感器把数据刷新一下。
if(IIC_Get_Anolog(Normal,8)){
    sprintf((char *)rx_buff,"Normalize %d-%d-%d-%d-%d-%d-%d-%d\r\n",Normal[0],Normal[1],Normal[2],Normal[3],Normal[4],Normal[5],Normal[6],Normal[7]);
    uart1_send_string((char *)rx_buff);
    memset(rx_buff,0,256);
}
IIC_Anolog_Normalize(0x00);//为了下一次循环是非归一化，所以清零
HAL_Delay(1000);

#endif


