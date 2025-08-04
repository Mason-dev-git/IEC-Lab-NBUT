#include "device_matrix_key.h"
#include "stm32f10x.h"   // Device header
#include "driver_systick.h"

uint8_t matrix_key_determine(void);

char matrix_key_table[16] = {
    '1', '2', '3', '0', '4', '5', '6', 'E', '7', '8', '9', 'C', 'L', 'U', 'D', 'R'};

// 列 GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
// 行 GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3

void matrix_key_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    // GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t matrix_key_get_value(void)
{
    uint8_t Key;

    GPIO_ResetBits(GPIOA, GPIO_Pin_0);
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
    GPIO_SetBits(GPIOA, GPIO_Pin_3);

    Key = matrix_key_determine();

    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    GPIO_SetBits(GPIOA, GPIO_Pin_0);
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
    GPIO_SetBits(GPIOA, GPIO_Pin_3);

    if (matrix_key_determine()) { Key = matrix_key_determine() + 4; }

    GPIO_ResetBits(GPIOA, GPIO_Pin_2);
    GPIO_SetBits(GPIOA, GPIO_Pin_0);
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
    GPIO_SetBits(GPIOA, GPIO_Pin_3);

    if (matrix_key_determine()) { Key = matrix_key_determine() + 8; }

    GPIO_ResetBits(GPIOA, GPIO_Pin_3);
    GPIO_SetBits(GPIOA, GPIO_Pin_0);
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
    GPIO_SetBits(GPIOA, GPIO_Pin_2);


    if (matrix_key_determine()) { Key = matrix_key_determine() + 12; }

    return Key - 1;
}

uint8_t matrix_key_determine()
{
    uint8_t Key = 0;
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == 0) {
        systick_delay_ms(10);
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == 0) { Key = 1; }
    }

    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == 0) {
        systick_delay_ms(10);
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == 0) { Key = 2; }
    }

    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == 0) {
        systick_delay_ms(10);
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == 0) { Key = 3; }
    }

    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0) {
        systick_delay_ms(10);
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0) { Key = 4; }
    }
    return Key;
}
