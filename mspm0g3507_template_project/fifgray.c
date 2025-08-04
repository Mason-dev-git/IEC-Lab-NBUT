#include "fifgray.h"
#include "bsp.h"

uint8_t v1,v2,v3,v4,v5,v6,v7 ;
uint8_t gray_scan(void)
{
     v1 = DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_1_PORT, GPIO_GRP_GRAY_PIN_SENSER_1_PIN) ? 1 : 0;
     v2 = DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_2_PORT, GPIO_GRP_GRAY_PIN_SENSER_2_PIN) ? 1 : 0;
     v3 = DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_3_PORT, GPIO_GRP_GRAY_PIN_SENSER_3_PIN) ? 1 : 0;
     v4 = DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_4_PORT, GPIO_GRP_GRAY_PIN_SENSER_4_PIN) ? 1 : 0;
     v5 = DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_5_PORT, GPIO_GRP_GRAY_PIN_SENSER_5_PIN) ? 1 : 0;
     v6 = DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_6_PORT, GPIO_GRP_GRAY_PIN_SENSER_6_PIN) ? 1 : 0;
     v7 = DL_GPIO_readPins(GPIO_GRP_GRAY_PIN_SENSER_7_PORT, GPIO_GRP_GRAY_PIN_SENSER_7_PIN) ? 1 : 0;
    // debug_printf("v1=%d v2=%d v3=%d v4=%d v5=%d v6=%d v7=%d\n", v1, v2, v3, v4, v5, v6, v7);


    // 将 v1~v7 拼接成一个 7 位的整数，v1 是最高位，v7 是最低位
    uint8_t state = (v1 << 7) | (v2 << 6) | (v3 << 5) | (v4 << 4) | (v5 << 3) | (v6 << 2) | (v7 << 1);
    return state;
}
