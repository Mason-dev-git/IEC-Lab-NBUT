/*********************************************************************************
    *Copyright(C),20xx-20xx,

    *Filename:mcu_hal.c

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
#include "mcu_hal.h"
#include "public.h"
#include "string.h"
#include "timer.h"




//////////////////////////////////////////mcu///////////////////////////////////////////////////
/***********************************************************************************************
 *函数名: void mcu_init(void)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_init(void)
{
    SystemInit();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}

/***********************************************************************************************
 *函数名: void mcu_disable_irq(void)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_disable_irq(void)
{
    __disable_irq();
}

/***********************************************************************************************
 *函数名: void mcu_enable_irq(void)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_enable_irq(void)
{
    __enable_irq();
}

/***********************************************************************************************
 *函数名: void mcu_kick_dog(void)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_kick_dog(void) {}

/***********************************************************************************************
 *函数名: void mcu_soft_reset(void)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_soft_reset(void)
{
    NVIC_SystemReset();
}





//////////////////////////////////////////////配置tim//////////////////////////////////////////////
static void (*pfun_timIsr)(void) = NULL;
/***********************************************************************************************
 *函数名: void mcu_tim_init(uint16 intvTime)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_tim_init(uint16 intvTime)
{
    //	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    //	NVIC_InitTypeDef NVIC_InitStructure;
    //	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    //	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
    //	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    //	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    //	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //	NVIC_Init(&NVIC_InitStructure);
    //	TIM_TimeBaseStructure.TIM_Period = 1000*intvTime;
    //	TIM_TimeBaseStructure.TIM_Prescaler = 71;
    //	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    //	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
    //	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    //	TIM_Cmd(TIM6, ENABLE);
    SysTick_Config(SystemCoreClock / 1000 * intvTime);
}

/***********************************************************************************************
 *函数名: void mcu_tim_init_call(uint16 intvTime, void(*pfun_callBack)(void))
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_tim_init_call(uint16 intvTime, void (*pfun_callBack)(void))
{
    pfun_timIsr = pfun_callBack;
    mcu_tim_init(intvTime);
}

/***********************************************************************************************
 *函数名: void mcu_tim_delayus(uint16 delayCount)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_tim_delayus(uint16 delayCount)
{
#define TIM_DELAY_MAX_COUNT (SystemCoreClock / 1000 - 1)
    uint8  overRun    = 0;
    uint32 startCount = 0, endCount = 0, count = 0;
    uint32 delayCount1 = delayCount * (SystemCoreClock / 1000 / 1000);
    startCount         = SysTick->VAL;
    //向下计数
    if (startCount >= delayCount1) {
        endCount = startCount - delayCount1;
        overRun  = 0;
    }
    else {
        endCount = startCount + TIM_DELAY_MAX_COUNT - delayCount1;
        overRun  = 1;
    }
    while (1) {
        count = SysTick->VAL;
        if (0 == overRun) {
            if (count <= endCount || count > startCount) break;
        }
        else {
            if (count <= endCount && count > startCount) break;
        }
    }
    //	//向上计数
    //	if(startCount+delayCount1>=TIM_DELAY_MAX_COUNT)
    //	{
    //		overRun = 1;
    //	}
    //	else
    //	{
    //		overRun = 0;
    //	}
    //	endCount = (startCount+delayCount1)%TIM_DELAY_MAX_COUNT;
    //	while(1)
    //	{
    //		count = SysTick->VAL;
    //		if(0 == overRun)
    //		{
    //			if(count >= endCount || count < startCount)
    //				break;
    //		}
    //		else
    //		{
    //			if(count >= endCount && count < startCount)
    //				break;
    //		}
    //	}
}

/***********************************************************************************************
 *函数名: void mcu_tim_isr(void)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_tim_isr(void)
{
    if (NULL != pfun_timIsr) pfun_timIsr();
}





//////////////////////////////////////////////ioin//////////////////////////////////////////////
static uint8         mcuIoInTableNum = 0;
static MCU_IOIN_TAB* p_mcuIoInTable  = NULL;
/***********************************************************************************************
 *函数名: void mcu_ioin_init(MCU_IOIN_TAB *p_table, uint8 tableNum)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_ioin_init(MCU_IOIN_TAB* p_table, uint8 tableNum)
{
    uint8            i = 0;
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    mcuIoInTableNum               = tableNum;
    p_mcuIoInTable                = p_table;
    for (i = 0; i < mcuIoInTableNum; i++) {
        p_mcuIoInTable[i].filterCnt  = 0;
        p_mcuIoInTable[i].rollCount  = 0;
        p_mcuIoInTable[i].status     = 0;
        p_mcuIoInTable[i].lastStatus = 0;
        RCC_APB2PeriphClockCmd(p_mcuIoInTable[i].portClk, ENABLE);
        GPIO_InitStructure.GPIO_Pin = p_mcuIoInTable[i].gpioPin;
        if (IN_PU == p_mcuIoInTable[i].mode)   //上拉
        {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        }
        else if (IN_PD == p_mcuIoInTable[i].mode)   //下拉
        {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
        }
        else {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        }
        GPIO_Init(p_mcuIoInTable[i].p_gpioPort, &GPIO_InitStructure);   //初始化管脚
    }
}

/***********************************************************************************************
 *函数名: uint16 mcu_ioin_get_ready(uint8 channel)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
uint16 mcu_ioin_get_ready(uint8 channel)
{
    return p_mcuIoInTable[channel].rollCount;
}

/***********************************************************************************************
 *函数名: void mcu_ioin_clr_ready(uint8 channel)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_ioin_clr_ready(uint8 channel)
{
    p_mcuIoInTable[channel].rollCount = 0;
}

/***********************************************************************************************
 *函数名: uint16 mcu_ioin_get_status(uint8 channel)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
uint8 mcu_ioin_get_status(uint8 channel)
{
    return p_mcuIoInTable[channel].status;
}

/***********************************************************************************************
 *函数名: void mcu_ioin_main(void)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_ioin_main(void)
{
    uint8 i  = 0;
    uint8 re = 0;
    for (i = 0; i < mcuIoInTableNum; i++) {
        re = GPIO_ReadInputDataBit(p_mcuIoInTable[i].p_gpioPort, p_mcuIoInTable[i].gpioPin) ? 1 : 0;
        re = p_mcuIoInTable[i].flip == re ? 0 : 1;
        if (p_mcuIoInTable[i].lastStatus == re) {
            p_mcuIoInTable[i].filterCnt++;
            if (p_mcuIoInTable[i].filterCnt >= p_mcuIoInTable[i].filterLen) {
                p_mcuIoInTable[i].filterCnt = p_mcuIoInTable[i].filterLen;   //=0;
                p_mcuIoInTable[i].status    = re;
                updata_roll_count16(&p_mcuIoInTable[i].rollCount);
            }
        }
        else {
            p_mcuIoInTable[i].filterCnt  = 0;
            p_mcuIoInTable[i].lastStatus = re;
        }
    }
}





///////////////////////////////////////////ioout////////////////////////////////////////////////
static uint8          mcuIoOutTableNum = 0;
static MCU_IOOUT_TAB* p_mcuIoOutTable  = NULL;
/***********************************************************************************************
 *函数名: void mcu_ioout_init(MCU_IOOUT_TAB *p_table, uint8 tableNum)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_ioout_init(MCU_IOOUT_TAB* p_table, uint8 tableNum)
{
    uint8            i = 0;
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    mcuIoOutTableNum              = tableNum;
    p_mcuIoOutTable               = p_table;
    for (i = 0; i < mcuIoOutTableNum; i++) {
        p_mcuIoOutTable[i].rollCount = 0;
        p_mcuIoOutTable[i].status    = p_mcuIoOutTable[i].initStatus ? 1 : 0;
        RCC_APB2PeriphClockCmd(p_mcuIoOutTable[i].portClk, ENABLE);
        GPIO_InitStructure.GPIO_Pin = p_mcuIoOutTable[i].gpioPin;
        if (OUT_OD == p_mcuIoOutTable[i].mode)   //开漏
        {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
        }
        else {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        }
        GPIO_Init(p_mcuIoOutTable[i].p_gpioPort, &GPIO_InitStructure);
        if (p_mcuIoOutTable[i].flip) {
            GPIO_WriteBit(p_mcuIoOutTable[i].p_gpioPort,
                          p_mcuIoOutTable[i].gpioPin,
                          (BitAction)!p_mcuIoOutTable[i].status);
        }
        else {
            GPIO_WriteBit(p_mcuIoOutTable[i].p_gpioPort,
                          p_mcuIoOutTable[i].gpioPin,
                          (BitAction)p_mcuIoOutTable[i].status);
        }
    }
}

/***********************************************************************************************
 *函数名: uint16 mcu_ioout_get_ready(uint8 channel)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
uint16 mcu_ioout_get_ready(uint8 channel)
{
    return p_mcuIoOutTable[channel].rollCount;
}

/***********************************************************************************************
 *函数名: void mcu_ioout_clr_ready(uint8 channel)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_ioout_clr_ready(uint8 channel)
{
    p_mcuIoOutTable[channel].rollCount = 0;
}

/***********************************************************************************************
 *函数名: void mcu_ioout_set_status(uint8 channel, uint8 status)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_ioout_set_status(uint8 channel, uint8 status)
{
    p_mcuIoOutTable[channel].status = status;
}

/***********************************************************************************************
 *函数名: uint8 mcu_ioout_get_status(uint8 channel)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
uint8 mcu_ioout_get_status(uint8 channel)
{
    return p_mcuIoOutTable[channel].status;
}

/***********************************************************************************************
 *函数名: void mcu_ioout_toggle(uint8 channel)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_ioout_toggle(uint8 channel)
{
    p_mcuIoOutTable[channel].status = !p_mcuIoOutTable[channel].status;
}

/***********************************************************************************************
 *函数名: void mcu_ioout_set_status_hard(uint8 channel, uint8 status)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_ioout_set_status_hard(uint8 channel, uint8 status)
{
    p_mcuIoOutTable[channel].status = status;
    if (p_mcuIoOutTable[channel].flip) {
        GPIO_WriteBit(p_mcuIoOutTable[channel].p_gpioPort,
                      p_mcuIoOutTable[channel].gpioPin,
                      (BitAction)!p_mcuIoOutTable[channel].status);
    }
    else {
        GPIO_WriteBit(p_mcuIoOutTable[channel].p_gpioPort,
                      p_mcuIoOutTable[channel].gpioPin,
                      (BitAction)p_mcuIoOutTable[channel].status);
    }
    updata_roll_count16(&p_mcuIoOutTable[channel].rollCount);
}

/***********************************************************************************************
 *函数名: uint8 mcu_ioout_get_status_hard(uint8 channel)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
uint8 mcu_ioout_get_status_hard(uint8 channel)
{
    uint8 re = 0;
    re       = GPIO_ReadOutputDataBit(p_mcuIoOutTable[channel].p_gpioPort,
                                p_mcuIoOutTable[channel].gpioPin)
                   ? 1
                   : 0;
    return (re == p_mcuIoOutTable[channel].flip ? 0 : 1);
}

/***********************************************************************************************
 *函数名: void mcu_ioout_toggle_hard(uint8 channel)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_ioout_toggle_hard(uint8 channel)
{
    p_mcuIoOutTable[channel].status = !p_mcuIoOutTable[channel].status;
    if (p_mcuIoOutTable[channel].flip) {
        GPIO_WriteBit(p_mcuIoOutTable[channel].p_gpioPort,
                      p_mcuIoOutTable[channel].gpioPin,
                      (BitAction)!p_mcuIoOutTable[channel].status);
    }
    else {
        GPIO_WriteBit(p_mcuIoOutTable[channel].p_gpioPort,
                      p_mcuIoOutTable[channel].gpioPin,
                      (BitAction)p_mcuIoOutTable[channel].status);
    }
    updata_roll_count16(&p_mcuIoOutTable[channel].rollCount);
}

/***********************************************************************************************
 *函数名: void mcu_ioout_main(void)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_ioout_main(void)
{
    uint8 i = 0;
    for (i = 0; i < mcuIoOutTableNum; i++) {
        if (p_mcuIoOutTable[i].flip) {
            GPIO_WriteBit(p_mcuIoOutTable[i].p_gpioPort,
                          p_mcuIoOutTable[i].gpioPin,
                          (BitAction)!p_mcuIoOutTable[i].status);
        }
        else {
            GPIO_WriteBit(p_mcuIoOutTable[i].p_gpioPort,
                          p_mcuIoOutTable[i].gpioPin,
                          (BitAction)p_mcuIoOutTable[i].status);
        }
        updata_roll_count16(&p_mcuIoOutTable[i].rollCount);
    }
}





////////////////////////////////////////////配置dio/////////////////////////////////////////////
static uint8        mcuDioTableNum = 0;
static MCU_DIO_TAB* p_mcuDioTable  = NULL;
/***********************************************************************************************
 *函数名: void mcu_dio_init(MCU_DIO_TAB *p_table, uint8 tableNum)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_dio_init(MCU_DIO_TAB* p_table, uint8 tableNum)
{
    uint8            i = 0;
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    mcuDioTableNum                = tableNum;
    p_mcuDioTable                 = p_table;
    for (i = 0; i < mcuDioTableNum; i++) {
        RCC_APB2PeriphClockCmd(p_mcuDioTable[i].portClk, ENABLE);
        GPIO_InitStructure.GPIO_Pin = p_mcuDioTable[i].gpioPin;
        if (DI_FT == p_mcuDioTable[i].mode) {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        }
        else if (DI_PU == p_mcuDioTable[i].mode) {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        }
        else if (DI_PD == p_mcuDioTable[i].mode) {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
        }
        else if (DO_PP == p_mcuDioTable[i].mode) {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        }
        else if (DO_OD == p_mcuDioTable[i].mode) {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
        }
        else {
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        }
        GPIO_Init(p_mcuDioTable[i].p_gpioPort, &GPIO_InitStructure);
        if (DO_PP == p_mcuDioTable[i].mode || DO_OD == p_mcuDioTable[i].mode) {
            if (p_mcuDioTable[i].flip) {
                GPIO_WriteBit(p_mcuDioTable[i].p_gpioPort,
                              p_mcuDioTable[i].gpioPin,
                              (BitAction)!p_mcuDioTable[i].initStatus);
            }
            else {
                GPIO_WriteBit(p_mcuDioTable[i].p_gpioPort,
                              p_mcuDioTable[i].gpioPin,
                              (BitAction)p_mcuDioTable[i].initStatus);
            }
        }
    }
}

/***********************************************************************************************
 *函数名: void mcu_dio_set_status(uint8 channel, uint8 status)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_dio_set_status(uint8 channel, uint8 status)
{
    if (DO_PP == p_mcuDioTable[channel].mode || DO_OD == p_mcuDioTable[channel].mode) {
        if (p_mcuDioTable[channel].flip) {
            GPIO_WriteBit(p_mcuDioTable[channel].p_gpioPort,
                          p_mcuDioTable[channel].gpioPin,
                          (BitAction)!status);
        }
        else {
            GPIO_WriteBit(p_mcuDioTable[channel].p_gpioPort,
                          p_mcuDioTable[channel].gpioPin,
                          (BitAction)status);
        }
    }
}

/***********************************************************************************************
 *函数名: void mcu_dio_toggle(uint8 channel)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_dio_toggle(uint8 channel)
{
    if (DO_PP == p_mcuDioTable[channel].mode || DO_OD == p_mcuDioTable[channel].mode) {
        GPIO_FlipBits(p_mcuDioTable[channel].p_gpioPort, p_mcuDioTable[channel].gpioPin);
    }
}

/***********************************************************************************************
 *函数名: uint8 mcu_dio_get_status(uint8 channel)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
uint8 mcu_dio_get_status(uint8 channel)
{
    //	if(DO_PP == p_mcuDioTable[channel].mode ||\
//		DO_OD == p_mcuDioTable[channel].mode)
    //	{
    //		return ((GPIO_ReadOutputDataBit(p_mcuDioTable[channel].p_gpioPort,
    //p_mcuDioTable[channel].gpioPin) ? 1 : 0) ==\ 			p_mcuDioTable[channel].flip) ? 0 : 1;
    //	}
    //	else
    //	{
    return ((GPIO_ReadInputDataBit(p_mcuDioTable[channel].p_gpioPort,
                                   p_mcuDioTable[channel].gpioPin)
                 ? 1
                 : 0) == p_mcuDioTable[channel].flip)
               ? 0
               : 1;
    //	}
}








//////////////////////////////////////////////配置uart///////////////////////////////////////////
static uint8         mcuUartTableNum = 0;
static MCU_UART_TAB* p_mcuUartTable  = NULL;
/***********************************************************************************************
 *函数名: void mcu_uart_init(MCU_UART_TAB *p_table, uint8 tableNum)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_uart_init(MCU_UART_TAB* p_table, uint8 tableNum)
{
    uint8            i = 0;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    mcuUartTableNum = tableNum;   // tableNum配置的串口个数
    p_mcuUartTable  = p_table;    // p_table串口的各个参数
    for (i = 0; i < mcuUartTableNum; i++) {
        RCC_APB2PeriphClockCmd(p_mcuUartTable[i].txPortClk, ENABLE);
        RCC_APB2PeriphClockCmd(p_mcuUartTable[i].rxPortClk, ENABLE);
        if (p_mcuUartTable[i].portRemap)   //管脚是否映射
        {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
            GPIO_PinRemapConfig(p_mcuUartTable[i].portRemap, ENABLE);
        }
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Pin  = p_mcuUartTable[i].rxPin;
        GPIO_Init(p_mcuUartTable[i].p_rxPort, &GPIO_InitStructure);   //初始化接收管脚
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Pin   = p_mcuUartTable[i].txPin;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(p_mcuUartTable[i].p_txPort, &GPIO_InitStructure);   //初始化发送管脚
        USART_DeInit(p_mcuUartTable[i].p_uart);                       //复位串口
        if (USART1 == p_mcuUartTable[i].p_uart)                       //发开串口时钟
        {
            RCC_APB2PeriphClockCmd(p_mcuUartTable[i].uartClk, ENABLE);
        }
        else {
            RCC_APB1PeriphClockCmd(p_mcuUartTable[i].uartClk, ENABLE);
        }
        NVIC_InitStructure.NVIC_IRQChannel                   = p_mcuUartTable[i].irqChannel;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
        NVIC_Init(&NVIC_InitStructure);   //配置串口中断
        USART_Init((USART_TypeDef*)p_mcuUartTable[i].p_uart,
                   (USART_InitTypeDef*)&p_mcuUartTable[i].initStruct);
        USART_ITConfig(p_mcuUartTable[i].p_uart, USART_IT_RXNE, ENABLE);   //使能接收中断
        USART_ClearFlag(p_mcuUartTable[i].p_uart, USART_FLAG_RXNE);
        USART_Cmd(p_mcuUartTable[i].p_uart, ENABLE);   //使能串口
    }
}

/***********************************************************************************************
 *函数名: uint8 mcu_uart_tx(uint8 channel, void *p_tx, uint16 len)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
uint8 mcu_uart_tx(uint8 channel, void* p_tx, uint16 len)
{
    uint16 i = 0;
    for (i = 0; i < len; i++) {
        USART_SendData(p_mcuUartTable[channel].p_uart, ((uint8*)p_tx)[i]);
        while (USART_GetFlagStatus(p_mcuUartTable[channel].p_uart, USART_FLAG_TXE) == RESET)
            ;
    }
    return 0;
}

/***********************************************************************************************
 *函数名: void mcu_uart_rx_isr(uint8 channel, uint8 *p_recv)
 *功能描述:
 *特别说明:
 *函数参数:
 *函数返回值:
 *修改记录:
 ***********************************************************************************************/
void mcu_uart_rx_isr(uint8 channel, uint8* p_recv)
{
    uint8 data = 0;
    if (USART_GetITStatus(p_mcuUartTable[channel].p_uart, USART_IT_RXNE) != RESET) {
        data = USART_ReceiveData(p_mcuUartTable[channel].p_uart);   //接收数据
        if (NULL != p_recv) { *p_recv = data; }
        if (NULL != p_mcuUartTable[channel].pfun_rxCallBack) {
            p_mcuUartTable[channel].pfun_rxCallBack(data);
        }
    }
}
