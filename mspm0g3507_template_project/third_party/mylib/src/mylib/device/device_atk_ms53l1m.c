#include <device/device_atk_ms53l1m.h>
#include <base/debug.h>
#include <driver/driver_systick.h>
#include <string.h>
#include <stdint.h>

/* ATK-MS53L1M模块功能码 */
enum
{
    ATK_MS53L1M_FUNCODE_SYS          = 0x00, /* 系统设置 */
    ATK_MS53L1M_FUNCODE_BACKRATE     = 0x01, /* 回传速率设置 */
    ATK_MS53L1M_FUNCODE_BAUDRATE     = 0x02, /* 波特率设置 */
    ATK_MS53L1M_FUNCODE_IDSET        = 0x03, /* 设备地址设置 */
    ATK_MS53L1M_FUNCODE_MEAUDATA     = 0x05, /* 测量数据获取 */
    ATK_MS53L1M_FUNCODE_OUTPUTSTATUS = 0x07, /* 测量状态 */
    ATK_MS53L1M_FUNCODE_MEAUMODE     = 0x08, /* 测量模式设置 */
    ATK_MS53L1M_FUNCODE_CALIMODE     = 0x09, /* 校准模式 */
    ATK_MS53L1M_FUNCODE_WORKMODE     = 0x0A, /* 工作模式 */
    ATK_MS53L1M_FUNCODE_TIMEBUDGET   = 0x0B, /* 定时预设 */
    ATK_MS53L1M_FUNCODE_TIMRPERIOD   = 0x0D, /* 测量间隔 */
    ATK_MS53L1M_FUNCODE_ERRORFRAM    = 0x0F, /* 错误帧信息 */
    ATK_MS53L1M_FUNCODE_VERSION      = 0x10, /* 版本信息 */
};

/* 系统设置参数 */
enum
{
    ATK_MS53L1M_SYS_PARAM_RESET = 0x01, /* 系统恢复设置 */
    ATK_MS53L1M_SYS_RESET       = 0x02, /* 系统复位 */
};

/* 回传速率设置参数 */
enum
{
    ATK_MS53L1M_BACKRATE_01HZ  = 0x00, /* 0.1Hz */
    ATK_MS53L1M_BACKRATE_02HZ  = 0x01, /* 0.2Hz */
    ATK_MS53L1M_BACKRATE_05HZ  = 0x02, /* 0.5Hz */
    ATK_MS53L1M_BACKRATE_1HZ   = 0x03, /* 1Hz */
    ATK_MS53L1M_BACKRATE_2HZ   = 0x04, /* 2Hz */
    ATK_MS53L1M_BACKRATE_5HZ   = 0x05, /* 5Hz */
    ATK_MS53L1M_BACKRATE_10HZ  = 0x06, /* 10Hz */
    ATK_MS53L1M_BACKRATE_20HZ  = 0x07, /* 20Hz */
    ATK_MS53L1M_BACKRATE_50HZ  = 0x08, /* 50Hz */
    ATK_MS53L1M_BACKRATE_100HZ = 0x09, /* 100Hz */
};

/* 串口波特率设置参数 */
enum
{
    ATK_MS53L1M_BAUDRATE_2400   = 0x00, /* 2400bps */
    ATK_MS53L1M_BAUDRATE_4800   = 0x01, /* 4800bps */
    ATK_MS53L1M_BAUDRATE_9600   = 0x02, /* 9600bps */
    ATK_MS53L1M_BAUDRATE_19200  = 0x03, /* 19200bps */
    ATK_MS53L1M_BAUDRATE_38400  = 0x04, /* 38400bps */
    ATK_MS53L1M_BAUDRATE_57600  = 0x05, /* 57600bps */
    ATK_MS53L1M_BAUDRATE_115200 = 0x06, /* 115200bps */
    ATK_MS53L1M_BAUDRATE_230400 = 0x07, /* 230400bps */
    ATK_MS53L1M_BAUDRATE_460800 = 0x08, /* 460800bps */
    ATK_MS53L1M_BAUDRATE_921600 = 0x09, /* 921600bps */
};

/* 测量模式设置参数 */
enum
{
    ATK_MS53L1M_MEAUMODE_SHORT  = 0x00, /* 短距离 */
    ATK_MS53L1M_MEAUMODE_MIDDLE = 0x01, /* 中距离 */
    ATK_MS53L1M_MEAUMODE_LONG   = 0x02, /* 长距离 */
};

/* 工作模式设置参数 */
enum
{
    ATK_MS53L1M_WORKMODE_NORMAL = 0x00, /* Normal模式 */
    ATK_MS53L1M_WORKMODE_MODBUS = 0x01, /* Modbus模式 */
    ATK_MS53L1M_WORKMODE_IIC    = 0x02, /* IIC模式 */
};

/* 错误帧输出设置参数 */
enum
{
    ATK_MS53L1M_ERRORFRAM_OFF = 0x00, /* 关闭 */
    ATK_MS53L1M_ERRORFRAM_ON  = 0x01, /* 开启 */
};



#define ATK_MS53L1M_MASTER_FRAME_HEAD 0x51 /* 主机请求帧头 */
#define ATK_MS53L1M_SLAVE_FRAME_HEAD 0x55 /* 从机应答帧头 */
#define ATK_MS53L1M_SENSOR_TYPE 0x0B /* ATK-MS53L1M传感器类型 */

#define ATK_MS53L1M_FRAME_LEN_MAX 270 /* 接收帧最大长度 */
#define ATK_MS53L1M_FRAME_LEN_MIN 8 /* 接收帧最小长度 */

#define ATK_MS53L1M_OPT_READ 0x00 /* 读操作 */
#define ATK_MS53L1M_OPT_WRITE 0x01 /* 写操作 */

/* UART收发缓冲大小 */
#define ATK_MS53L1M_UART_RX_BUF_SIZE 128

#if PLATFORM_STM32
static UART_HandleTypeDef* g_uart_handle; /* ATK-MS53L1M UART */
#elif PLATFORM_MSPM0
static UART_Regs* g_uart_handle; /* ATK-MS53L1M UART */

#endif



static struct
{
    uint8_t buf[ATK_MS53L1M_UART_RX_BUF_SIZE]; /* 帧接收缓冲 */
    struct
    {
        uint16_t len : 15;  /* 帧接收长度，sta[14:0] */
        uint16_t finsh : 1; /* 帧接收完成标志，sta[15] */
    } sta;                  /* 帧状态信息 */
} g_uart_rx_frame = {0};    /* ATK-MS53L1M UART接收帧缓冲信息结构体 */



// 前置函数声明
/* ATK-MS53L1M UART初始化 */
#if PLATFORM_STM32
uint8_t atk_ms53l1m_init(UART_HandleTypeDef* huart, uint16_t* id);
#elif PLATFORM_MSPM0
uint8_t atk_ms53l1m_init(UART_Regs* huart, uint16_t* id);
#endif
static void atk_ms53l1m_uart_send(uint8_t* dat, uint8_t len); /* ATK-MS53L1M UART发送数据 */
void        atk_ms53l1m_uart_rx_restart(void); /* ATK-MS53L1M UART重新开始接收数据 */
uint8_t* atk_ms53l1m_uart_rx_get_frame(void);  /* 获取ATK-MS53L1M UART接收到的一帧数据 */
uint16_t atk_ms53l1m_uart_rx_get_frame_len(void); /* 获取ATK-MS53L0 UART接收到的一帧数据的长度 */
void     atk_ms53l1m_uart_iqr_handler(void);
uint8_t  atk_ms53l1m_read_data(uint16_t addr, uint8_t code, uint8_t len,
                               uint16_t* dat); /* 根据模块功能码读取数据 */
uint8_t  atk_ms53l1m_write_data(uint16_t addr, uint8_t reg,
                                uint8_t dat); /* 根据模块功能码写入1字节数据 */
// uint8_t atk_ms53l1m_init(uint32_t baudrate, uint16_t *id);                              /*
// ATK-MS53L1M初始化 */
uint8_t atk_ms53l1m_normal_get_data(uint16_t* dat); /* ATK-MS53L1M Normal工作模式获取测量值 */
uint8_t atk_ms53l1m_modbus_get_data(uint16_t id,
                                    uint16_t* dat); /* ATK-MS53L1M Modbus工作模式获取测量值 */

///////////////////////////////////////////////////////////////////////////////

// 平台有关的函数

static inline void atk_ms53l1m_delay_ms(uint32_t t)
{
    systick_delay_ms(t);
    // HAL_Delay(t);
}

/**
 * @brief       ATK-MS53L1M UART初始化
 * @param       huart: UART句柄
 * @retval      无
 */
#if PLATFORM_STM32
void atk_ms53l1m_uart_init(UART_HandleTypeDef* huart)
{
    if (huart == NULL) { DEBUG_INFO("atk_ms53l1m_uart_init param huart is NULL"); }
    g_uart_handle = huart;
}
#elif PLATFORM_MSPM0
void atk_ms53l1m_uart_init(UART_Regs* huart)
{
    if (huart == NULL) { DEBUG_INFO("atk_ms53l1m_uart_init param huart is NULL"); }
    g_uart_handle = huart;
}
#endif

/**
 * @brief       ATK-MS53L1M UART发送数据
 * @param       dat: 待发送的数据
 *              len: 待发送数据的长度
 * @retval      无
 */
static void atk_ms53l1m_uart_send(uint8_t* dat, uint8_t len)
{
#if PLATFORM_STM32
    HAL_UART_Transmit(g_uart_handle, dat, len, HAL_MAX_DELAY);
#elif PLATFORM_MSPM0
    for (int i = 0; i < len; i++) { DL_UART_transmitData(g_uart_handle, dat[i]); }
#endif
}

/**
 * @brief       ATK-MS53L1M UART中断回调函数
 * @param       无
 * @retval      无
 */
void atk_ms53l1m_uart_irq_handler(void)
{
    uint8_t tmp;
// DEBUG_INFO("atk_ms53l1m_uart_iqr_handler");
#if PLATFORM_STM32
    // 空闲中断
    if (__HAL_UART_GET_FLAG(g_uart_handle, UART_FLAG_ORE) != RESET) /* UART接收过载错误中断 */
    {
        __HAL_UART_CLEAR_OREFLAG(g_uart_handle); /* 清除接收过载错误中断标志 */
        (void)g_uart_handle->Instance->SR;       /* 先读SR寄存器，再读DR寄存器 */
        (void)g_uart_handle->Instance->DR;
    }

    if (__HAL_UART_GET_FLAG(g_uart_handle, UART_FLAG_RXNE) != RESET) /* UART接收中断 */
    {
        HAL_UART_Receive(g_uart_handle, &tmp, 1, HAL_MAX_DELAY); /* UART接收数据 */

        if (g_uart_rx_frame.sta.len <
            (ATK_MS53L1M_UART_RX_BUF_SIZE - 1)) /* 判断UART接收缓冲是否溢出
                                                 * 留出一位给结束符'\0'
                                                 */
        {
            g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = tmp; /* 将接收到的数据写入缓冲 */
            g_uart_rx_frame.sta.len++;                          /* 更新接收到的数据长度 */
        }
        else /* UART接收缓冲溢出 */
        {
            g_uart_rx_frame.sta.len                      = 0; /* 覆盖之前接收的数据 */
            g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = tmp; /* 将接收到的数据写入缓冲 */
            g_uart_rx_frame.sta.len++;                          /* 更新接收到的数据长度 */
        }
    }

    if (__HAL_UART_GET_FLAG(g_uart_handle, UART_FLAG_IDLE) != RESET) /* UART总线空闲中断 */
    {
        g_uart_rx_frame.sta.finsh = 1; /* 标志帧接收完成 */

        __HAL_UART_CLEAR_IDLEFLAG(g_uart_handle); /* 清除UART总线空闲中断 */
    }
#elif PLATFORM_MSPM0
    switch (DL_UART_getPendingInterrupt(g_uart_handle)) {
    case DL_UART_IIDX_OVERRUN_ERROR:
        // 获取错误
        DL_UART_getRawInterruptStatus(g_uart_handle, DL_UART_IIDX_OVERRUN_ERROR);
        DL_UART_receiveData(g_uart_handle);   // 清除错误数据
        break;
    case DL_UART_IIDX_RX:
    {
        // 接收中断
        uint8_t tmp = DL_UART_receiveData(g_uart_handle);
        if (g_uart_rx_frame.sta.len <
            (ATK_MS53L1M_UART_RX_BUF_SIZE - 1)) /* 判断UART接收缓冲是否溢出
                                                 * 留出一位给结束符'\0'
                                                 */
        {
            g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = tmp; /* 将接收到的数据写入缓冲 */
            g_uart_rx_frame.sta.len++;                          /* 更新接收到的数据长度 */
        }
        else /* UART接收缓冲溢出 */
        {
            g_uart_rx_frame.sta.len                      = 0; /* 覆盖之前接收的数据 */
            g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = tmp; /* 将接收到的数据写入缓冲 */
            g_uart_rx_frame.sta.len++;                          /* 更新接收到的数据长度 */
        }
    } break;
    case DL_UART_IIDX_RX_TIMEOUT_ERROR:
        // 超时错误
        g_uart_rx_frame.sta.finsh = 1; /* 标志帧接收完成 */
    default: break;
    }



#endif
}

///////////////////////////////////////////////////////////////////////////////

/**
 * @brief       ATK-MS53L1M UART重新开始接收数据
 * @param       无
 * @retval      无
 */
void atk_ms53l1m_uart_rx_restart(void)
{
    g_uart_rx_frame.sta.len   = 0;
    g_uart_rx_frame.sta.finsh = 0;
}

/**
 * @brief       获取ATK-MS53L1M UART接收到的一帧数据
 * @param       无
 * @retval      NULL: 未接收到一帧数据
 *              其他: 接收到的一帧数据
 */
uint8_t* atk_ms53l1m_uart_rx_get_frame(void)
{
    if (g_uart_rx_frame.sta.finsh == 1) {
        g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = '\0';
        return g_uart_rx_frame.buf;
    }
    else {
        return NULL;
    }
}

/**
 * @brief       获取ATK-MS53L0 UART接收到的一帧数据的长度
 * @param       无
 * @retval      0   : 未接收到一帧数据
 *              其他: 接收到的一帧数据的长度
 */
uint16_t atk_ms53l1m_uart_rx_get_frame_len(void)
{
    if (g_uart_rx_frame.sta.finsh == 1) { return g_uart_rx_frame.sta.len; }
    else {
        return 0;
    }
}

/**
 * @brief       计算CRC校验和
 * @param       buf: 数据缓冲
 *              len: 数据长度
 * @retval      CRC校验和值
 */
static inline uint16_t atk_ms53l1m_crc_check_sum(uint8_t* buf, uint16_t len)
{
    uint16_t check_sum = 0;
    uint16_t i;

    for (i = 0; i < len; i++) { check_sum += buf[i]; }

    return check_sum;
}

/**
 * @brief       解析接收到的数据包
 * @param       dat: 读操作时，读取到的数据
 * @retval      ATK_MS53L1M_EOK     : 没有错误
 *              ATK_MS53L1M_ETIMEOUT: 接收数据超时
 *              ATK_MS53L1M_EFRAME  : 帧错误
 *              ATK_MS53L1M_ECRC    : CRC校验错误
 *              ATK_MS53L1M_EOPT    : 操作错误
 */
static uint8_t atk_ms53l1m_unpack_recv_data(uint16_t* dat)
{
    uint8_t* frame_buf  = NULL;
    uint16_t timeout    = 0;
    uint16_t recv_len   = 0;
    uint16_t frame_loop = 0;
    uint16_t frame_head_index;
    uint16_t frame_len;
    uint8_t  opt_type;
    uint16_t dat_len;
    uint16_t frame_check_sum;
    uint16_t check_sum;
    DEBUG_INFO("atk_ms53l1m_unpack_recv_data");
    while (frame_buf == NULL) {
        /* 等待ATK-MS53L1M UART接收到一帧数据 */
        frame_buf = atk_ms53l1m_uart_rx_get_frame();
        atk_ms53l1m_delay_ms(1);   // delay_ms(1);
        timeout++;
        if (timeout == 1000) {
            DEBUG_INFO("atk_ms53l1m_uart_rx_get_frame 接收超时错误");
            /* 接收超时错误 */
            return ATK_MS53L1M_ETIMEOUT;
        }
    }

    /* 获取接收数据的长度 */
    recv_len = atk_ms53l1m_uart_rx_get_frame_len();
    if ((recv_len < ATK_MS53L1M_FRAME_LEN_MIN) || (recv_len > ATK_MS53L1M_FRAME_LEN_MAX)) {
        /* 接收帧长度异常错误 */
        return ATK_MS53L1M_EFRAME;
    }

    /* 查找帧头 */
    do {
        if ((frame_buf[frame_loop] == ATK_MS53L1M_SLAVE_FRAME_HEAD) &&
            (frame_buf[frame_loop + 1] == ATK_MS53L1M_SENSOR_TYPE)) {
            break;
        }

        if (frame_loop != (recv_len - 2)) { frame_loop++; }
        else {
            /* 帧异常 */
            return ATK_MS53L1M_EFRAME;
        }
    } while (1);

    frame_head_index = frame_loop;                  /* 记录帧头位置 */
    frame_len        = recv_len - frame_head_index; /* 计算帧长度 */

    if ((frame_len < ATK_MS53L1M_FRAME_LEN_MIN) || (frame_len > ATK_MS53L1M_FRAME_LEN_MAX)) {
        /* 接收帧长度异常错误 */
        return ATK_MS53L1M_EFRAME;
    }

    opt_type = frame_buf[frame_head_index + 4]; /* 获取操作类型 */


    if (opt_type == 0x00) {
        dat_len = frame_buf[frame_head_index + 7]; /* 获取数据长度 */
        if ((dat_len + 10) > frame_len) {
            /* 帧异常 */
            return ATK_MS53L1M_EFRAME;
        }

        frame_check_sum = atk_ms53l1m_crc_check_sum(&frame_buf[frame_head_index],
                                                    dat_len + 8); /* 计算CRC校验和 */
        check_sum       = ((uint16_t)frame_buf[frame_head_index + dat_len + 8] << 8) +
                    frame_buf[frame_head_index + dat_len + 9]; /* 获取帧的CRC校验和 */
        if (frame_check_sum == check_sum) {
            if (frame_buf[frame_head_index + 7] == 1) { *dat = frame_buf[frame_head_index + 8]; }
            else if (frame_buf[frame_head_index + 7] == 2) {
                *dat = ((uint16_t)frame_buf[frame_head_index + 8] << 8) +
                       frame_buf[frame_head_index + 9];
            }
            else {
                /* 帧错误 */
                return ATK_MS53L1M_EFRAME;
            }

            return ATK_MS53L1M_EOK;
        }
        else {
            /* CRC错误 */
            return ATK_MS53L1M_ECRC;
        }
    }
    else if (opt_type == 0x01) {
        frame_check_sum =
            atk_ms53l1m_crc_check_sum(&frame_buf[frame_head_index], 6); /* 计算CRC校验和 */
        check_sum =
            ((uint16_t)frame_buf[frame_head_index + 6] << 8) + frame_buf[frame_head_index + 7];
        if (frame_check_sum == check_sum) { return ATK_MS53L1M_EOK; }
        else {
            /* CRC错误 */
            return ATK_MS53L1M_ECRC;
        }
    }
    else if (opt_type == 0xFF) {
        if ((frame_buf[frame_head_index + 2] == 0xFF) &&
            (frame_buf[frame_head_index + 3] == 0xFF)) {
            frame_check_sum =
                atk_ms53l1m_crc_check_sum(&frame_buf[frame_head_index], 6); /* 计算CRC校验和 */
            check_sum =
                ((uint16_t)frame_buf[frame_head_index + 6] << 8) + frame_buf[frame_head_index + 7];
            if (frame_check_sum == check_sum) {
                /* 异常操作 */
                return ATK_MS53L1M_EOPT;
            }
            else {
                /* CRC错误 */
                return ATK_MS53L1M_ECRC;
            }
        }
        else {
            /* 帧异常 */
            return ATK_MS53L1M_EFRAME;
        }
    }
    else {
        /* 帧异常 */
        return ATK_MS53L1M_EFRAME;
    }
}

/**
 * @brief       根据模块功能码读取数据
 * @param       addr: 设备地址
 *              fun_code : 功能码
 *              len : 数据长度，取值范围：1或2
 *              dat : 读取到的数据
 * @retval      ATK_MS53L1M_EOK     : 没有错误
 *              ATK_MS53L1M_ETIMEOUT: 接收数据超时
 *              ATK_MS53L1M_EFRAME  : 帧错误
 *              ATK_MS53L1M_ECRC    : CRC校验错误
 *              ATK_MS53L1M_EOPT    : 操作错误
 */
uint8_t atk_ms53l1m_read_data(uint16_t addr, uint8_t fun_code, uint8_t len, uint16_t* dat)
{
    uint8_t  ret;
    uint16_t check_sum;
    uint8_t  buf[9];
    DEBUG_INFO("atk_ms53l1m_read_data");
    buf[0] = ATK_MS53L1M_MASTER_FRAME_HEAD; /* 标志头 */
    buf[1] = ATK_MS53L1M_SENSOR_TYPE;       /* 传感器类型 */
    buf[2] = (uint8_t)(addr >> 8);          /* 传感器地址，高8位 */
    buf[3] = (uint8_t)(addr & 0xFF);        /* 传感器地址，低8位 */
    buf[4] = ATK_MS53L1M_OPT_READ;          /* 读操作 */
    buf[5] = fun_code;                      /* 功能码 */
    buf[6] = len;                           /* 数据长度 */

    check_sum = atk_ms53l1m_crc_check_sum(buf, 7); /* 计算CRC校验和 */

    buf[7] = (uint8_t)(check_sum >> 8);   /* CRC校验码，高8位 */
    buf[8] = (uint8_t)(check_sum & 0xFF); /* CRC校验码，低8位 */

    atk_ms53l1m_uart_rx_restart();           /* 准备重新开始接收新的一帧数据 */
    atk_ms53l1m_uart_send(buf, 9);           /* 发送数据 */
    ret = atk_ms53l1m_unpack_recv_data(dat); /* 解析应答数据 */
    DEBUG_INFO("atk_ms53l1m_unpack_recv_data, ret=%d", ret);
    return ret;
}

/**
 * @brief       根据模块功能码写入1字节数据
 * @param       addr     : 设备地址
 *              fun_code : 功能码
 *              dat      : 待写入的1字节数据
 * @retval      ATK_MS53L1M_EOK     : 没有错误
 *              ATK_MS53L1M_ETIMEOUT: 接收数据超时
 *              ATK_MS53L1M_EFRAME  : 帧错误
 *              ATK_MS53L1M_ECRC    : CRC校验错误
 *              ATK_MS53L1M_EOPT    : 操作错误
 */
uint8_t atk_ms53l1m_write_data(uint16_t addr, uint8_t fun_code, uint8_t dat)
{
    uint8_t  ret;
    uint8_t  buf[10];
    uint16_t check_sum;

    buf[0] = ATK_MS53L1M_MASTER_FRAME_HEAD; /* 标志头 */
    buf[1] = ATK_MS53L1M_SENSOR_TYPE;       /* 传感器类型 */
    buf[2] = (uint8_t)(addr >> 8);          /* 传感器地址，高8位 */
    buf[3] = (uint8_t)(addr & 0xFF);        /* 传感器地址，低8位 */
    buf[4] = ATK_MS53L1M_OPT_WRITE;         /* 写操作 */
    buf[5] = fun_code;                      /* 功能码 */
    buf[6] = 0x01;                          /* 数据长度 */
    buf[7] = dat;                           /* 数据 */

    check_sum = atk_ms53l1m_crc_check_sum(buf, 8); /* 计算CRC校验和 */

    buf[8] = (uint8_t)(check_sum >> 8);   /* CRC校验码，高8位 */
    buf[9] = (uint8_t)(check_sum & 0xFF); /* CRC校验码，低8位 */

    atk_ms53l1m_uart_rx_restart();            /* 准备重新开始接收新的一帧数据 */
    atk_ms53l1m_uart_send(buf, 10);           /* 发送数据 */
    ret = atk_ms53l1m_unpack_recv_data(NULL); /* 解析应答数据 */

    return ret;
}

/**
 * @brief       ATK-MS53L1M初始化
 * @param       baudrate: ATK-MS53L1M UART通讯波特率
 *              id      : ATK-MS53L1M的设备ID
 * @retval      ATK_MS53L1M_EOK  : ATK-MS53L1M初始化成功，函数执行成功
 *              ATK_MS53L1M_ERROR: ATK-MS53L1M初始化失败，函数执行失败
 */
#if PLATFORM_STM32
uint8_t atk_ms53l1m_init(UART_HandleTypeDef* huart, uint16_t* id)
#elif PLATFORM_MSPM0
uint8_t atk_ms53l1m_init(UART_Regs* huart, uint16_t* id)
#endif
{
    uint8_t i;
    DEBUG_INFO("atk_ms53l1m开始初始化");
    /* ATK-MS53L1M UART初始化 */
    atk_ms53l1m_uart_init(huart);

    /* 获取设备地址 */
    i = 0;
    while (atk_ms53l1m_read_data(0xFFFF, ATK_MS53L1M_FUNCODE_IDSET, 2, id) != ATK_MS53L1M_EOK) {
        atk_ms53l1m_delay_ms(100);   // delay_ms(100);
        DEBUG_INFO("normal模式重试中，i=%d", i);
        if (++i == 5) { return ATK_MS53L1M_ERROR; }
    }

    /* 设置ATK-MS53L1M模块的工作模式为Modbus模式 */
    i = 0;
    while (atk_ms53l1m_write_data(*id, ATK_MS53L1M_FUNCODE_WORKMODE, ATK_MS53L1M_WORKMODE_MODBUS) !=
           ATK_MS53L1M_EOK) {
        atk_ms53l1m_delay_ms(100);   // delay_ms(100);
        // DEBUG_INFO("modbus模式重试中，i=%d", i);
        if (++i == 5) { return ATK_MS53L1M_ERROR; }
    }

    return ATK_MS53L1M_EOK;
}

/**
 * @brief       ATK-MS53L1M Normal工作模式获取测量值
 * @param       dat: 获取到的测量值
 * @retval      ATK_MS53L1M_EOK  : 获取测量值成功
 *              ATK_MS53L1M_ERROR: UART未接收到数据，获取测量值失败
 */
uint8_t atk_ms53l1m_normal_get_data(uint16_t* dat)
{
    uint8_t* buf = NULL;
    uint8_t  i   = 0;
    char*    p;
    uint16_t dat_tmp = 0;

    atk_ms53l1m_uart_rx_restart();
    while (buf == NULL) {
        buf = atk_ms53l1m_uart_rx_get_frame();
        if (++i == 10) { return ATK_MS53L1M_ERROR; }
        atk_ms53l1m_delay_ms(100);   // delay_ms(100);
    }

    p = strstr((char*)buf, "d:");
    while (*p != 'm') {
        if (*p >= '0' && *p <= '9') { dat_tmp = dat_tmp * 10 + (*p - '0'); }
        p++;
    }

    *dat = dat_tmp;

    return ATK_MS53L1M_EOK;
}

/**
 * @brief       ATK-MS53L1M Modbus工作模式获取测量值
 * @param       dat: 获取到的测量值
 * @retval      ATK_MS53L1M_EOK  : 获取测量值成功
 *              ATK_MS53L1M_ERROR: UART未接收到数据，获取测量值失败
 */
uint8_t atk_ms53l1m_modbus_get_data(uint16_t id, uint16_t* dat)
{
    uint8_t  ret;
    uint16_t dat_tmp;

    ret = atk_ms53l1m_read_data(id, ATK_MS53L1M_FUNCODE_MEAUDATA, 2, &dat_tmp);
    if (ret != 0) {
        *dat = 0;
        return ATK_MS53L1M_ERROR;
    }
    else {
        *dat = dat_tmp;
        return ATK_MS53L1M_EOK;
    }
}
