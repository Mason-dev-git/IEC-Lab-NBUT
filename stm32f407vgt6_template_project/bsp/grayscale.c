#include "grayscale.h"

GrayscaleInitTypeDef Grayscale_InitStructure;

void Grayscale_Init(void)
{
    Grayscale_InitStructure.address = 0x48;                  // Device address
    Grayscale_InitStructure.input_mode = SINGLE_ENDED_MODE;  // Default input mode
    Grayscale_InitStructure.PD_selection = PD_MODE_ADC_ON;   // Default power-down selection
    Grayscale_InitStructure.channel = SINGLE_CH0;            // Default channel selection
}

// Set the device address
void Set_GrayscaleAddress(uint8_t address)
{
    Grayscale_InitStructure.address = address;               
}

// Set the input mode
void Set_GrayscaleInputMode(uint8_t input_mode)
{
    Grayscale_InitStructure.input_mode = input_mode;         
}

 // Set the power-down selection
void Set_GrayscalePDSelection(uint8_t PD_selection)
{
    Grayscale_InitStructure.PD_selection = PD_selection;    
}

void Set_GrayscaleChannel(uint8_t channel)
{
    Grayscale_InitStructure.channel = channel;               
}

//写命令，(内部使用,用户无需更改)
void Grayscale_SendCommand(void)
{
    uint8_t command = Grayscale_InitStructure.input_mode | 
                      Grayscale_InitStructure.PD_selection | 
                      Grayscale_InitStructure.channel;
    HAL_I2C_Master_Transmit(&hi2c3, (Grayscale_InitStructure.address<<1), &command, 1, HAL_MAX_DELAY);
}

//读数据，(内部使用,用户无需更改)
uint16_t Grayscale_ReadData(void)
{
    uint8_t data[2] = {0}; 
    Grayscale_SendCommand();
    HAL_I2C_Master_Receive(&hi2c3, (Grayscale_InitStructure.address<<1), data, 2, HAL_MAX_DELAY);
     return (data[1] << 8) | data[0];
}

//通道转换，(内部使用,用户无需更改)
uint8_t channel_switch(uint16_t channel)
{
    switch (channel)
    {
        case 0: return SINGLE_CH0;
        case 1: return SINGLE_CH1;
        case 2: return SINGLE_CH2;
        case 3: return SINGLE_CH3;
        case 4: return SINGLE_CH4;
        case 5: return SINGLE_CH5;
        case 6: return SINGLE_CH6;
        case 7: return SINGLE_CH7;
        default: return 0;
    }
}


uint16_t Grayscale_ReadChannel(uint16_t channel)
{ 
    uint16_t data_buff;
    Set_GrayscaleChannel(channel_switch(channel));
    data_buff = Grayscale_ReadData();
    return data_buff;
}

void Grayscale_ReadAllChannels(uint16_t *data_list)
{
    for(uint16_t i = 0; i < 8; i++)
    {
        * (data_list + i) = Grayscale_ReadChannel(i);
    }
}

