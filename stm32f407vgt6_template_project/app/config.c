#include "config.h"
#include "bsp.h"


static const u8 g_magic_number[4] = { 0x66, 0x66, 0x88, 0x99};

// 检查是否已经写入魔数
// 如果已经写入，返回1，否则返回0
u8 check_magic_number(void)
{
    sfud_err result = SFUD_SUCCESS;
    extern sfud_flash *sfud_dev;
    const sfud_flash *flash = sfud_get_device(SFUD_W25_DEVICE_INDEX);
    u8 data[4] = {0};
    // 读取4个字节数据
    result = sfud_read(flash, MAGIC_NUMBER_START_ADDR, 4, data);
    if (result != SFUD_SUCCESS) {
        debug_print("sfud_read error in check_magic_number\n");
        return 0;
    }
    return data[0] == g_magic_number[0] 
        && data[1] == g_magic_number[1]
        && data[2] == g_magic_number[2]
        && data[3] == g_magic_number[3];
}

// 写入魔数
s8 write_magic_number(void)
{
    sfud_err result = SFUD_SUCCESS;
    extern sfud_flash *sfud_dev;
    const sfud_flash *flash = sfud_get_device(SFUD_W25_DEVICE_INDEX);
    
    result = sfud_erase(flash, MAGIC_NUMBER_START_ADDR, 256);
    if (result != SFUD_SUCCESS) {
        printf("sfud_erase error in write_magic_number\n");
        return -1;
    }

    result = sfud_write(flash, MAGIC_NUMBER_START_ADDR, 4, g_magic_number);
    if (result != SFUD_SUCCESS) {
        printf("sfud_write error in write_magic_number\n");
        return -1;
    }
    
    return 0;
}

// 保存配置数据到某个w25qxx的地址
s8 save_to_addr(config_t* config, u32 flash_addr)
{
    sfud_err result = SFUD_SUCCESS;
    extern sfud_flash *sfud_dev;
    const sfud_flash *flash = sfud_get_device(SFUD_W25_DEVICE_INDEX);
    
    result = sfud_erase(flash, flash_addr, 256);
    if (result != SFUD_SUCCESS) {
        printf("sfud_erase error in save_to_addr\n");
        return -1;
    }

    result = sfud_write(flash, flash_addr, sizeof(config_t), (u8*)config);
    if (result != SFUD_SUCCESS) {
        printf("sfud_write error in save_to_addr\n");
        return -1;
    }
    
    return 0;
}

// 从某个w25qxx的地址加载数据
s8 load_from_addr(config_t* config, u32 flash_addr)
{
    sfud_err result = SFUD_SUCCESS;
    extern sfud_flash *sfud_dev;
    const sfud_flash *flash = sfud_get_device(SFUD_W25_DEVICE_INDEX);
    
    usize config_size = sizeof(config_t);
    // 读取config_t大小的字节数据
    result = sfud_read(flash, flash_addr, config_size, (u8*)config);
    if (result != SFUD_SUCCESS) {
        debug_print("sfud_read error in check_magic_number\n");
        return -1;
    }
    return 0;
}

