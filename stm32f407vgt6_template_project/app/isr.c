#include "bsp.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    if (huart->Instance == USART3) {
        WT901C_IRQHandler();
    }
    if (huart->Instance == USART6) {
        tofxxf_irq_handler();
    }
}

void My_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    // TIM9 -> APB2 -> 168MHZ/168/10000 = 10ms
    if (htim->Instance == TIM9) {
        scan_key();
        led_irq_handler();
        buzzer_irq_handler();
    }
    // TIM5 -> APB1 -> 84MHZ/168-/10000 = 20ms
    if (htim->Instance == TIM5) {
        Controller_IRQHandler();
    }
}
