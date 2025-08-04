#include "ui.h"

typedef s32 (*ui_handler_t)(s32);
float pid[3] = {0};// Kp, Ki, Kd


// 获取页面处理函数(内部使用)
ui_handler_t get_page_handler(s32 index)
{
    switch (index) {
        case UI_PAGE_MAIN: return ui_page_main;
        case UI_PAGE_CONFIG: return ui_page_config;
        case UI_PAGE_RUN: return ui_page_run;
        case UI_PAGE_PID: return ui_page_pid;
        case UI_PAGE_GRAY: return ui_page_gray;
        case UI_PAGE_KP: return ui_page_kp;
        case UI_PAGE_KI: return ui_page_ki;
        case UI_PAGE_KD: return ui_page_kd;
        case UI_PAGE_SAVE_PID: return ui_page_save_pid;
        // other page
        default: return NULL;
    }
}


// UI界面主循环
void ui_loop(void)
{
    s32 current = UI_PAGE_MAIN;
    ui_handler_t handler;
    LCD_Fill(0, 0, LCD_W, 16, BLUE);
    LCD_ShowString(48, 0, "MENU", WHITE, BLUE, 16, 0);
    LCD_ShowString(0, 16, ">>", BLACK, WHITE, 16, 0);
    while (1) {
        handler = get_page_handler(current);
        current = handler(current);
        switch (current) {
            case UI_PAGE_NULL: break;
            case UI_PAGE_MAIN:
                LCD_ShowString(16, 16, "Config", BLACK, WHITE, 16, 0);
                LCD_ShowString(16, 32, "Run", BLACK, WHITE, 16, 0);
                break;
            case UI_PAGE_CONFIG:
                LCD_ShowString(16, 16, "PID", BLACK, WHITE, 16, 0);
                LCD_ShowString(16, 32, "Gray", BLACK, WHITE, 16, 0);
                break;
            case UI_PAGE_RUN:
                LCD_ShowString(16, 16, "Task1", BLACK, WHITE, 16, 0);
                LCD_ShowString(16, 32, "Task2", BLACK, WHITE, 16, 0);
                LCD_ShowString(16, 48, "Task3", BLACK, WHITE, 16, 0);
                LCD_ShowString(16, 64, "Task4", BLACK, WHITE, 16, 0);

                break;
            case UI_PAGE_PID:
                LCD_ShowString(16, 16, "Kp:", BLACK, WHITE, 16, 0);
                LCD_ShowFloatNum(48, 16, pid[0], 4, BLACK, WHITE, 16);
                LCD_ShowString(16, 32, "Ki:", BLACK, WHITE, 16, 0);
                LCD_ShowFloatNum(48, 32, pid[1], 4, BLACK, WHITE, 16);
                LCD_ShowString(16, 48, "Kd:", BLACK, WHITE, 16, 0);
                LCD_ShowFloatNum(48, 48, pid[2], 4, BLACK, WHITE, 16);
                break;
            case UI_PAGE_GRAY:
                LCD_ShowString(16, 16, "CH0:", BLACK, WHITE, 16, 0);
                LCD_ShowString(16, 32, "CH1:", BLACK, WHITE, 16, 0);
                LCD_ShowString(16, 48, "CH2:", BLACK, WHITE, 16, 0);
                LCD_ShowString(16, 64, "CH3:", BLACK, WHITE, 16, 0);
                LCD_ShowString(16, 80, "CH4:", BLACK, WHITE, 16, 0);
                LCD_ShowString(16, 96, "CH5:", BLACK, WHITE, 16, 0);
                LCD_ShowString(16, 112, "CH6:", BLACK, WHITE, 16, 0);
                LCD_ShowString(16, 128, "CH7:", BLACK, WHITE, 16, 0);
                break;
            case UI_PAGE_TASK1:
                LCD_ShowString(16, 16, "STOP", BLACK, WHITE, 16, 0);
                LCD_ShowString(16, 32, "RUN", BLACK, WHITE, 16, 0);
                break;
            case UI_PAGE_TASK2:
                LCD_ShowString(16, 16, "STOP", BLACK, WHITE, 16, 0);
                LCD_ShowString(16, 32, "RUN", BLACK, WHITE, 16, 0);
                break;
            case UI_PAGE_TASK3:
                LCD_ShowString(16, 16, "STOP", BLACK, WHITE, 16, 0);
                LCD_ShowString(16, 32, "RUN", BLACK, WHITE, 16, 0);
                break;
            case UI_PAGE_TASK4:
                LCD_ShowString(16, 16, "STOP", BLACK, WHITE, 16, 0);
                LCD_ShowString(16, 32, "RUN", BLACK, WHITE, 16, 0);
                break;
            case UI_PAGE_KP:
                LCD_ShowString(0, 16, ">>Kp:", BLACK, WHITE, 16, 0);
                LCD_ShowFloatNum(48, 16, pid[0], 4, BLACK, WHITE, 16);
                LCD_ShowString(16, 32, "Ki:", LGRAY, WHITE, 16, 0);
                LCD_ShowFloatNum(48, 32, pid[1], 4, LGRAY, WHITE, 16);
                LCD_ShowString(16, 48, "Kd:", LGRAY, WHITE, 16, 0);
                LCD_ShowFloatNum(48, 48, pid[2], 4, LGRAY, WHITE, 16);
                break;
            case UI_PAGE_KI:
                LCD_ShowString(16, 16, "Kp:", LGRAY, WHITE, 16, 0);
                LCD_ShowFloatNum(48, 16, pid[0], 4, LGRAY, WHITE, 16);
                LCD_ShowString(0, 32, ">>Ki:", BLACK, WHITE, 16, 0);
                LCD_ShowFloatNum(48, 32, pid[1], 4, BLACK, WHITE, 16);
                LCD_ShowString(16, 48, "Kd:", LGRAY, WHITE, 16, 0);
                LCD_ShowFloatNum(48, 48, pid[2], 4, LGRAY, WHITE, 16);
                break;
            case UI_PAGE_KD:
                LCD_ShowString(16, 16, "Kp:", LGRAY, WHITE, 16, 0);
                LCD_ShowFloatNum(48, 16, pid[0], 4, LGRAY, WHITE, 16);
                LCD_ShowString(16, 32, "Ki:", LGRAY, WHITE, 16, 0);
                LCD_ShowFloatNum(48, 32, pid[1], 4, LGRAY, WHITE, 16);
                LCD_ShowString(0, 48, ">>Kd:", BLACK, WHITE, 16, 0);
                LCD_ShowFloatNum(48, 48, pid[2], 4, BLACK, WHITE, 16);
                break;
            case UI_PAGE_SAVE_PID:
                LCD_ShowString(32, 16, "Kp:", BLACK, WHITE, 16, 0);
                LCD_ShowFloatNum(56, 16, pid[0], 4, BLACK, WHITE, 16);
                LCD_ShowString(32, 32, "Ki:", BLACK, WHITE, 16, 0);
                LCD_ShowFloatNum(56, 32, pid[1], 4, BLACK, WHITE, 16);
                LCD_ShowString(32, 48, "Kd:", BLACK, WHITE, 16, 0);
                LCD_ShowFloatNum(56, 48, pid[2], 4, BLACK, WHITE, 16);
                LCD_ShowString(0, 64, "[Warning]Sure to", RED, WHITE, 16, 0);
                LCD_ShowString(0, 80, " save to Flash? ", RED, WHITE, 16, 0);
                LCD_ShowString(24, 112, "YES", WHITE, RED, 16, 0);
                LCD_ShowString(80, 112, "NO", WHITE, BLACK, 16, 0);
                break;
            default: break;
        }
    }
}


/*********************************主页面**********************************/
/************************************************************************/
//(MAIN)
s32 ui_page_main(s32 current_index)
{
    static u32 pos_index = UI_PAGE_CONFIG;
    if (keys[1].singleFlag == 1)   // 上移按键，维持当前页面
    {
        keys[1].singleFlag = 0;
        pos_index = (pos_index - 1) < UI_PAGE_CONFIG ? UI_PAGE_CONFIG : (pos_index - 1);//目标页面指针更新
        LCD_ShowString(0, ((pos_index - UI_PAGE_CONFIG) + 2) * 16, "  ", BLACK, WHITE, 16, 0);//屏幕指针更新
        LCD_ShowString(0, ((pos_index - UI_PAGE_CONFIG) + 1) * 16, ">>", BLACK, WHITE, 16, 0);
    }
    if (keys[2].singleFlag == 1)   // 下移按键，维持当前页面
    {
        keys[2].singleFlag = 0;
        pos_index = (pos_index + 1) > UI_PAGE_RUN ? UI_PAGE_RUN : (pos_index + 1);
        LCD_ShowString(0, (pos_index - UI_PAGE_CONFIG) * 16, "  ", BLACK, WHITE, 16, 0);
        LCD_ShowString(0, ((pos_index - UI_PAGE_CONFIG) + 1) * 16, ">>", BLACK, WHITE, 16, 0);
    }
    if (keys[3].singleFlag == 1)   // 取消按键，返回上一页面
    {
        keys[3].singleFlag = 0;
        current_index = UI_PAGE_MAIN;
        pos_index = UI_PAGE_CONFIG;
        LCD_Fill(0, 16, LCD_W, LCD_H, WHITE);
        LCD_ShowString(0, 16, ">>", BLACK, WHITE, 16, 0);   // 屏幕指针复位
    }
    if (keys[4].singleFlag == 1)   // 确认按键，进入下一页面
    {
        keys[4].singleFlag = 0;
        current_index = pos_index;
        pos_index = UI_PAGE_CONFIG;
        LCD_Fill(0, 16, LCD_W, LCD_H, WHITE);
        LCD_ShowString(0, 16, ">>", BLACK, WHITE, 16, 0);   // 屏幕指针复位
    }
    return current_index;
}


/********************************一级页面*********************************/
/************************************************************************/
//(MAIN->CONFIG)
s32 ui_page_config(s32 current_index)
{
    static u32 pos_index = UI_PAGE_PID;
    if (keys[1].singleFlag == 1)   // 上移按键，维持当前页面
    {
        keys[1].singleFlag = 0;
        pos_index = (pos_index - 1) < UI_PAGE_PID ? UI_PAGE_PID : (pos_index - 1);
        LCD_ShowString(0, ((pos_index - UI_PAGE_PID) + 2) * 16, "  ", BLACK, WHITE, 16, 0);   // 屏幕指针更新
        LCD_ShowString(0, ((pos_index - UI_PAGE_PID) + 1) * 16, ">>", BLACK, WHITE, 16, 0);
    }
    if (keys[2].singleFlag == 1)   // 下移按键，维持当前页面
    {
        keys[2].singleFlag = 0;
        pos_index = (pos_index + 1) > UI_PAGE_GRAY ? UI_PAGE_GRAY : (pos_index + 1);
        LCD_ShowString(0, (pos_index - UI_PAGE_PID) * 16, "  ", BLACK, WHITE, 16, 0);   // 屏幕指针更新
        LCD_ShowString(0, ((pos_index - UI_PAGE_PID) + 1) * 16, ">>", BLACK, WHITE, 16, 0);
    }
    if (keys[3].singleFlag == 1)   // 取消按键，返回上一页面
    {
        keys[3].singleFlag = 0;
        current_index = UI_PAGE_MAIN;
        pos_index = UI_PAGE_PID;
        LCD_Fill(0, 16, LCD_W, LCD_H, WHITE);
        LCD_ShowString(0, 16, ">>", BLACK, WHITE, 16, 0);   // 屏幕指针复位
    }
    if (keys[4].singleFlag == 1)   // 确认按键，进入下一页面
    {
        keys[4].singleFlag = 0;
        current_index = pos_index;
        pos_index = UI_PAGE_PID;
        LCD_Fill(0, 16, LCD_W, LCD_H, WHITE);
        LCD_ShowString(0, 16, ">>", BLACK, WHITE, 16, 0);   // 屏幕指针复位
    }
    return current_index;
}

//(MAIN->RUN)
s32 ui_page_run(s32 current_index)
{
    static u32 pos_index = UI_PAGE_TASK1;
    if (keys[1].singleFlag == 1)   // 上移按键，维持当前页面
    {
        keys[1].singleFlag = 0;
        pos_index = (pos_index - 1) < UI_PAGE_TASK1 ? UI_PAGE_TASK1 : (pos_index - 1);
        LCD_ShowString(0, ((pos_index - UI_PAGE_TASK1) + 2) * 16, "  ", BLACK, WHITE, 16, 0);
        LCD_ShowString(0, ((pos_index - UI_PAGE_TASK1) + 1) * 16, ">>", BLACK, WHITE, 16, 0);
    }
    if (keys[2].singleFlag == 1)   // 下移按键，维持当前页面
    {
        keys[2].singleFlag = 0;
        pos_index = (pos_index + 1) > UI_PAGE_TASK4 ? UI_PAGE_TASK4 : (pos_index + 1);
        LCD_ShowString(0, (pos_index - UI_PAGE_TASK1) * 16, "  ", BLACK, WHITE, 16, 0);
        LCD_ShowString(0, ((pos_index - UI_PAGE_TASK1) + 1) * 16, ">>", BLACK, WHITE, 16, 0);
    }
    if (keys[3].singleFlag == 1)   // 取消按键，返回上一页面
    {
        keys[3].singleFlag = 0;
        current_index = UI_PAGE_MAIN;
        pos_index = UI_PAGE_TASK1;
        LCD_Fill(0, 16, LCD_W, LCD_H, WHITE);
        LCD_ShowString(0, 16, ">>", BLACK, WHITE, 16, 0);   // 屏幕指针复位
    }
    if (keys[4].singleFlag == 1)   // 确认按键，进入下一页面
    {
        keys[4].singleFlag = 0;
        current_index = UI_PAGE_RUN;
        pos_index = UI_PAGE_TASK1;
        LCD_Fill(0, 16, LCD_W, LCD_H, WHITE);
        LCD_ShowString(0, 16, ">>", BLACK, WHITE, 16, 0);   // 屏幕指针复位
    }
    return current_index;
}



/********************************二级页面*********************************/
/************************************************************************/
// 二级页面(MAIN->CONFIG->PID)
s32 ui_page_pid(s32 current_index)
{
    static u32 pos_index = UI_PAGE_KP;
    if (keys[1].singleFlag == 1)   // 上移按键，维持当前页面
    {
        keys[1].singleFlag = 0;
        pos_index = (pos_index - 1) < UI_PAGE_KP ? UI_PAGE_KP : (pos_index - 1);
        LCD_ShowString(0, ((pos_index - UI_PAGE_KP) + 2) * 16, "  ", BLACK, WHITE, 16, 0);   // 屏幕指针更新
        LCD_ShowString(0, ((pos_index - UI_PAGE_KP) + 1) * 16, ">>", BLACK, WHITE, 16, 0);
    }
    if (keys[2].singleFlag == 1)   // 下移按键，维持当前页面
    {
        keys[2].singleFlag = 0;
        pos_index = (pos_index + 1) > UI_PAGE_KD ? UI_PAGE_KD : (pos_index + 1);
        LCD_ShowString(0, (pos_index - UI_PAGE_KP) * 16, "  ", BLACK, WHITE, 16, 0);   // 屏幕指针更新
        LCD_ShowString(0, ((pos_index - UI_PAGE_KP) + 1) * 16, ">>", BLACK, WHITE, 16, 0);
    }
    if (keys[3].singleFlag == 1)   // 取消按键，返回上一页面
    {
        keys[3].singleFlag = 0;
        current_index = UI_PAGE_CONFIG;
        pos_index = UI_PAGE_KP;
        LCD_Fill(0, 16, LCD_W, LCD_H, WHITE);
        LCD_ShowString(0, 16, ">>", BLACK, WHITE, 16, 0);   // 屏幕指针复位
    }
    if (keys[4].singleFlag == 1)   // 确认按键，进入下一页面
    {
        keys[4].singleFlag = 0;
        current_index = pos_index;
        pos_index = UI_PAGE_KP;
        LCD_Fill(0, 16, LCD_W, LCD_H, WHITE);
        // LCD_ShowString(0, 16, ">>", BLACK, WHITE, 16, 0);//由于下一页面该指针固定，所以屏幕指针不更新
    }
    // 显示当前页面
    return current_index;
}

//(MAIN->CONFIG->GRAY)
s32 ui_page_gray(s32 current_index)
{
    static u32 pos_index = UI_PAGE_GRAY_CH0;
    if (keys[1].singleFlag == 1)   // 上移按键，维持当前页面
    {
        keys[1].singleFlag = 0;
        pos_index = (pos_index - 1) < UI_PAGE_GRAY_CH0 ? UI_PAGE_GRAY_CH0 : (pos_index - 1);
        LCD_ShowString(0, ((pos_index - UI_PAGE_GRAY_CH0) + 2) * 16, "  ", BLACK, WHITE, 16, 0);   // 屏幕指针更新
        LCD_ShowString(0, ((pos_index - UI_PAGE_GRAY_CH0) + 1) * 16, ">>", BLACK, WHITE, 16, 0);
    }
    if (keys[2].singleFlag == 1)   // 下移按键，维持当前页面
    {
        keys[2].singleFlag = 0;
        pos_index = (pos_index + 1) > UI_PAGE_GRAY_CH7 ? UI_PAGE_GRAY_CH7 : (pos_index + 1);
        LCD_ShowString(0, (pos_index - UI_PAGE_GRAY_CH0) * 16, "  ", BLACK, WHITE, 16, 0);   // 屏幕指针更新
        LCD_ShowString(0, ((pos_index - UI_PAGE_GRAY_CH0) + 1) * 16, ">>", BLACK, WHITE, 16, 0);
    }
    if (keys[3].singleFlag == 1)   // 取消按键，返回上一页面
    {
        keys[3].singleFlag = 0;
        current_index = UI_PAGE_CONFIG;
        pos_index = UI_PAGE_GRAY_CH0;
        LCD_Fill(0, 16, LCD_W, LCD_H, WHITE);
        LCD_ShowString(0, 16, ">>", BLACK, WHITE, 16, 0);   // 屏幕指针复位
    }
    if (keys[4].singleFlag == 1)   // 确认按键，进入下一页面
    {
        keys[4].singleFlag = 0;
        current_index = UI_PAGE_GRAY;
        pos_index = UI_PAGE_GRAY_CH0;
        LCD_Fill(0, 16, LCD_W, LCD_H, WHITE);
        LCD_ShowString(0, 16, ">>", BLACK, WHITE, 16, 0);   // 屏幕指针复位
    }
    // 显示当前页面
    return current_index;
}


/********************************三级页面*********************************/
/************************************************************************/
//(MAIN->CONFIG->PID->KP)
s32 ui_page_kp(s32 current_index)
{
    if (keys[1].singleFlag == 1)   // 上移按键，维持当前页面
    {
        keys[1].singleFlag = 0;
        pid[0] = (pid[0] + 0.01) > pid_max_val ? pid_max_val : (pid[0] + 0.01);
        LCD_ShowFloatNum(48, 16, pid[0], 4, BLACK, WHITE, 16);   // 更新Kp值显示
    }
    if (keys[2].singleFlag == 1)   // 下移按键，维持当前页面
    {
        keys[2].singleFlag = 0;
        pid[0] = (pid[0] - 0.01) < pid_min_val ? pid_min_val : (pid[0] - 0.01);
        LCD_ShowFloatNum(48, 16, pid[0], 4, BLACK, WHITE, 16);   // 更新Kp值显示
    }
    if (keys[3].singleFlag == 1)   // 取消按键，返回上一页面
    {
        keys[3].singleFlag = 0;
        current_index = UI_PAGE_PID;
        LCD_Fill(0, 16, LCD_W, LCD_H, WHITE);
        LCD_ShowString(0, 16, ">>", BLACK, WHITE, 16, 0);   // 屏幕指针复位
    }
    if (keys[4].singleFlag == 1)   // 确认按键，进入下一页面
    {
        keys[4].singleFlag = 0;
        current_index = UI_PAGE_SAVE_PID;
        LCD_Fill(0, 16, LCD_W, LCD_H, WHITE);
    }
    // 显示当前页面
    return current_index;
}

//(MAIN->CONFIG->PID->KI)
s32 ui_page_ki(s32 current_index)
{
    if (keys[1].singleFlag == 1)   // 上移按键，维持当前页面
    {
        keys[1].singleFlag = 0;
        pid[1] = (pid[1] + 0.01) > pid_max_val ? pid_max_val : (pid[1] + 0.01);
        LCD_ShowFloatNum(48, 16, pid[1], 4, BLACK, WHITE, 16);   // 更新Kp值显示
    }
    if (keys[2].singleFlag == 1)   // 下移按键，维持当前页面
    {
        keys[2].singleFlag = 0;
        pid[1] = (pid[1] - 0.01) < pid_min_val ? pid_min_val : (pid[1] - 0.01);
        LCD_ShowFloatNum(48, 16, pid[1], 4, BLACK, WHITE, 16);   // 更新Kp值显示
    }
    if (keys[3].singleFlag == 1)   // 取消按键，返回上一页面
    {
        keys[3].singleFlag = 0;
        current_index = UI_PAGE_PID;
        LCD_Fill(0, 16, LCD_W, LCD_H, WHITE);
        LCD_ShowString(0, 16, ">>", BLACK, WHITE, 16, 0);   // 屏幕指针复位
    }
    if (keys[4].singleFlag == 1)   // 确认按键，进入下一页面
    {
        keys[4].singleFlag = 0;
        current_index = UI_PAGE_SAVE_PID;
        LCD_Fill(0, 16, LCD_W, LCD_H, WHITE);
    }
    // 显示当前页面
    return current_index;
}

//(MAIN->CONFIG->PID->KD)
s32 ui_page_kd(s32 current_index)
{
    if (keys[1].singleFlag == 1)   // 上移按键，维持当前页面
    {
        keys[1].singleFlag = 0;
        pid[2] = (pid[2] + 0.01) > pid_max_val ? pid_max_val : (pid[2] + 0.01);
        LCD_ShowFloatNum(48, 16, pid[2], 4, BLACK, WHITE, 16);   // 更新Kp值显示
    }
    if (keys[2].singleFlag == 1)   // 下移按键，维持当前页面
    {
        keys[2].singleFlag = 0;
        pid[2] = (pid[2] - 0.01) < pid_min_val ? pid_min_val : (pid[2] - 0.01);
        LCD_ShowFloatNum(48, 16, pid[2], 4, BLACK, WHITE, 16);   // 更新Kp值显示
    }
    if (keys[3].singleFlag == 1)   // 取消按键，返回上一页面
    {
        keys[3].singleFlag = 0;
        current_index = UI_PAGE_PID;
        LCD_Fill(0, 16, LCD_W, LCD_H, WHITE);
        LCD_ShowString(0, 16, ">>", BLACK, WHITE, 16, 0);   // 屏幕指针复位
    }
    if (keys[4].singleFlag == 1)   // 确认按键，进入下一页面
    {
        keys[4].singleFlag = 0;
        current_index = UI_PAGE_SAVE_PID;
        LCD_Fill(0, 16, LCD_W, LCD_H, WHITE);
    }
    // 显示当前页面
    return current_index;
}



/********************************四级页面*********************************/
/************************************************************************/
//(MAIN->CONFIG->PID->KP/KI/KD->SAVE_PID)
s32 ui_page_save_pid(s32 current_index)
{
    if (keys[1].singleFlag == 1)   // 上移按键，维持当前页面
    {
        keys[1].singleFlag = 0;
    }
    if (keys[2].singleFlag == 1)   // 下移按键，维持当前页面
    {
        keys[2].singleFlag = 0;
    }
    if (keys[3].singleFlag == 1)   // 取消按键，返回上一页面
    {
        keys[3].singleFlag = 0;
        current_index = UI_PAGE_PID;
        LCD_Fill(0, 16, LCD_W, LCD_H, WHITE);
        LCD_ShowString(0, 16, ">>", BLACK, WHITE, 16, 0);   // 屏幕指针复位
    }
    if (keys[4].singleFlag == 1)   // 确认按键，进入下一页面
    {
        keys[4].singleFlag = 0;
        /*

            保存PID参数到FLASH

        */
        current_index = UI_PAGE_PID;
        LCD_Fill(0, 16, LCD_W, LCD_H, WHITE);
        LCD_ShowString(0, 16, ">>", BLACK, WHITE, 16, 0);   // 屏幕指针复位
    }
    // 显示当前页面
    return current_index;
}

