#include "interrupt.h"
#include "string.h"
#include "mcu_hal_user.h"
#include "timer_user.h"
#include "uart_tpm_user.h"

void NMI_Handler(void) {}
void HardFault_Handler(void)
{
    while (1) {}
}
void MemManage_Handler(void)
{
    while (1) {}
}
void BusFault_Handler(void)
{
    while (1) {}
}
void UsageFault_Handler(void)
{
    while (1) {}
}
void SVC_Handler(void) {}
void DebugMon_Handler(void) {}
void PendSV_Handler(void) {}
void SysTick_Handler(void)
{
    tim_isr();
}



void USART1_IRQHandler(void)
{
    uint8 data;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        data = USART_ReceiveData(USART1);
        uart_tpm_rx_isr(MCU_UART_ONE, data);
    }
}
void USART2_IRQHandler(void)
{
    uint8 data;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        data = USART_ReceiveData(USART2);
        uart_tpm_rx_isr(MCU_UART_TWO, data);
    }
}
void UART4_IRQHandler(void)
{
    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) { USART_ReceiveData(UART4); }
}


void TIM6_IRQHandler(void)
{
    //	if(TIM_GetITStatus(TIM6, TIM_IT_Update ) != RESET)
    //	{
    //		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    //		tim_isr();
    //	}
}


void PPP_IRQHandler(void) {}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
