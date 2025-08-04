//
// 标准库的矩阵键盘
//
#ifndef DEVICE_MATRIX_KEY_H
#define DEVICE_MATRIX_KEY_H

#include <stdint.h>

#define MATRIX_KEY_VALUE_UP 'U'
#define MATRIX_KEY_VALUE_DOWN 'D'
#define MATRIX_KEY_VALUE_LEFT 'L'
#define MATRIX_KEY_VALUE_RIGHT 'R'
#define MATRIX_KEY_VALUE_ENTER 'E'
#define MATRIX_KEY_VALUE_CANCEL 'C'

// E -> Enter   确定  C -> Cancel 取消
// L -> Left    左    R -> Right  右
// U -> Up      上    D -> Down   下
//

extern char matrix_key_table[16];

void matrix_key_init(void);

uint8_t matrix_key_get_value(void);

#endif
