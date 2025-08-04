#ifndef _HC_SR04_H
#define _HC_SR04_H

#include <stdint.h>

/*
    PC8:Trig(发射引脚)
    PC9:Echo(接收引脚)
*/
#define HC_R04_PIN GPIO_Pin_9
#define Trig_ON GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET)
#define Trig_OFF GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET)


void HCSR04_Init(void);


void SR04_Trigger(void);

uint32_t Get_Distance(void);


#endif
