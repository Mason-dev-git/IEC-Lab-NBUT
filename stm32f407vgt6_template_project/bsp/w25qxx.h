#ifndef MYLIB_DEVICE_W25QXX_H
#define MYLIB_DEVICE_W25QXX_H

#include "bsp.h"

// 使用之前需要初始化好SPI和CS引脚，这个函数负责让模块绑定SPI和CS引脚
void w25qxx_init(SPI_HandleTypeDef* hspi, GPIO_TypeDef* hcs_port, u16 hcs_pin);
// 读取设备ID
u16 w25qxx_read_device_id(void);
// 使能写入
void w25qxx_write_enable(void);
// 禁用写入
void w25qxx4_write_disable(void);
// 读取状态寄存器
u8 w25qxx_read_status_register(void);
// 擦除扇区
void w25qxx_sector_erase(u32 address);
// 读取数据
void w25qxx_read_data(u32 address,uint8_t *buf, usize len);
// 扇区写入数据，一次最多写入256字节
void w25qxx_page_program(u32 address,uint8_t *buf, usize len);


#endif   // !MYLIB_DEVICE_W25QXX_H
