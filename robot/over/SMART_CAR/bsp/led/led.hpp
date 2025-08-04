#ifndef __LED_HPP
#define __LED_HPP
#include "main.h"

class LED_Class {
private:
    GPIO_TypeDef *GPIOx; 
    uint16_t GPIO_Pin;
 
public:
    LED_Class(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);
    void Open(void);//开灯
    void Close(void);//关灯
    void toggle(void);//状态反转
 
};


#endif



