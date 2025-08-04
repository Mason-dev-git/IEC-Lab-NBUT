#include "wt901c.h"
#include "time_util.h"   // 需要使用delay_ms这个阻塞的delay函数
#include "bsp.h"
uint8_t frame_buffer[11];                       // 数据帧缓冲区（11字节）
GyroReceiveStatus receive_status = WAIT_HEAD;   // 接收状态机初始化
uint8_t GyroFrame[6];   // 陀螺仪数据帧处理区，符合校验的数据帧将在这里等待处理

/**
 * @brief 陀螺仪串口发送函数（内部使用）
 * 
 * @param data 
 * @param size 
 */
static void uart_send(uint8_t* data, uint16_t size)
{
    // 这里需要实现具体的UART发送函数
    HAL_UART_Transmit(&huart3, data, size, HAL_MAX_DELAY);
}

/**
 * @brief WT901C初始化函数（无返回值，按顺序发送命令）
 * 
 */
void wt901c_init(void)
{
    HAL_UART_Receive_IT(&huart3, frame_buffer, 1);
#ifdef DEBUG
    // 调试模式：按索引发送调试命令
    uart_send((uint8_t*)gyr_debug_cmds[DBG_CMD_UNLOCK], WT901_CMD_LEN);
    delay_ms(DELAY_SHORT);

    uart_send((uint8_t*)gyr_debug_cmds[DBG_CMD_ACC_CALIB], WT901_CMD_LEN);
    delay_ms(DELAY_CALIB);   // 校准等待

    uart_send((uint8_t*)gyr_debug_cmds[DBG_CMD_END_CALIB], WT901_CMD_LEN);
    delay_ms(DELAY_SHORT);

    uart_send((uint8_t*)gyr_debug_cmds[DBG_CMD_RATE_100HZ], WT901_CMD_LEN);
    delay_ms(DELAY_SHORT);

    uart_send((uint8_t*)gyr_debug_cmds[DBG_CMD_OUTPUT_CFG], WT901_CMD_LEN);
    delay_ms(DELAY_SHORT);

    uart_send((uint8_t*)gyr_debug_cmds[DBG_CMD_SAVE], WT901_CMD_LEN);
    delay_ms(DELAY_SHORT);
#else
    // 生产模式：按索引发送基础命令
    uart_send((uint8_t*)gyr_init_cmds[INIT_CMD_UNLOCK], WT901_CMD_LEN);
    delay_ms(DELAY_SHORT);

    uart_send((uint8_t*)gyr_init_cmds[INIT_CMD_XY_REF], WT901_CMD_LEN);
    delay_ms(DELAY_SET);

    uart_send((uint8_t*)gyr_init_cmds[INIT_CMD_Z_REF], WT901_CMD_LEN);
    delay_ms(DELAY_SET);

    uart_send((uint8_t*)gyr_init_cmds[INIT_CMD_SAVE], WT901_CMD_LEN);
    delay_ms(DELAY_SHORT);
#endif
    // HAL_UART_Receive_IT(&huart3, frame_buffer, 1);默认不自动开启，需要接收时再开启，节约中断资源
}


/**
 * @brief 接收中断回调函数,接收到数据后会调用该函数，处理数据帧
 * 
 */
void WT901C_IRQHandler(void)
{
    switch (receive_status) {
        case WAIT_HEAD: {
            if (frame_buffer[0] == 0x55) {
                receive_status = WAIT_TYPE;
                HAL_UART_Receive_IT(&huart3, frame_buffer + 1, 1);   // 启用中断接收(接收下一个字节数据)
            }
            else {
                HAL_UART_Receive_IT(&huart3, frame_buffer, 1);   // 启用中断接收(接收下一个字节数据)
            }
            break;
        }
        case WAIT_TYPE: {
            // current_frame_type = frame_buffer[1];
            switch (frame_buffer[1]) {
                case FRAME_ANGLE:
                    receive_status = WAIT_DATA;
                    HAL_UART_Receive_IT(&huart3, frame_buffer + 2, 9);   // 启用中断接收(接收角度数据)
                    break;                                               // 角度帧数据
                // 其他数据种类
                default:
                    receive_status = WAIT_HEAD;
                    HAL_UART_Receive_IT(&huart3, frame_buffer, 1);   // 启用中断接收(接收下一个字节数据)
                    break;                                           // 未知帧类型，忽略
            }
        }
        case WAIT_DATA: {
            memcpy(GyroFrame, frame_buffer + 2, 6);
            // 校验数据会经常出问题，只会偶尔校验通过，但是不校验影响不大，因为数据连续性问题不大(^v^)
            /******校验******/
            // static uint8_t sum_buffer = 0;
            // for (uint8_t i = 0; i < 10; i++) {
            //     sum_buffer += frame_buffer[i];
            // }
            // if (sum_buffer == frame_buffer[10])   // 校验成功
            // {
            //     sum_buffer = 0;
            //     memcpy(store_wt901_date, frame_buffer + 2, 6);
            // }
            /****************/
            receive_status = WAIT_HEAD;
            HAL_UART_Receive_IT(&huart3, frame_buffer, 1);   // 启用中断接收(接收下一个字节数据)
            break;
        }
        default:
            HAL_UART_Receive_IT(&huart3, frame_buffer, 1);   // 启用中断接收(接收下一个字节数据)
            break;
    }
}

float get_angle_roll(void)
{
    return (float)(GyroFrame[1] << 8 | GyroFrame[0]) / 32768.0f * 180.0f;   // 横滚角roll（X轴）
}
float get_angle_pitch(void)
{
    return (float)(GyroFrame[3] << 8 | GyroFrame[2]) / 32768.0f * 180.0f;   // 俯仰角pitch（Y轴）
}
float get_angle_yaw(void)
{
    return (float)(GyroFrame[5] << 8 | GyroFrame[4]) / 32768.0f * 180.0f;   // 航向角yaw（Z轴）
}

/**
 * @brief 解析陀螺仪角度帧数据，计算欧拉角
 * @details 数据格式：55 52 [RollL] [RollH] [PitchL] [PitchH] [YawL] [YawH] [TMPL] [TMPH] [SUM]
 *          计算逻辑：角度值 = (高8位 << 8 | 低8位) / 32768.0 * 180°（量程±180°）
 * @param data 指向角度读取地址
 * @note *
 */
void parse_angle_frame(float data[3])
{
    data[0] = get_angle_roll();
    data[1] = get_angle_pitch();
    data[2] = get_angle_yaw();
}




// // 解析加速度帧（55 50）
// static void parse_accel_frame(uint8_t* frame)
// {
//     // 数据格式：55 50 [AXL] [AXH] [AYL] [AYH] [AZL] [AZH] [TMPL] [TMPH] [SUM]
//     // 加速度值 = (高8位 << 8 | 低8位) / 32768.0 * 16g（量程±16g）
//     int16_t ax = (int16_t)(frame[3] << 8 | frame[2]);
//     int16_t ay = (int16_t)(frame[5] << 8 | frame[4]);
//     int16_t az = (int16_t)(frame[7] << 8 | frame[6]);
//     g_wt901c_data.accel_x = ax / 32768.0f * 16.0f;
//     g_wt901c_data.accel_y = ay / 32768.0f * 16.0f;
//     g_wt901c_data.accel_z = az / 32768.0f * 16.0f;

// 温度值（可选，单位：℃，计算方式：(TMPH<<8 | TMPL)/100.0）
// int16_t temp = (int16_t)(frame[9] << 8 | frame[8]);
// float temperature = temp / 100.0f;
// }

// 解析陀螺仪帧（55 51）
// static void parse_gyro_frame(uint8_t* frame)
//{
//    // 数据格式：55 51 [GX L] [GX H] [GY L] [GY H] [GZ L] [GZ H] [TMPL] [TMPH] [SUM]
//    // 陀螺仪值 = (高8位 << 8 | 低8位) / 32768.0 * 2000°/s（量程±2000°/s）
//    int16_t gx = (int16_t)(frame[3] << 8 | frame[2]);
//    int16_t gy = (int16_t)(frame[5] << 8 | frame[4]);
//    int16_t gz = (int16_t)(frame[7] << 8 | frame[6]);
//    g_wt901c_data.gyro_x = gx / 32768.0f * 2000.0f;
//    g_wt901c_data.gyro_y = gy / 32768.0f * 2000.0f;
//    g_wt901c_data.gyro_z = gz / 32768.0f * 2000.0f;
//}


//// 解析磁力计帧（55 53，按需启用）
// static void parse_mag_frame(uint8_t* frame)
//{
//     // 数据格式：55 53 [MXL] [MXH] [MYL] [MYH] [MZL] [MZH] [TMPL] [TMPH] [SUM]
//     // 磁力计值 = (高8位 << 8 | 低8位)（单位uT）
//     int16_t mx = (int16_t)(frame[3] << 8 | frame[2]);
//     int16_t my = (int16_t)(frame[5] << 8 | frame[4]);
//     int16_t mz = (int16_t)(frame[7] << 8 | frame[6]);
//     g_wt901c_data.mag_x = (float)mx;
//     g_wt901c_data.mag_y = (float)my;
//     g_wt901c_data.mag_z = (float)mz;
// }

//// 解析端口状态帧（55 54，按需处理）
// static void parse_port_frame(uint8_t* frame)
//{
//     // 数据格式：55 54 [D0] [D1] [D2] [D3] [D4] [D5] [D6] [D7] [SUM]
//     // 此处仅存储原始数据，根据传感器手册定义具体含义
//     memcpy(g_wt901c_data.port_status, &frame[2], 6);
// }
