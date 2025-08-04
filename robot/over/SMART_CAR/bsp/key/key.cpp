#include "key.hpp"


Key::Key()
{
    key_mode=0;
}

uint8_t Key::Change(void)
{
    if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==0)
    {
        HAL_Delay(10);
        while ((HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==0))
        {
            HAL_Delay(1);
        }
        return 1;
    }
    if(HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==0)
    {
        HAL_Delay(10);
        while ((HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==0))
        {
            HAL_Delay(1);
        }
        return 2;
    }
    if(HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin)==0)
    {
        HAL_Delay(10);
        while ((HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin)==0))
        {
            HAL_Delay(1);
        }
        return 3;
    }
	if(HAL_GPIO_ReadPin(KEY4_GPIO_Port,KEY4_Pin)==0)
    {
        HAL_Delay(10);
        while ((HAL_GPIO_ReadPin(KEY4_GPIO_Port,KEY4_Pin)==0))
        {
            HAL_Delay(1);
        }
        return 4;
    }
    return 0;
}


uint8_t Key::Read(void)
{
    if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==0)
    {
        HAL_Delay(10);
        while ((HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==0))
        {
            HAL_Delay(1);
        }
        key_mode=1;
    }
    if(HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==0)
    {
        HAL_Delay(10);
        while ((HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==0))
        {
            HAL_Delay(1);
        }
        key_mode=2;
    }
    if(HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin)==0)
    {
        HAL_Delay(10);
        while ((HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin)==0))
        {
            HAL_Delay(1);
        }
        key_mode=3;
    }
	if(HAL_GPIO_ReadPin(KEY4_GPIO_Port,KEY4_Pin)==0)
    {
        HAL_Delay(10);
        while ((HAL_GPIO_ReadPin(KEY4_GPIO_Port,KEY4_Pin)==0))
        {
            HAL_Delay(1);
        }
        key_mode=4;
    }
    return key_mode;
}
 

