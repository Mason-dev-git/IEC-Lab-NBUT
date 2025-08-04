/*********************************************************************************
    *Copyright(C),20xx-20xx,

    *Filename:mcu_hal.h

    *Author:
            wwyyy
            Q1046685883

    *Version:
            V1.0

    *Data:
            2019/11/27

    *Description:


    *Other:


    *Function List:
        1.
        2.
        3.
        4.
    *History modify:
        1.Data:
        Author:
        Log:

**********************************************************************************/
#ifndef __MCU_HAL_H_
#define __MCU_HAL_H_
#include "type.h"
#include "stm32f10x.h"





/////////////////////////////////////////////配置core///////////////////////////////////////////////
void mcu_init(void);
void mcu_disable_irq(void);
void mcu_enable_irq(void);
void mcu_kick_dog(void);
void mcu_soft_reset(void);





//////////////////////////////////////////////配置tim//////////////////////////////////////////////
void mcu_tim_init(uint16 intvTime);
void mcu_tim_init_call(uint16 intvTime, void (*pfun_callBack)(void));
void mcu_tim_delayus(uint16 delayCount);
void mcu_tim_isr(void);   //用于驱动注册回调




//////////////////////////////////////////////配置ioin/////////////////////////////////////////////
typedef enum
{
    IN_FT = 0,   //浮空
    IN_PU,       //上拉
    IN_PD,       //下拉
} IOIN_MODE;
typedef struct
{
    uint32        portClk;      // clk
    GPIO_TypeDef* p_gpioPort;   // port
    uint16        gpioPin;      // pin
    IOIN_MODE     mode;         // 0:float,1:ipu,2:ipd
    uint8         flip;         // 0:不翻转,1:翻转
    uint16        filterLen;    //滤波长度
    //以下无需配置
    uint16 filterCnt;
    uint16 rollCount;
    uint8  status;
    uint8  lastStatus;
} MCU_IOIN_TAB;
void   mcu_ioin_init(MCU_IOIN_TAB* p_table, uint8 tableNum);
uint16 mcu_ioin_get_ready(uint8 channel);
void   mcu_ioin_clr_ready(uint8 channel);
uint8  mcu_ioin_get_status(uint8 channel);
void   mcu_ioin_main(void);





///////////////////////////////////////////ioout////////////////////////////////////////////////
typedef enum
{
    OUT_PP = 0,   //推挽
    OUT_OD,       //开漏(带上拉)
} IOOUT_MODE;
typedef struct
{
    uint32        portClk;      // clk
    GPIO_TypeDef* p_gpioPort;   // port
    uint16        gpioPin;      // pin
    IOOUT_MODE    mode;         //:0:pp,1:od
    uint8         flip;         // 0:不翻转,1:翻转
    uint8         initStatus;   //默认电平状态
    //以下无需配置
    uint16 rollCount;
    uint8  status;
} MCU_IOOUT_TAB;
void   mcu_ioout_init(MCU_IOOUT_TAB* p_table, uint8 tableNum);
uint16 mcu_ioout_get_ready(uint8 channel);
void   mcu_ioout_clr_ready(uint8 channel);
void   mcu_ioout_set_status(uint8 channel, uint8 status);
uint8  mcu_ioout_get_status(uint8 channel);
void   mcu_ioout_toggle(uint8 channel);
void   mcu_ioout_set_status_hard(uint8 channel, uint8 status);
uint8  mcu_ioout_get_status_hard(uint8 channel);
void   mcu_ioout_toggle_hard(uint8 channel);
void   mcu_ioout_main(void);





////////////////////////////////////////////配置dio/////////////////////////////////////////////
typedef enum
{
    DI_FT = 0,   //浮空输入
    DI_PU,       //上拉输入
    DI_PD,       //下拉输入
    DO_PP,       //推挽输出
    DO_OD,       //开漏输出(带上拉)
} DIO_MODE;
typedef struct
{
    uint32        portClk;      // clk
    GPIO_TypeDef* p_gpioPort;   // port
    uint16        gpioPin;      // pin
    DIO_MODE      mode;         // 0:float,1:ipu,2:ipd,3:pp,4:od;
    uint8         flip;         // 0:不翻转,1:翻转
    uint8         initStatus;   //默认电平状态
} MCU_DIO_TAB;
void  mcu_dio_init(MCU_DIO_TAB* p_table, uint8 tableNum);
void  mcu_dio_set_status(uint8 channel, uint8 status);
void  mcu_dio_toggle(uint8 channel);
uint8 mcu_dio_get_status(uint8 channel);







//////////////////////////////////////////////配置uart///////////////////////////////////////////
typedef struct
{
    USART_TypeDef*    p_uart;         //串口主体
    uint8             irqChannel;     //中断通道
    uint32            uartClk;        //串口时钟源
    uint32            portRemap;      //是否管脚映射
    USART_InitTypeDef initStruct;     //参数配置
    uint32            txPortClk;      //发送管脚时钟
    GPIO_TypeDef*     p_txPort;       //发送管脚主体
    uint16            txPin;          //发送管脚号
    uint32            rxPortClk;      //接收管脚时钟
    GPIO_TypeDef*     p_rxPort;       //接收管脚主体
    uint16            rxPin;          //接收管脚号
    void (*pfun_rxCallBack)(uint8);   //接收回调
} MCU_UART_TAB;
void  mcu_uart_init(MCU_UART_TAB* p_table, uint8 tableNum);
uint8 mcu_uart_tx(uint8 channel, void* p_tx, uint16 len);
void  mcu_uart_rx_isr(uint8 channel, uint8* p_recv);   //用于驱动回调





#endif   //__MCU_HAL_H_
