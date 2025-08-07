#ifndef __GRAYSCALE_H
#define __GRAYSCALE_H

#include "bsp.h"
#include "i2c.h"

// Grayscale sensor initialization structure
typedef struct GrayscaleInit
{
    uint8_t address;                         // Device address
    uint8_t input_mode;                      // Input mode (e.g., single-ended, differential)
    uint8_t PD_selection;                    // Power-down selection
    uint8_t channel;                         // Channel selection
}GrayscaleInitTypeDef;

/************input mode definitions************/
#define DIFFERENTIAL_MODE           0x00     // Differential mode
#define SINGLE_ENDED_MODE           0x80     // Single-ended mode
/************power-down mode definitions************/
#define PD_MODE_CONV_SLEEP          0x00     // Conversion sleep, ADC and internal reference off
#define PD_MODE_ADC_ON              0x04     // ADC on and internal reference off
#define PD_MODE_REF_ON              0x08     // ADC off and internal reference on
#define PD_MODE_REF_ADC_ON          0x0C     // ADC and internal reference on
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

