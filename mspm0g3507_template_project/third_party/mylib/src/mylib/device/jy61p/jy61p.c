#include <device/jy61p/jy61p.h>
#include <device/jy61p/wit_c_sdk.h>
#include <driver/driver_systick.h>
#include <driver/driver_soft_i2c.h>

#define ACC_UPDATE 0x01
#define GYRO_UPDATE 0x02
#define ANGLE_UPDATE 0x04
#define MAG_UPDATE 0x08
#define READ_UPDATE 0x80

static volatile char s_cDataUpdate = 0, s_cCmd = 0xff;

static float fAcc[3], fGyro[3], fAngle[3];

static soft_i2c_t g_i2c;


static void AutoScanSensor(void);
static void CopeSensorData(uint32_t uiReg, uint32_t uiRegNum);

static int32_t IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t* data, uint32_t length)
{
    // 第1个参数为I2C操作句柄
    // 第2个参数为从机设备地址
    // 第3个参数为从机寄存器地址
    // 第4个参数为从机寄存器地址长度
    // 第5个参数为发送的数据的起始地址
    // 第6个参数为传输数据的大小
    // 第7个参数为操作超时时间
    // HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c1, dev, reg, 8, data, length, 0x20);
    // return HAL_OK == status ? 1 : 0;
    return 0;
}

static int32_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t* data, uint32_t length)
{
    // HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, dev, reg, 8, data, length, 0x20);
    // return HAL_OK == status ? 1 : 0
    //    for(int i = 0 ; i < length; i++){
    //        soft_i2c_send_byte(g_i2c,data);
    //    }

    return 0;
}

static inline void jy61p_delay_ms(uint16_t ucMs)
{
#if PLATFORM_STM32
    HAL_Delay(ucMs);
#elif PLATFORM_MSPM0
    systick_delay_ms(ucMs);
#endif
}

// 自动扫描iic连接的jy61p设备
static void AutoScanSensor(void)
{
    int i, iRetry;

    for (i = 0; i < 0x7F; i++) {
        WitInit(WIT_PROTOCOL_I2C, i);
        iRetry = 2;
        do {
            s_cDataUpdate = 0;
            WitReadReg(AX, 3);
            jy61p_delay_ms(5);   // 延迟5ms
            if (s_cDataUpdate != 0) {
                // printf("find %02X addr sensor\r\n", i);
                // ShowHelp();
                return;
            }
            iRetry--;
        } while (iRetry);
    }
    // printf("can not find sensor\r\n");
    // printf("please check your connection\r\n");
}

// 处理传感器数据
static void CopeSensorData(uint32_t uiReg, uint32_t uiRegNum)
{
    int i;
    for (i = 0; i < uiRegNum; i++) {
        switch (uiReg) {
            //            case AX:
            //            case AY:
        case AZ:
            s_cDataUpdate |= ACC_UPDATE;
            break;
            //            case GX:
            //            case GY:
        case GZ:
            s_cDataUpdate |= GYRO_UPDATE;
            break;
            //            case HX:
            //            case HY:
        case HZ:
            s_cDataUpdate |= MAG_UPDATE;
            break;
            //            case Roll:
            //            case Pitch:
        case Yaw: s_cDataUpdate |= ANGLE_UPDATE; break;
        default: s_cDataUpdate |= READ_UPDATE; break;
        }
        uiReg++;
    }
}

// 初始化jy61p
void jy61p_init(void)
{
    // 初始化iic协议，设置设备地址
    WitInit(WIT_PROTOCOL_I2C, JY61P_DEFAULT_ADDRESS);
    // 注册iic读写回调函数
    WitI2cFuncRegister(IICwriteBytes, IICreadBytes);
    // 注册数据处理回调函数
    WitRegisterCallBack(CopeSensorData);
    // 注册ms延迟的回调函数
    WitDelayMsRegister(jy61p_delay_ms);
    //自动搜索传感器
    AutoScanSensor();
}

// 从jy61p获取数据，调用以后会把数据解析并且计算好，之后存储在内部
void jy61p_get_data(void)
{
    // 从寄存器里面读取数据
    WitReadReg(AX, 12);

    // 数据有更新才重新计算
    if (s_cDataUpdate) {
        for (int i = 0; i < 3; i++) {

            fGyro[i]  = sReg[GX + i] / 32768.0f * 2000.0f;
            fAngle[i] = sReg[Roll + i] / 32768.0f * 180.0f;
        }
        if (s_cDataUpdate & ACC_UPDATE) {
            // printf("acc:%.3f %.3f %.3f\r\n", fAcc[0], fAcc[1], fAcc[2]);
            s_cDataUpdate &= ~ACC_UPDATE;
        }
        if (s_cDataUpdate & GYRO_UPDATE) {
            // printf("gyro:%.3f %.3f %.3f\r\n", fGyro[0], fGyro[1], fGyro[2]);
            s_cDataUpdate &= ~GYRO_UPDATE;
        }
        if (s_cDataUpdate & ANGLE_UPDATE) {
            // printf("angle:%.3f %.3f %.3f\r\n", fAngle[0], fAngle[1], fAngle[2]);
            s_cDataUpdate &= ~ANGLE_UPDATE;
        }
    }
}

// 从内部获取三维加速度数据，参数为用于存储数据的变量
void jy61p_get_acc(float* ax, float* ay, float* az)
{
    WitReadReg(AX, 1);
    WitReadReg(AY, 1);
    WitReadReg(AZ, 1);

    // 加速度计算公式：
    // ax=((AxH<<8)|AxL)/32768*16g(g为重力加速度，可取9.8m/s2)
    // ay=((AyH<<8)|AyL)/32768*16g(g为重力加速度，可取9.8m/s2)
    // az=((AzH<<8)|AzL)/32768*16g(g为重力加速度，可取9.8m/s2)
    *ax = sReg[0] / 32768.0f * 16.0f * 9.8f;
    *ay = sReg[1] / 32768.0f * 16.0f * 9.8f;
    *az = sReg[2] / 32768.0f * 16.0f * 9.8f;
}

// 从内部获取三维角速度数据，参数为用于存储数据的变量
void jy61p_get_gyro(float* gx, float* gy, float* gz)
{
    WitReadReg(GX, 1);
    WitReadReg(GY, 1);
    WitReadReg(GZ, 1);

    *gx = sReg[0] / 32768.0f * 2000.0f;
    *gy = sReg[1] / 32768.0f * 2000.0f;
    *gz = sReg[2] / 32768.0f * 2000.0f;
}

// 从内部获取三维角度数据，参数为用于存储数据的变量
void jy61p_get_angle(float* ax, float* ay, float* az)
{
    WitReadReg(Roll, 1);
    WitReadReg(Pitch, 1);
    WitReadReg(Yaw, 1);

    *ax = sReg[0] / 32768.0f * 180.0f;
    *ay = sReg[1] / 32768.0f * 180.0f;
    *az = sReg[2] / 32768.0f * 180.0f;
}

// 获取四元数，要求传入一个至少为4个元素的数组
void jy61p_get_q(int16_t* q)
{
    WitReadReg(q0, 1);
    WitReadReg(q1, 1);
    WitReadReg(q2, 1);
    WitReadReg(q3, 1);

    q[0] = sReg[q0];
    q[1] = sReg[q1];
    q[2] = sReg[q2];
    q[3] = sReg[q3];
}
