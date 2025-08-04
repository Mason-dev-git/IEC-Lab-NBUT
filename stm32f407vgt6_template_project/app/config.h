#ifndef CONFIG_H
#define CONFIG_H

#include "datatype.h"
#include "pid.h"

// pid的参数结构体
typedef struct
{
    f32 kp;           // 比例系数 
    f32 ki;           // 积分系数 ki=kp*T/Ti
    f32 kd;           // 微分系数 kd=kp*Td/T
} pid_config_t;

static inline void pid_config_to_struct(pid_config_t* config, pid_incremental_t* pid)
{
    pid->kp = config->kp;
    pid->ki = config->ki;
    pid->kd = config->kd;
}

static inline void pid_struct_to_config(pid_incremental_t* pid, pid_config_t* config)
{
    config->kp = pid->kp;
    config->ki = pid->ki;
    config->kd = pid->kd;
}

// 总体的配置，保证一定只能是POD类型，而且不能超过256个字节
typedef struct {
    pid_config_t left_pid;
    pid_config_t right_pid;
}config_t;

// 因为w25q128是256个字节一页，这个是最小擦除单元，所以magic number直接用一页
// 魔数地址，用于标记是否已经写入默认配置，如果已经写入，那么这个地址开始的4个字节数据会被写为魔数
#define MAGIC_NUMBER_START_ADDR 0
// 默认配置的写入地址
#define DEFULT_CONFIG_START_ADDR 256
// 活动可以修改配置的其实地址
#define ACTIVE_CONFIG_START_ADDR 256*2

// 检查是否已经写入魔数
// 如果已经写入，返回1，否则返回0
u8 check_magic_number(void);
// 写入魔数
// 如果写入成功返回0，否则返回-1
s8 write_magic_number(void);
// 保存配置数据到某个w25qxx的地址
s8 save_to_addr(config_t* config, u32 flash_addr);
// 从某个w25qxx的地址加载数据
s8 load_from_addr(config_t* config, u32 flash_addr);


#endif // !CONFIG_H
