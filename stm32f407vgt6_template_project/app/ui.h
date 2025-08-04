#ifndef UI_H
#define UI_H

#include "datatype.h"
#include "bsp.h"

#define UI_MAX_PAGE_NUM                10

#define UI_PAGE_NULL                  -1
#define UI_PAGE_MAIN                   0
//MAIN
#define UI_PAGE_CONFIG                 10
#define UI_PAGE_RUN                    11
//CONFIG
#define UI_PAGE_PID                    20
#define UI_PAGE_GRAY                   21
//RUN
#define UI_PAGE_TASK1                  30
#define UI_PAGE_TASK2                  31
#define UI_PAGE_TASK3                  32
#define UI_PAGE_TASK4                  33

#define UI_PAGE_KP                     40
#define UI_PAGE_KI                     41
#define UI_PAGE_KD                     42
#define pid_min_val                    0
#define pid_max_val                    100

#define UI_PAGE_GRAY_CH0               50
#define UI_PAGE_GRAY_CH1               51
#define UI_PAGE_GRAY_CH2               52
#define UI_PAGE_GRAY_CH3               53
#define UI_PAGE_GRAY_CH4               54
#define UI_PAGE_GRAY_CH5               55
#define UI_PAGE_GRAY_CH6               56
#define UI_PAGE_GRAY_CH7               57

#define UI_PAGE_SAVE_PID               60


s32 ui_page_main(s32 current_index);
s32 ui_page_config(s32 current_index);
s32 ui_page_run(s32 current_index);
s32 ui_page_pid(s32 current_index);
s32 ui_page_gray(s32 current_index);
s32 ui_page_kp(s32 current_index);
s32 ui_page_ki(s32 current_index);
s32 ui_page_kd(s32 current_index);
s32 ui_page_save_pid(s32 current_index);


void ui_loop(void);

#endif   // !UI_H



