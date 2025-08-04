#include "led.hpp"

//构造函数
LED_Class::LED_Class(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
    LED_Class::GPIOx = GPIOx;
    LED_Class::GPIO_Pin = GPIO_Pin;
}
 
void LED_Class::Open(void)
{
    HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_SET);
}
 
void LED_Class::Close(void)
{
    HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_RESET);
}
 
void LED_Class::toggle(void)
{
    HAL_GPIO_TogglePin(GPIOx,GPIO_Pin);
}


