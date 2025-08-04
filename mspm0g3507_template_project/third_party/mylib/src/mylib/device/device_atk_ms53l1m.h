//
// ATK_MS53L1M高性能激光测距模块
// 使用方法:
//      1. 使用HAL库初始化好一个串口
//      2. 在初始化好串口以后，调用atk_ms53l1m_init，把串口句柄的地址传入
//      3. 在UARTx_IRQHandler中断服务例程里调用atk_ms53l1m_uart_iqr_handler
//      4. 调用atk_ms53l1m_normal_get_data来获取数据
//

// 例子：
#if 0
    // 初始化ATK-MS53L1M
    ret = atk_ms53l1m_init(&huart2, &id);
    if (ret == ATK_MS53L1M_ERROR) {
        DEBUG_INFO("ATK-MS53L1M init failed!");
        return 0;
    }
    DEBUG_INFO("ATK-MS53L1M id = %d", id);

    uint16_t dat;
    while (1) {      
        ret = atk_ms53l1m_modbus_get_data(id, &dat);
        if (ret == 0) { 
            DEBUG_INFO("[Modbus]Distance: %dmm", dat); 
        }
        else {
            DEBUG_INFO("Modbus mode get data failed!");
        }

        HAL_Delay(500);
    }
#endif

#ifndef MYLIB_DEVICE_ATK_MS53L1M_H
#    define MYLIB_DEVICE_ATK_MS53L1M_H

#    include <mylib/mylib_config.h>
#    include <stdint.h>

// 错误代码
#    define ATK_MS53L1M_EOK 0      /* 没有错误 */
#    define ATK_MS53L1M_ERROR 1    /* 错误 */
#    define ATK_MS53L1M_ETIMEOUT 2 /* 超时错误 */
#    define ATK_MS53L1M_EFRAME 3   /* 帧错误 */
#    define ATK_MS53L1M_ECRC 4     /* CRC校验错误 */
#    define ATK_MS53L1M_EOPT 5     /* 操作错误 */

// 可以调用的函数
// 所有函数的返回值为错误代码，如果为ATK_MS53L1M_EOK就说明调用成功

// 初始化，其中huart为串口的句柄，需要变量存储设备id
#    if PLATFORM_STM32
uint8_t atk_ms53l1m_init(UART_HandleTypeDef* huart, uint16_t* id);
#    elif PLATFORM_MSPM0
uint8_t atk_ms53l1m_init(UART_Regs* huart, uint16_t* id);
#    endif
// 串口获取数据，其中dat是用于存放数据的地方，因为默认是modbus模式，所以暂时不可用
// uint8_t atk_ms53l1m_normal_get_data(uint16_t *dat);
// Modbus的方式获取数据，需要设备id号
uint8_t atk_ms53l1m_modbus_get_data(uint16_t id, uint16_t* dat);
// 中断处理函数
void atk_ms53l1m_uart_irq_handler(void);

#endif   // !MYLIB_DEVICE_ATK_MS53L1M_H
