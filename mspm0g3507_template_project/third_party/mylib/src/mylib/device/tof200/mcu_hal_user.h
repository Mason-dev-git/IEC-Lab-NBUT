#ifndef __MCU_HAL_USER_H_
#define __MCU_HAL_USER_H_
#include "mcu_hal.h"


typedef enum
{
    MCU_IOIN_ONE = 0,
    MCU_IOIN_TWO,
    MCU_IOIN_THREE,
    MCU_IOIN_NUM,
} MCU_IOIN_ID;
void mcu_ioin_user_init(void);





typedef enum
{
    MCU_IOOUT_ONE = 0,
    MCU_IOOUT_TWO,
    MCU_IOOUT_NUM
} MCU_IOOUT_ID;
void mcu_ioout_user_init(void);





typedef enum
{
    MCU_DIO_ONE = 0,
    MCU_DIO_TWO,
    MCU_DIO_THREE,
    MCU_DIO_NUM
} MCU_DIO_ID;
void mcu_dio_user_init(void);


typedef enum
{
    MCU_UART_ONE = 0,
    MCU_UART_TWO = 1,
    MCU_UART_NUM
} MCU_UART_ID;



void mcu_uart_user_init(void);


void mcu_user_init(void);
void mcu_user_main(void);







#endif   //__MCU_HAL_USER_H_
