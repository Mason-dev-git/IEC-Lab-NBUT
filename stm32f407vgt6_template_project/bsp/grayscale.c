#include "grayscale.h"

GrayscaleInitTypeDef Grayscale_InitStructure;

/**
 * @brief Grayscale初始化
 *
 */
void Grayscale_Init(void)
{
    Grayscale_InitStructure.address = DEFAULT_SLAVE_ADDR;     // 从机地址
    Grayscale_InitStructure.input_mode = SINGLE_ENDED_MODE;   // 输入模式
    Grayscale_InitStructure.PD_selection = PD_MODE_ADC_ON;    // 电源管理模式
    Grayscale_InitStructure.channel = SINGLE_CH0;             // 通道选择
}

/**
 * @brief 设置Grayscale从机地址
 *
 * @param address 从机地址
 */
void Set_GrayscaleAddress(uint8_t address)
{
    Grayscale_InitStructure.address = address;
}

/**
 * @brief 设置Grayscale输入模式
 *
 * @param input_mode 输入模式
 */
void Set_GrayscaleInputMode(uint8_t input_mode)
{
    Grayscale_InitStructure.input_mode = input_mode;
}

/**
 * @brief 设置Grayscale电源管理模式
 *
 * @param PD_selection 电源管理模式
 */
void Set_GrayscalePDSelection(uint8_t PD_selection)
{
    Grayscale_InitStructure.PD_selection = PD_selection;
}

/**
 * @brief 设置Grayscale转换通道
 *
 * @param channel 所选通道
 */
void Set_GrayscaleChannel(uint8_t channel)
{
    Grayscale_InitStructure.channel = channel;
}

/**
 * @brief 发送命令（由上述模式确定）
 *
 */
void Grayscale_SendCommand(void)
{
    uint8_t command =
        Grayscale_InitStructure.input_mode | Grayscale_InitStructure.PD_selection | Grayscale_InitStructure.channel;
    HAL_I2C_Master_Transmit(&hi2c3, (Grayscale_InitStructure.address << 1), &command, 1, HAL_MAX_DELAY);
}

/**
 * @brief 读取数据
 *
 * @return uint16_t 指定通道数据（高4位为0）
 */
uint16_t Grayscale_ReadData(void)
{
    uint8_t data[2] = {0};
    Grayscale_SendCommand();
    HAL_I2C_Master_Receive(&hi2c3, (Grayscale_InitStructure.address << 1), data, 2, HAL_MAX_DELAY);
    return (data[0] << 8) | data[1];
}

/**
 * @brief 通道选择（内部使用）
 *
 * @param channel 通道编号
 * @return uint8_t 通道码
 */
uint8_t channel_switch(uint16_t channel)
{
    switch (channel) {
        case 0:
            if (Grayscale_InitStructure.input_mode == SINGLE_ENDED_MODE)
                return SINGLE_CH0;
            else
                return DIFFER_CH0_1;
        case 1:
            if (Grayscale_InitStructure.input_mode == SINGLE_ENDED_MODE)
                return SINGLE_CH1;
            else
                return DIFFER_CH2_3;
        case 2:
            if (Grayscale_InitStructure.input_mode == SINGLE_ENDED_MODE)
                return SINGLE_CH2;
            else
                return DIFFER_CH4_5;
        case 3:
            if (Grayscale_InitStructure.input_mode == SINGLE_ENDED_MODE)
                return SINGLE_CH3;
            else
                return DIFFER_CH6_7;
        case 4:
            if (Grayscale_InitStructure.input_mode == SINGLE_ENDED_MODE)
                return SINGLE_CH4;
            else
                return DIFFER_CH1_0;
        case 5:
            if (Grayscale_InitStructure.input_mode == SINGLE_ENDED_MODE)
                return SINGLE_CH5;
            else
                return DIFFER_CH3_2;
        case 6:
            if (Grayscale_InitStructure.input_mode == SINGLE_ENDED_MODE)
                return SINGLE_CH6;
            else
                return DIFFER_CH5_4;
        case 7:
            if (Grayscale_InitStructure.input_mode == SINGLE_ENDED_MODE)
                return SINGLE_CH7;
            else
                return DIFFER_CH7_6;
        default: return 0;
    }
}

/**
 * @brief 读取指定通道数据
 *
 * @param channel 通道编号
 * @return uint16_t 读取数据
 */
uint16_t Grayscale_ReadChannel(uint16_t channel)
{
    Set_GrayscaleChannel(channel_switch(channel));
    uint16_t data_buff = Grayscale_ReadData();
    return data_buff;
}

/**
 * @brief 读取所有通道数据
 *
 * @param data_buff 数据缓冲区
 */
void Grayscale_ReadAllChannels(uint16_t* data_buff)
{
    for (uint16_t i = 0; i < 8; i++) {
        *(data_buff + i) = Grayscale_ReadChannel(i);
    }
}

/**
 * @brief Grayscale测试代码，串口打印所有通道数据，使用前务必初始化串口屏幕打印
 *
 */
void test_grayscale(void)
{
    uint16_t data_buff[8];
    Grayscale_Init();
    while (1) {
        Grayscale_ReadAllChannels(data_buff);
        debug_print("%d,%d,%d,%d,%d,%d,%d,%d\n",
                    data_buff[0],
                    data_buff[1],
                    data_buff[2],
                    data_buff[3],
                    data_buff[4],
                    data_buff[5],
                    data_buff[6],
                    data_buff[7]);
    }
}
