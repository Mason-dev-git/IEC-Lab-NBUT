#include "mcu_hal_user.h"
#include <stdio.h>

//ÅäÖÃioin
MCU_IOIN_TAB a_mcuIoInTale[MCU_IOIN_NUM] = {
    {RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_0, IN_PD, NFLP, 10},
    {RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_1, IN_PU, FLIP, 10},
    {RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_2, IN_PD, NFLP, 10},
};
void mcu_ioin_user_init(void)
{
    mcu_ioin_init(a_mcuIoInTale, MCU_IOIN_NUM);
}




//ÅäÖÃioout
MCU_IOOUT_TAB a_mcuIoOutTale[MCU_IOOUT_NUM] = {
    {RCC_APB2Periph_GPIOD, GPIOD, GPIO_Pin_12, OUT_PP, NFLP, LOW},
    {RCC_APB2Periph_GPIOD, GPIOD, GPIO_Pin_13, OUT_PP, FLIP, LOW},
};
void mcu_ioout_user_init(void)
{
    mcu_ioout_init(a_mcuIoOutTale, MCU_IOOUT_NUM);
}





//ÅäÖÃdio
MCU_DIO_TAB a_mcuDioTable[MCU_DIO_NUM] = {
    {RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_5, DI_PU, NFLP, LOW},
    {RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_6, DO_PP, FLIP, LOW},
    {RCC_APB2Periph_GPIOE, GPIOE, GPIO_Pin_2, DO_PP, NFLP, LOW},
};
void mcu_dio_user_init(void)
{
    mcu_dio_init(a_mcuDioTable, MCU_DIO_NUM);
}





//___________________________________________________________________________________________

int fputc(int ch, FILE* f)
{
    char str = ch;
    mcu_uart_tx(MCU_UART_TWO, &str, 1);
    return ch;
}

//ÅäÖÃuart
MCU_UART_TAB a_mcuUartTable[MCU_UART_NUM] = {
    {USART1,
     USART1_IRQn,
     RCC_APB2Periph_USART1,
     0,
     115200,
     USART_WordLength_8b,
     USART_StopBits_1,
     USART_Parity_No,
     USART_Mode_Rx | USART_Mode_Tx,
     USART_HardwareFlowControl_None,
     RCC_APB2Periph_GPIOA,
     GPIOA,
     GPIO_Pin_9,
     RCC_APB2Periph_GPIOA,
     GPIOA,
     GPIO_Pin_10,
     NULL},
    {USART2,
     USART2_IRQn,
     RCC_APB1Periph_USART2,
     0,
     115200,
     USART_WordLength_8b,
     USART_StopBits_1,
     USART_Parity_No,
     USART_Mode_Rx | USART_Mode_Tx,
     USART_HardwareFlowControl_None,
     RCC_APB2Periph_GPIOA,
     GPIOA,
     GPIO_Pin_2,
     RCC_APB2Periph_GPIOA,
     GPIOA,
     GPIO_Pin_3,
     NULL},
};
void mcu_uart_user_init(void)
{
    mcu_uart_init(a_mcuUartTable, MCU_UART_NUM);
}

void mcu_user_init(void)
{
    mcu_ioin_user_init();
    mcu_ioout_user_init();
    mcu_dio_user_init();
}
void mcu_user_main(void)
{
    mcu_ioin_main();
    mcu_ioout_main();
    // mcu_pwmi_main();
}
