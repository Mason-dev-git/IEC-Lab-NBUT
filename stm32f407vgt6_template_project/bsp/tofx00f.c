#include "tofx00f.h"
#include "math_util.h"
#include <stdint.h>
#include <stdio.h>


#define TOFXXF_CMD_READ 0x03
#define TOFXXF_CMD_WRITE 0x06
#define TOFXXF_REG_SPECIAL 0x0001

UART_HandleTypeDef* g_tofxxf_huart = NULL;

// 读测量距离命令
#define REG_RD_ADDR 0x0010   // 测量结果寄存器

#define BUFFER_SIZE 32
volatile int     flag = 0;
volatile u8 g_buffer[BUFFER_SIZE];
uint16_t g_buffer_length;
volatile bool    g_ok_flag = false;

void tofxxf_init(UART_HandleTypeDef* huart)
{
    g_tofxxf_huart = huart;
}


// 中断解析数据
void tofxxf_irq_handler(void)
{
    g_ok_flag = true;
    g_buffer_length = sizeof(g_buffer) - __HAL_DMA_GET_COUNTER(g_tofxxf_huart->hdmarx);
}

// 请求获取测量值
void tofxxf_requst(void)
{
    g_buffer_length = 0;
    // 开dma中断
    HAL_UART_Receive_DMA(g_tofxxf_huart, g_buffer, BUFFER_SIZE);
}

// 获取测量值
int32_t tofxxf_get_distance(void)
{
    if(!g_ok_flag || g_buffer_length < 8){
        return -1;
    }
    debug_print("buf size = %d\n", g_buffer_length);
    for(int i = 0 ; i < g_buffer_length; i++){
         debug_print("%02X ", g_buffer[i]);
    }
    debug_print("\n");
    for(int i = 0 ; i < g_buffer_length - 7; i++){
        // 01 03 02 00 00 B8 44 
        // 如果不是01 03 02那么就跳过
        if(g_buffer[i] == 0x01 && g_buffer[i+1] == 0x03 && g_buffer[i+2] == 0x02){
            return (int32_t)((((u16)g_buffer[i+3]) << 8) | (((u16)g_buffer[i+4])));
        }
    }
    
    return -1;
}


#if 0 

// 发送命令
static inline void tofxxf_send_cmd(uint8_t* cmd, uint8_t len)
{
    HAL_UART_Transmit(g_tofxxf_huart, cmd, len, 1000);
}

void modbus_send_data(uint8_t* cmd, uint8_t len)
{
    HAL_UART_Transmit(g_tofxxf_huart, cmd, len, 1000);
}

void modbus_recv_data(uint8_t* buf, uint8_t len)
{
    HAL_UART_Receive(g_tofxxf_huart, buf, len, 1000);
}

// 往特定moddus从机的寄存器内写入某个值
bool modbus_write_reg(uint8_t address, uint16_t reg, uint16_t value, bool use_check)
{
    uint8_t  cmd[8] = {address,
                       TOFXXF_CMD_WRITE,
                       BITS_GET_HIGH_BIT_IN_WORD(reg),
                       BITS_GET_LOW_BIT_IN_WORD(reg),
                       BITS_GET_HIGH_BIT_IN_WORD(value),
                       BITS_GET_LOW_BIT_IN_WORD(value)};
    uint16_t crc16  = crc16_modbus_calculate(cmd, 6);
    cmd[6]          = BITS_GET_LOW_BIT_IN_WORD(crc16);
    cmd[7]          = BITS_GET_HIGH_BIT_IN_WORD(crc16);

    modbus_send_data(cmd, 8);

    if (use_check) {
        // 对于回发消息进行校验
        uint8_t result[8];
        modbus_recv_data(result, 8);
        for (uint8_t i = 0; i < 8; i++) {
            if (result[i] != cmd[i]) {
                return false;
            }
        }
    }

    return true;
}

// 读取一个寄存器的值
bool modbus_read_reg(uint8_t address, uint16_t reg, uint16_t value) {
    return false;
}


// 初始化tofxxf
void tofxxf_init(UART_HandleTypeDef* huart)
{
    g_tofxxf_huart = huart;
}

// 扫描tofxxf设备，通过变量返回设备地址，如果成功则address就是地址，且返回true。失败返回false，且address不会写入任何值
bool tofxxf_scan_device(uint8_t* address)
{
    // 通过广播帧来扫描设备
    // uint8_t  cmd[8] = {0x01,
    //                    TOFXXF_CMD_WRITE,
    //                    GET_HIGH_BIT_IN_WORD(TOFXXF_REG_SPECIAL),
    //                    GET_LOW_BIT_IN_WORD(TOFXXF_REG_SPECIAL),
    //                    GET_HIGH_BIT_IN_WORD(test_data),
    //                    GET_LOW_BIT_IN_WORD(test_data)};
    // uint16_t crc16  = crc16_modbus_calculate(cmd, 6);
    // cmd[6]          = GET_LOW_BIT_IN_WORD(crc16);
    // cmd[7]          = GET_HIGH_BIT_IN_WORD(crc16);

    // tofxxf_send_cmd(cmd, 8);
    return true;

    // // 测试通信使用的数据
    // const uint16_t test_data = 0x0000;
    // for (uint16_t try_addr = 1; try_addr <= 0xff; try_addr++) {
    //     uint8_t cmd[8] = {
    //         try_addr & 0xff,
    //         TOFXXF_CMD_WRITE,
    //         GET_HIGH_BIT_IN_WORD(TOFXXF_REG_SPECIAL),
    //         GET_LOW_BIT_IN_WORD(TOFXXF_REG_SPECIAL),
    //         GET_HIGH_BIT_IN_WORD(test_data),
    //         GET_LOW_BIT_IN_WORD(test_data)
    //     };
    //     uint16_t crc16 = crc16_modbus_calculate(cmd, 6);
    //     cmd[6] = GET_LOW_BIT_IN_WORD(crc16);
    //     cmd[7] = GET_HIGH_BIT_IN_WORD(crc16);

    //     tofxxf_send_cmd(cmd, 8);
    // }
}

// 恢复默认参数
void tofxxf_reset(void)
{
    uint8_t TOFXXF_RESET_CMD[] = {0x01, 0x06, 0x02, 0x06, 0x00, 0x01, 0x10, 0x00, 0xD5, 0xF9};
    tofxxf_send_cmd(TOFXXF_RESET_CMD, sizeof(TOFXXF_RESET_CMD));
}
// 重启设备
void tofxxf_restart(void) {}
// 测试通讯
void tofxxf_test(void) {}
// 设置量程模式，0为默认，1为高精度，2位长距离，3为高速
void tofxxf_set_range_mode(uint8_t mode) {}

#endif
