#ifndef MYLIB_DEVICE_KEY_H
#define MYLIB_DEVICE_KEY_H

#include "bsp.h"
#include "datatype.h"
#include <stdint.h>
#define KEY_VALUE_NONE 0
#define KEY_VALUE_1 1
#define KEY_VALUE_2 2
#define KEY_VALUE_3 3
#define KEY_VALUE_4 4

struct key_t{
    s32 keyState;
    s32 judgeState;
    s32 singleFlag;
    s32 longFlag;
    s32 time;
};
// 公开key
extern struct key_t keys[4];



void key_init(void);

int32_t key_get_value(void);

#endif   // !MYLIB_DEVICE_KEY_H
