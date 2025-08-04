#include "soft_i2c.h"

#if PLATFORM_STM32
#    define SOFT_I2C_SETBIT(gpio, pin) gpio->BSRR = pin
#    define SOFT_I2C_RESETBIT(gpio, pin) gpio->BRR = pin
#    define SOFT_I2C_READ(gpio, pin) ((gpio->IDR & pin) != 0)

#elif PLATFORM_MSPM0
#    define i2c_read_sda() ((DL_GPIO_readPins(soft_i2c->sda_gpio, soft_i2c->sda_pin)) ?: 1, 0)
#    define i2c_sda_high() DL_GPIO_setPins(soft_i2c->sda_gpio, soft_i2c->sda_pin)
#    define i2c_sda_low() DL_GPIO_clearPins(soft_i2c->sda_gpio, soft_i2c->sda_pin)
#    define i2c_scl_high() DL_GPIO_setPins(soft_i2c->scl_gpio, soft_i2c->scl_pin)
#    define i2c_scl_low() DL_GPIO_clearPins(soft_i2c->scl_gpio, soft_i2c->scl_pin)
#endif


// i2c_sda_out(, )
void i2c_sda_out(soft_i2c_t* soft_i2c)
{
#if PLATFORM_MSPM0
    DL_GPIO_initDigitalOutput(soft_i2c->sda_iomux);

    DL_GPIO_enableOutput(soft_i2c->sda_gpio, soft_i2c->sda_pin);
#endif
}
void i2c_sda_in(soft_i2c_t* soft_i2c)
{

#if PLATFORM_MSPM0
    // DL_GPIO_initDigitalInput(USER_GPIO_OLED_SDA_IOMUX);
    DL_GPIO_initDigitalInputFeatures(soft_i2c->sda_iomux,
                                     DL_GPIO_INVERSION_DISABLE,
                                     DL_GPIO_RESISTOR_PULL_UP,
                                     DL_GPIO_HYSTERESIS_DISABLE,
                                     DL_GPIO_WAKEUP_DISABLE);
#endif
}


void soft_i2c_init(soft_i2c_t* soft_i2c);

/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void soft_i2c_delay(void)
{
    volatile uint8_t i;

    /*　
        下面的时间是通过安富莱AX-Pro逻辑分析仪测试得到的。
        CPU主频72MHz时，在内部Flash运行, MDK工程不优化
        循环次数为10时，SCL频率 = 205KHz
        循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us
        循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us

    IAR工程编译效率高，不能设置为7
    */
#if PLATFORM_STM32
    for (i = 0; i < 10; i++)
        ;
#elif PLATFORM_MSPM0
    for (i = 0; i < 5; i++)
        ;
#endif
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void soft_i2c_start(soft_i2c_t* soft_i2c)
{
#if PLATFORM_STM32
    /* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */

    SOFT_I2C_SETBIT(soft_i2c->scl_gpio, soft_i2c->scl_pin);
    SOFT_I2C_SETBIT(soft_i2c->sda_gpio, soft_i2c->sda_pin);

    soft_i2c_delay();
    SOFT_I2C_RESETBIT(soft_i2c->sda_gpio, soft_i2c->sda_pin);

    soft_i2c_delay();
    SOFT_I2C_RESETBIT(soft_i2c->scl_gpio, soft_i2c->scl_pin);

    soft_i2c_delay();
#elif PLATFORM_MSPM0
    /*    _____
     *SDA      \_____________
     *    __________
     *SCL           \________
     */

    i2c_sda_out(soft_i2c);
    i2c_sda_high();
    i2c_scl_high();
    soft_i2c_delay();
    i2c_sda_low();
    soft_i2c_delay();
    i2c_scl_low();
    soft_i2c_delay();
#endif
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void soft_i2c_stop(soft_i2c_t* soft_i2c)
{
#if PLATFORM_STM32

    /* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */

    SOFT_I2C_RESETBIT(soft_i2c->sda_gpio, soft_i2c->sda_pin);
    SOFT_I2C_SETBIT(soft_i2c->scl_gpio, soft_i2c->scl_pin);
    soft_i2c_delay();
    SOFT_I2C_SETBIT(soft_i2c->sda_gpio, soft_i2c->sda_pin);
#elif PLATFORM_MSPM0

    /*               _______
     *SDA __________/
     *          ____________
     *SCL _____/
     */
    i2c_sda_out(soft_i2c);
    i2c_sda_low();
    i2c_scl_high();
    soft_i2c_delay();
    i2c_sda_high();
    soft_i2c_delay();
#endif
}

/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void soft_i2c_send_byte(soft_i2c_t* soft_i2c, uint8_t byte)
{

#if PLATFORM_STM32
    uint8_t i;

    /* 先发送字节的高位bit7 */
    for (i = 0; i < 8; i++) {
        if (byte & 0x80) { SOFT_I2C_SETBIT(soft_i2c->sda_gpio, soft_i2c->sda_pin); }
        else {
            SOFT_I2C_RESETBIT(soft_i2c->sda_gpio, soft_i2c->sda_pin);
        }
        soft_i2c_delay();
        SOFT_I2C_SETBIT(soft_i2c->scl_gpio, soft_i2c->scl_pin);
        soft_i2c_delay();
        SOFT_I2C_RESETBIT(soft_i2c->scl_gpio, soft_i2c->scl_pin);
        if (i == 7) {
            SOFT_I2C_SETBIT(soft_i2c->sda_gpio, soft_i2c->sda_pin);   // 释放总线
        }
        byte <<= 1; /* 左移一个bit */
        soft_i2c_delay();
    }
#elif PLATFORM_MSPM0
    uint8_t i;
    i2c_sda_out(soft_i2c);
    i2c_scl_low();
    for (i = 0; i < 8; i++) {
        if ((byte & 0x80) >> 7) { i2c_sda_high(); }
        else {
            i2c_sda_low();
        }
        byte <<= 1;
        soft_i2c_delay();
        i2c_scl_high();
        soft_i2c_delay();
        i2c_scl_low();
        soft_i2c_delay();
    }
    soft_i2c_wait_ack(soft_i2c);
#endif
}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t soft_i2c_read_byte(soft_i2c_t* soft_i2c, uint8_t ack)
{
#if PLATFORM_STM32
    uint8_t i;
    uint8_t value;

    /* 读到第1个bit为数据的bit7 */
    value = 0;
    for (i = 0; i < 8; i++) {
        value <<= 1;
        SOFT_I2C_SETBIT(soft_i2c->scl_gpio, soft_i2c->scl_pin);
        soft_i2c_delay();
        if (SOFT_I2C_READ(soft_i2c->sda_gpio, soft_i2c->scl_pin)) { value++; }
        SOFT_I2C_RESETBIT(soft_i2c->scl_gpio, soft_i2c->scl_pin);
        soft_i2c_delay();
    }
    if (ack == 0)
        soft_i2c_nack(soft_i2c);
    else
        soft_i2c_ack(soft_i2c);
    return value;
#elif PLATFORM_MSPM0
    uint8_t i, data = 0;
    for (i = 0; i < 8; i++) {
        data <<= 1;
        i2c_scl_high();
        soft_i2c_delay();
        if (i2c_read_sda()) { data++; }
        i2c_scl_low();
        soft_i2c_delay();
    }
    return data;
#endif
}

/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t soft_i2c_wait_ack(soft_i2c_t* soft_i2c)
{
#if PLATFORM_STM32
    uint8_t re;

    SOFT_I2C_SETBIT(soft_i2c->sda_gpio, soft_i2c->sda_pin); /* CPU释放SDA总线 */
    soft_i2c_delay();
    SOFT_I2C_SETBIT(soft_i2c->scl_gpio,
                    soft_i2c->scl_pin); /* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    soft_i2c_delay();

    /* CPU读取SDA口线状态 */
    if (SOFT_I2C_READ(soft_i2c->sda_gpio, soft_i2c->scl_pin)) { re = 1; }
    else {
        re = 0;
    }
    SOFT_I2C_RESETBIT(soft_i2c->scl_gpio, soft_i2c->scl_pin);
    soft_i2c_delay();
    return re;
#elif PLATFORM_MSPM0
    uint32_t timeout = 0;
    i2c_sda_high();
    i2c_sda_in(soft_i2c);
    soft_i2c_delay();
    i2c_scl_high();
    soft_i2c_delay();
    while (i2c_read_sda()) {
        timeout++;
        if (timeout > 100)   // 2000
        {
            soft_i2c_stop(soft_i2c);
            return 1;
        }
    }
    i2c_scl_low();
    soft_i2c_delay();
    return 0;
#endif
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void soft_i2c_ack(soft_i2c_t* soft_i2c)
{

#if PLATFORM_STM32

    SOFT_I2C_RESETBIT(soft_i2c->sda_gpio, soft_i2c->sda_pin); /* CPU驱动SDA = 0 */
    soft_i2c_delay();
    SOFT_I2C_SETBIT(soft_i2c->scl_gpio, soft_i2c->scl_pin); /* CPU产生1个时钟 */
    soft_i2c_delay();
    SOFT_I2C_RESETBIT(soft_i2c->scl_gpio, soft_i2c->scl_pin);
    soft_i2c_delay();
    SOFT_I2C_SETBIT(soft_i2c->sda_gpio, soft_i2c->sda_pin); /* CPU释放SDA总线 */
#elif PLATFORM_MSPM0
    /*           ____
     *SCL ______/    \______
     *    ____         _____
     *SDA     \_______/
     */
    i2c_scl_low();
    i2c_sda_out(soft_i2c);
    i2c_sda_high();
    soft_i2c_delay();
    i2c_sda_low();
    soft_i2c_delay();
    i2c_scl_high();
    soft_i2c_delay();
    i2c_scl_low();
    soft_i2c_delay();
    i2c_sda_high();
#endif
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void soft_i2c_nack(soft_i2c_t* soft_i2c)
{
#if PLATFORM_STM32
    SOFT_I2C_SETBIT(soft_i2c->sda_gpio, soft_i2c->sda_pin); /* CPU驱动SDA = 1 */
    soft_i2c_delay();
    SOFT_I2C_SETBIT(soft_i2c->scl_gpio, soft_i2c->scl_pin); /* CPU产生1个时钟 */
    soft_i2c_delay();
    SOFT_I2C_RESETBIT(soft_i2c->scl_gpio, soft_i2c->scl_pin);
    soft_i2c_delay();
#elif PLATFORM_MSPM0
    /*           ____
     *SCL ______/    \______
     *    __________________
     *SDA
     */
    i2c_scl_low();
    i2c_sda_out(soft_i2c);
    i2c_sda_high();
    soft_i2c_delay();
    i2c_scl_high();
    soft_i2c_delay();
    i2c_scl_low();
    soft_i2c_delay();
#endif
}

/*
*********************************************************************************************************
*	函 数 名: i2c_GPIO_Config
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void soft_i2c_init(soft_i2c_t* soft_i2c)
{

#ifdef __STM32F10x_H
    // stm32固件库才有的初始化

    GPIO_InitTypeDef GPIO_InitStructure;
    // 打开SCL的GPIO时钟
    RCC_APB2PeriphClockCmd(soft_i2c->scl_rcc, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = soft_i2c->scl_pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;   // 开漏输出
    GPIO_Init(soft_i2c->scl_gpio, &GPIO_InitStructure);
    // 打开SDA的GPIO时钟
    RCC_APB2PeriphClockCmd(soft_i2c->sda_rcc, ENABLE);
    GPIO_InitStructure.GPIO_Pin = soft_i2c->sda_pin;
    GPIO_Init(soft_i2c->sda_gpio, &GPIO_InitStructure);
#endif

#if PLATFORM_STM32

    /* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
    soft_i2c_stop(soft_i2c);
#elif PLATFORM_MSPM0

    // delay_ms(10);

    soft_i2c_stop(soft_i2c);
#endif
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
uint8_t soft_i2c_check_device(soft_i2c_t* soft_i2c, uint8_t address)
{

    uint8_t ack;

    soft_i2c_init(soft_i2c); /* 配置GPIO */

    soft_i2c_start(soft_i2c); /* 发送启动信号 */

    /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
    soft_i2c_send_byte(soft_i2c, address | SOFT_I2C_WR);
    ack = soft_i2c_wait_ack(soft_i2c); /* 检测设备的ACK应答 */

    soft_i2c_stop(soft_i2c); /* 发送停止信号 */

    return ack;
}
