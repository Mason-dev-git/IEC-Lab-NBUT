#include "jy61p.h"


UART_Regs* g_huart = NULL;
uint8_t    uart3_rx_byte,   //单个接收字节
    data_p,                 // 1为接收角加速度，2为接收加速度，3为接收角度
    byte_num,               //用于自增接收数据
    acc[10],                //接收角加速度的数据包
    gyro[10],               //接收角速度的数据包
    angle[10];              //接收角度的数据包

uint8_t state = 1;   //状态机
//碰到55 51这两个字节后，就开始连续接收9个字节，才能回到上一个状态，就是重新判断是否55
// 51这两个字节的状态 state=1：判断前缀字符55，若是则进入state=2
// state=2：判断第二字符，是否为51、52、53这些，若是则进入state=3，反之则回到state=1；
// state=3：开始接收9个字节，接收完成回到state=1

int16_t Acc, Gyro, Angle;   //暂存解析的数据
double  ACC[3], GYRO[3], ANGLE[3];   //第一个是X轴数据，第二个是Y轴数据，第三个是Z轴数据
float   temperature;


// 初始化jy61p
void jy61p_init(UART_Regs* huart)
{
    g_huart = huart;

    // 解除休眠模式
    //    DL_UART_transmitDataBlocking(g_huart, 0xFF);
    //    DL_UART_transmitDataBlocking(g_huart, 0xAA);
    //    DL_UART_transmitDataBlocking(g_huart, 0x60);

    // 加速度校准
    //    DL_UART_transmitDataBlocking(g_huart, 0xFF);
    //    DL_UART_transmitDataBlocking(g_huart, 0xAA);
    //    DL_UART_transmitDataBlocking(g_huart, 0x67);
}

// 从jy61p获取数据，调用以后会把数据解析并且计算好，之后存储在内部
void jy61p_get_data(void)
{
    for (uint8_t i = 0; i < 3; i++) {
        Acc   = (acc[2 * i + 1] << 8) | acc[2 * i];
        Gyro  = (gyro[2 * i + 1] << 8) | gyro[2 * i];
        Angle = (angle[2 * i + 1] << 8) | angle[2 * i];

        ACC[i]   = ((double)Acc) / 32767 * 16;      //单位为G，可取1G=9.8m/s
        GYRO[i]  = ((double)Gyro) / 32767 * 2000;   //单位为度每秒
        ANGLE[i] = ((double)Angle) / 32767 * 180;   //单位为度
                                                    //        DEBUG_INFO("ACC[i]=%.3f", ACC[i]);
                                                    //        DEBUG_INFO("GYRO[i]=%.3f", GYRO[i]);
                                                    //        DEBUG_INFO("ANGLE[i]=%.3f", ANGLE[i]);
    }
}

// 从内部获取三维加速度数据，参数为用于存储数据的变量
void jy61p_get_acc(float* ax, float* ay, float* az)
{
    *ax = ACC[0];
    *ay = ACC[1];
    *az = ACC[2];
}

// 从内部获取三维角速度数据，参数为用于存储数据的变量
void jy61p_get_gyro(float* gx, float* gy, float* gz)
{
    *gx = GYRO[0];
    *gy = GYRO[1];
    *gz = GYRO[2];
}

// 从内部获取三维角度数据，参数为用于存储数据的变量
void jy61p_get_angle(float* ax, float* ay, float* az)
{
    *ax = ANGLE[0];
    *ay = ANGLE[1];
    *az = ANGLE[2];
}

// Wit协议：https://wit-motion.yuque.com/wumwnr/ltst03/wegquy
// 数据帧头0x55 类型0x51~0x54 数据低8位 数据高8位 数据低8位 数据高8位 数据低8位 数据高8位 数据低8位
// 数据高8位 SUMCRC（16位） 55 51 9F FD E8 FF A5 07 35 0B 15 加速度 55 52 00 00 00 00 00 00 35 0B E7
// 角速度 55 53 69 21 7D F8 E8 C4 82 46 1B 角度 55 54 00 00 00 00 00 00 00 00 A9 磁场

void jy61p_irq_handler(void)
{
    if (DL_UART_getPendingInterrupt(g_huart) == (DL_UART_IIDX)((uint32_t)0x0000000BU)) {
        uart3_rx_byte = DL_UART_receiveData(g_huart);
        // DEBUG_INFO("uart3_rx_byte=%d", uart3_rx_byte);
        /*状态1时接收到帧头0x55，则进入下一个状态，否则直接跳出接收中断函数*/
        if (state == 1) {
            if (uart3_rx_byte == 0x55)
                state = 2;
            else
                goto jy61p_irq_handler_exit;
        }

        /*状态2时判断第二个帧头，0x51对应角加速度数据，0x52对应角速度数据，0x53对应角度数据，
         *如果有相应的二帧头，则进入下一个状态，反之回到状态1，
         *若检测到二帧头，则根据二帧头给data_p置不同的数值，代表即将接收的是哪种数据*/
        else if (state == 2) {
            if (uart3_rx_byte == 0x51) {
                state  = 3;
                data_p = 1;
            }
            else if (uart3_rx_byte == 0x52) {
                state  = 3;
                data_p = 2;
            }
            else if (uart3_rx_byte == 0x53) {
                state  = 3;
                data_p = 3;
            }
            else {
                state  = 1;
                data_p = 0;
            }
        }

        /*状态3时，根据data_p所置的值，开始连续接收8个字节，
         *连续接收完8个字节后，如果这8个字节是属于角度(0x53)的了，则说明已经接收完包含acc、gyro、angle的一整条数据包，
         *回到状态1，并根据协议，开始对接收到的总共3×8=24个字节进行处理，得到可读的数值*/
        else if (state == 3) {
            if (data_p == 1)
                acc[byte_num++] = uart3_rx_byte;
            else if (data_p == 2)
                gyro[byte_num++] = uart3_rx_byte;
            else if (data_p == 3)
                angle[byte_num++] = uart3_rx_byte;
            /*接收完8个字节了，则回到状态1。如果是接收完一整条数据包，则开始对数据进行处理*/
            if (byte_num == 8) {
                byte_num = 0;
                state    = 1;
                if (data_p == 3) jy61p_get_data();
            }
        }
    }
jy61p_irq_handler_exit:;
}

uint8_t JY61P_ULOCK_CMD[5] = {0xFF, 0xAA, 0x69, 0x88, 0xB5};   //解锁
uint8_t JY61P_BAUD_CMD[5]  = {0xFF, 0xAA, 0x04, 0x06, 0x00};   //波特率修改为115200
uint8_t JY61P_SAVE_CMD[5]  = {0xFF, 0xAA, 0x00, 0x00, 0x00};   //保存
uint8_t JY61P_XY0_CMD[5]   = {0xFF, 0xAA, 0x01, 0x08, 0x00};   // XY角度归零
uint8_t JY61P_Z0_CMD[5]    = {0xFF, 0xAA, 0x01, 0x04, 0x00};   // Z轴归零
uint8_t JY61P_SLEEP_CMD[5] = {0xFF, 0xAA, 0x22, 0x01, 0x00};   //休眠

void jy61p_send_bytes(uint8_t* bytes, int32_t len)
{
    for (int i = 0; i < len; i++) { DL_UART_transmitData(g_huart, bytes[i]); }
}

// 将偏航角Yaw置零
void jy61p_clear_yaw(void)
{
    // 参考：https://blog.csdn.net/m0_52011717/article/details/138538530
    jy61p_send_bytes(JY61P_ULOCK_CMD, sizeof(JY61P_ULOCK_CMD));   //解锁
    delay_ms(200);                                                //延时200ms
    jy61p_send_bytes(JY61P_XY0_CMD, sizeof(JY61P_XY0_CMD));       // XY轴归零
    delay_ms(200);
    jy61p_send_bytes(JY61P_Z0_CMD, sizeof(JY61P_Z0_CMD));   // Z轴归零
    delay_ms(200);
    jy61p_send_bytes(JY61P_SAVE_CMD, sizeof(JY61P_SAVE_CMD));   //保存
    delay_ms(200);

    jy61p_send_bytes(JY61P_ULOCK_CMD, sizeof(JY61P_ULOCK_CMD));   //解锁
    delay_ms(200);                                                //延时200ms
    jy61p_send_bytes(JY61P_SLEEP_CMD, sizeof(JY61P_SLEEP_CMD));   //休眠
    delay_ms(200);
    jy61p_send_bytes(JY61P_SLEEP_CMD, sizeof(JY61P_SLEEP_CMD));   //解除休眠
    delay_ms(200);
    jy61p_send_bytes(JY61P_SAVE_CMD, sizeof(JY61P_SAVE_CMD));   //保存
    delay_ms(200);
}
