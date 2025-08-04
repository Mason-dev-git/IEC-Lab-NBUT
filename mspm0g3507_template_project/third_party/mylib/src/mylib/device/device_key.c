#include <mylib/device/device_key.h>
#include <mylib/driver/driver_systick.h>
#include <mylib/utility/debug.h>
#include "main.h"
#include "ti_msp_dl_config.h"

struct key_t keys[4];
void key_init(void)
{
    //    DL_GPIO_setPins(GPIOA, DL_GPIO_PIN_18);
    //    DL_GPIO_setPins(GPIOB, DL_GPIO_PIN_21);
    //    DL_GPIO_setPins(GPIOB, DL_GPIO_PIN_23);
    //    DL_GPIO_setPins(GPIOB, DL_GPIO_PIN_24);
}

int32_t key_get_value(void)
{
    // systick_delay_ms(100);
    // DEBUG_INFO("key_get_value");

    // delay_cycles(3200000); // 100ms
    // DEBUG_INFO("if (DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_18) == 0)");

    if (DL_GPIO_readPins(GPIO_GRP_MISC_PORT,GPIO_GRP_MISC_PIN_KEY_1_PIN) == 0)   //按下为低电平
    {
        delay_ms(25);                                       // 20ms
        if (DL_GPIO_readPins(GPIO_GRP_MISC_PORT,GPIO_GRP_MISC_PIN_KEY_1_PIN) == 0)   //按下为低电平
        {
            keys[0].singleFlag = 1;
            // DEBUG_INFO("if (DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_18) == 0)");
            while (DL_GPIO_readPins(GPIO_GRP_MISC_PORT,GPIO_GRP_MISC_PIN_KEY_1_PIN) > 0)
                ;   //等待按键释放
            return KEY_VALUE_1;
        }
    }
    if (DL_GPIO_readPins(GPIO_GRP_MISC_PORT,GPIO_GRP_MISC_PIN_KEY_2_PIN) == 0)   //按下为低电平
    {
        delay_ms(25);                                       // 20ms
        if (DL_GPIO_readPins(GPIO_GRP_MISC_PORT,GPIO_GRP_MISC_PIN_KEY_2_PIN) == 0)   //按下为低电平
        {
            keys[1].singleFlag = 1;
            // DEBUG_INFO("if (DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_18) == 0)");
            while (DL_GPIO_readPins(GPIO_GRP_MISC_PORT,GPIO_GRP_MISC_PIN_KEY_2_PIN) > 0)
                ;   //等待按键释放
            return KEY_VALUE_2;
        }
    }

    if (DL_GPIO_readPins(GPIO_GRP_MISC_PORT,GPIO_GRP_MISC_PIN_KEY_3_PIN) == 0)   //按下为低电平
    {
        delay_ms(25);                                       // 20ms
        if (DL_GPIO_readPins(GPIO_GRP_MISC_PORT,GPIO_GRP_MISC_PIN_KEY_3_PIN) == 0)   //按下为低电平
        {
            keys[2].singleFlag = 1;
            // DEBUG_INFO("if (DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_18) == 0)");
            while (DL_GPIO_readPins(GPIO_GRP_MISC_PORT,GPIO_GRP_MISC_PIN_KEY_3_PIN) > 0)
                ;   //等待按键释放
            return KEY_VALUE_3;
        }
    }
    
    if (DL_GPIO_readPins(GPIO_GRP_MISC_PORT,GPIO_GRP_MISC_PIN_KEY_4_PIN) == 0)   //按下为低电平
    {
        delay_ms(25);                                       // 20ms
        if (DL_GPIO_readPins(GPIO_GRP_MISC_PORT,GPIO_GRP_MISC_PIN_KEY_4_PIN) == 0)   //按下为低电平
        {
            keys[3].singleFlag = 1;
            // DEBUG_INFO("if (DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_18) == 0)");
            while (DL_GPIO_readPins(GPIO_GRP_MISC_PORT,GPIO_GRP_MISC_PIN_KEY_4_PIN) > 0)
                ;   //等待按键释放
            return KEY_VALUE_4;
        }
    }

    return KEY_VALUE_NONE;
}
