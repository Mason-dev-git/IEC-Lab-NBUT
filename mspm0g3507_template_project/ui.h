#ifndef UI_H
#define UI_H

#include "bsp.h"

#define UI_MAX_PAGE_NUM                10

#define UI_PAGE_NULL                  -1
#define UI_PAGE_MAIN                   0
//MAIN
#define UI_PAGE_BASIC                  10
#define UI_PAGE_ADVANCE                11
//BASIC
#define UI_PAGE_TASK1                  20
#define UI_PAGE_TASK2                  21
#define UI_PAGE_TASK3                  22
//ADVANCE
#define UI_PAGE_TASK4                  30
#define UI_PAGE_TASK5                  31
#define UI_PAGE_TASK6                  32

s32 ui_page_main(s32 current_index);
s32 ui_page_basic(s32 current_index);
s32 ui_page_advance(s32 current_index);

s32 ui_page_task1(s32 current_index);
s32 ui_page_task2(s32 current_index);
s32 ui_page_task3(s32 current_index);

s32 ui_page_task4(s32 current_index);
s32 ui_page_task5(s32 current_index);
s32 ui_page_task6(s32 current_index);

void ui_loop(void);

#endif   // !UI_H



