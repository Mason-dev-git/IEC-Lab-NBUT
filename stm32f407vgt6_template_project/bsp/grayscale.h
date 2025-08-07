#ifndef __GRAYSCALE_H
#define __GRAYSCALE_H

#include "bsp.h"
#include "i2c.h"

/**
 * @brief Grayscale 初始化结构体
 *         
 *        address 从机地址（具体地址由A1、A0引脚确定，详见ADS7828数据手册）
 *        input_mode 输入模式（单端输入/差分输入）
 *        PD_selection 电源模式选择
 *        channel 通道选择
 */
typedef struct GrayscaleInit
{
    uint8_t address;                         
    uint8_t input_mode;                      
    uint8_t PD_selection;                    
    uint8_t channel;                        
}GrayscaleInitTypeDef;

#define DEFAULT_SLAVE_ADDR          0x48     //默认从机地址
/************input mode definitions************/
#define DIFFERENTIAL_MODE           0x00     // 差分输入模式
#define SINGLE_ENDED_MODE           0x80     // 单端输入模式
/************power-down mode definitions************/
#define PD_MODE_CONV_SLEEP          0x00     // 休眠模式，ADC、内部参考电压关闭
#define PD_MODE_ADC_ON              0x04     // ADC开启、内部参考电压关闭
#define PD_MODE_REF_ON              0x08     // ADC关闭、内部参考电压开启
#define PD_MODE_REF_ADC_ON          0x0C     // ADC、内部参考电压开启
/************channel selection definitions************/
#define SINGLE_CH0                  0x00     // Single channel 0
#define SINGLE_CH1                  0x40     // Single channel 1
#define SINGLE_CH2                  0x10     // Single channel 2        
#define SINGLE_CH3                  0x50     // Single channel 3
#define SINGLE_CH4                  0x20     // Single channel 4
#define SINGLE_CH5                  0x60     // Single channel 5
#define SINGLE_CH6                  0x30     // Single channel 6
#define SINGLE_CH7                  0x70     // Single channel 7
#define DIFFER_CH0_1                0x00     // Differential channel 0+/1-
#define DIFFER_CH2_3                0x10     // Differential channel 2+/3-
#define DIFFER_CH4_5                0x20     // Differential channel 4+/5-
#define DIFFER_CH6_7                0x30     // Differential channel 6+/7-
#define DIFFER_CH1_0                0x40     // Differential channel 0-/1+
#define DIFFER_CH3_2                0x50     // Differential channel 2-/3+
#define DIFFER_CH5_4                0x60     // Differential channel 4-/5+
#define DIFFER_CH7_6                0x70     // Differential channel 6-/7+

void Grayscale_Init(void);
void Set_GrayscaleAddress(uint8_t address);
void Set_GrayscaleInputMode(uint8_t input_mode);
void Set_GrayscalePDSelection(uint8_t PD_selection);
void Set_GrayscaleChannel(uint8_t channel);

void Grayscale_SendCommand(void);
uint16_t Grayscale_ReadData(void);

uint16_t Grayscale_ReadChannel(uint16_t channel);
void Grayscale_ReadAllChannels(uint16_t *data_list);

void test_grayscale(void);

#endif

