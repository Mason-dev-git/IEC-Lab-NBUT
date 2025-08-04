#ifndef WT901C_H
#define WT901C_H
#include "main.h"
#include <string.h>
#include <stdint.h>

// 宏定义：命令包长度（WT901C配置命令固定5字节）
#define WT901_CMD_LEN 5U
// 宏定义：延迟时间（单位：ms，根据传感器手册推荐值设置）
#define DELAY_SHORT 100U    // 普通命令间隔
#define DELAY_CALIB 2000U   // 加速度校准等待时间（必须足够长）
#define DELAY_SET 500U      // 参数设置后等待生效时间

// WT901C配置命令（每个命令5字节，格式：0xFF 0xAA [指令] [参数] [校验和]）
// 基础初始化命令
// --------------------------
// 基础配置命令索引
// --------------------------
#define INIT_CMD_UNLOCK 0     // 解锁命令索引
#define INIT_CMD_XY_REF 1     // 设置X/Y轴参考索引
#define INIT_CMD_Z_REF 2      // 设置Z轴参考索引
#define INIT_CMD_SAVE 3       // 保存配置索引
#define target_RX_date 0x53   // 目标第二节字节
// 基础配置命令集合（与上面的索引一一对应）
static const uint8_t gyr_init_cmds[][WT901_CMD_LEN] = {
    {0xFF, 0xAA, 0x69, 0x88, 0xB5},   // [INIT_CMD_UNLOCK] 解锁命令
    {0xFF, 0xAA, 0x01, 0x08, 0x00},   // [INIT_CMD_XY_REF] 设置X/Y轴角度参考
    {0xFF, 0xAA, 0x01, 0x04, 0x00},   // [INIT_CMD_Z_REF] 设置Z轴参考（六轴模式）
    {0xFF, 0xAA, 0x00, 0x00, 0x00}    // [INIT_CMD_SAVE] 保存配置
};

// --------------------------
// 调试命令索引（调试模式）
// --------------------------
#ifdef DEBUG
#    define DBG_CMD_UNLOCK 0       // 解锁命令索引
#    define DBG_CMD_ACC_CALIB 1    // 加速度校准索引
#    define DBG_CMD_END_CALIB 2    // 结束校准索引
#    define DBG_CMD_RATE_100HZ 3   // 输出速率100Hz索引
#    define DBG_CMD_OUTPUT_CFG 4   // 输出内容配置索引
#    define DBG_CMD_SAVE 5         // 保存配置索引
// 调试命令集合（与上面的索引一一对应）
static const uint8_t gyr_debug_cmds[][WT901_CMD_LEN] = {
    {0xFF, 0xAA, 0x69, 0x88, 0xB5},   // [DBG_CMD_UNLOCK] 解锁命令
    {0xFF, 0xAA, 0x01, 0x01, 0x00},   // [DBG_CMD_ACC_CALIB] 加速度校准（需静置）
    {0xFF, 0xAA, 0x01, 0x00, 0x00},   // [DBG_CMD_END_CALIB] 结束校准
    {0xFF, 0xAA, 0x03, 0x09, 0x00},   // [DBG_CMD_RATE_100HZ] 输出速率100Hz
    {0xFF, 0xAA, 0x02, 0x3E, 0x00},   // [DBG_CMD_OUTPUT_CFG] 输出内容：加速度+角速度+角度
    {0xFF, 0xAA, 0x00, 0x00, 0x00}    // [DBG_CMD_SAVE] 保存配置
};
#endif


// 接收状态机
typedef enum
{
    WAIT_HEAD,
    WAIT_TYPE,
    WAIT_DATA,
    WAIT_CHECKSUM,
} GyroReceiveStatus;


// WT901C数据帧类型定义（协议详见维特官网）
typedef enum
{
    FRAME_TIME = 0x50,    // 时间帧 (55 50)
    FRAME_ACCEL = 0x51,   // 加速度帧 (55 51)
    FRAME_ANGUL = 0x52,   // 角速度帧 (55 52)
    FRAME_ANGLE = 0x53,   // 角度帧 (55 53)
    // FRAME_MAG = 0x54,      // 磁力计帧 (55 54)
    // FRAME_PORT = 0x55,     // 端口状态帧 (55 55)
    // FRAME_HIGH = 0x56,     // 气压高度帧 (55 56)
    // FRAME_LATLNG = 0x57,   // 经纬度帧 (55 57)
    // FRAME_GPS = 0x58,      // GPS帧 (55 58)
    // FRAME_QUATER = 0x59,   // 四元数帧 (55 59)
    // FRAME_GPSACC = 0x5A,   // GPS加速度帧 (55 5A)
    // FRAME_REG = 0x5F,      // 寄存器帧 (55 5F)
} Wt901cFrameType;


// 直接暴露数据，方便拿取
extern uint8_t frame_buffer[11];
extern uint8_t GyroFrame[6];
void wt901c_init(void);
void WT901C_IRQHandler(void);   // 中断回调函数：逐字节接收数据并组帧解析

void parse_angle_frame(float* data);   // 解析角度帧
float get_angle_roll(void);            // 单独获取滚转角
float get_angle_pitch(void);           // 单独获取俯仰角
float get_angle_yaw(void);             // 单独获取偏航角

#endif   // !WT901C_H
