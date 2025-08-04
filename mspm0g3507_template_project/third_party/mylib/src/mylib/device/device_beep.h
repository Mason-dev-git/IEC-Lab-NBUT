#ifndef MYLIB_DEVICE_W25Q64_H
#define MYLIB_DEVICE_W25Q64_H

#include <mylib/mylib_config.h>

typedef struct
{
    GPIO_Regs* port;
    uint32_t   pin;
    int8_t     valid;   // 什么时候响，如果是VALID_ON_LOW_LEVEL，那么在调用beep_on时，会被设置为低电平
} beep_t;

void beep_on(beep_t* beep);

void beep_off(beep_t* beep);



#endif   // !MYLIB_DEVICE_W25Q64_H
