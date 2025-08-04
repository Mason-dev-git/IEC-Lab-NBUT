#include "ui.h"

extern s32 target_turn_cnt;
extern s32 current_turn_cnt;
extern int32_t question;

typedef s32 (*ui_handler_t)(s32);
float pid[3] = {0};   // Kp, Ki, Kd
char buf[32];
int32_t key = KEY_VALUE_NONE;
bool is_ui_running = true;
// 获取页面处理函数(内部使用)
ui_handler_t get_page_handler(s32 index)
{
    switch (index) {
        case UI_PAGE_MAIN: return ui_page_main;
        case UI_PAGE_BASIC: return ui_page_basic;
        case UI_PAGE_ADVANCE: return ui_page_advance;
        case UI_PAGE_TASK1: return ui_page_task1;
        case UI_PAGE_TASK2: return ui_page_task2;
        case UI_PAGE_TASK3: return ui_page_task3;
        case UI_PAGE_TASK4: return ui_page_task4;
        case UI_PAGE_TASK5: return ui_page_task5;
        case UI_PAGE_TASK6: return ui_page_task6;
        // other page
        default: return NULL;
    }
}





// UI界面主循环
void ui_loop(void)
{
    buf[0] = '\0';
    s32 current = UI_PAGE_MAIN;
    ui_handler_t handler;
    sprintf(buf, "<<");
    OLED_ShowString(110, 0, (u8*)buf, 16);
    while (is_ui_running) {
        key = key_get_value();
        handler = get_page_handler(current);
        current = handler(current);
        switch (current) {
            case UI_PAGE_NULL: break;
            case UI_PAGE_MAIN:
                sprintf(buf, "basic");
                OLED_ShowString(0, 0, (u8*)buf, 16);
                sprintf(buf, "advance");
                OLED_ShowString(0, 2, (u8*)buf, 16);
                break;
            case UI_PAGE_BASIC:
                sprintf(buf, "task 1");
                OLED_ShowString(0, 0, (u8*)buf, 16);
                sprintf(buf, "task 2");
                OLED_ShowString(0, 2, (u8*)buf, 16);
                sprintf(buf, "task 3");
                OLED_ShowString(0, 4, (u8*)buf, 16);
                break;
            case UI_PAGE_ADVANCE:
                sprintf(buf, "task 4");
                OLED_ShowString(0, 0, (u8*)buf, 16);
                sprintf(buf, "task 5");
                OLED_ShowString(0, 2, (u8*)buf, 16);
                sprintf(buf, "task 6");
                OLED_ShowString(0, 4, (u8*)buf, 16);
                break;
            case UI_PAGE_TASK1:
                sprintf(buf, "run times:");
                OLED_ShowString(0, 0, (u8*)buf, 16);
                break;
            case UI_PAGE_TASK2:
                sprintf(buf, "stop");
                OLED_ShowString(0, 0, (u8*)buf, 16);
                sprintf(buf, "run");
                OLED_ShowString(0, 2, (u8*)buf, 16);
                break;
            case UI_PAGE_TASK3:
                sprintf(buf, "stop");
                OLED_ShowString(0, 0, (u8*)buf, 16);
                sprintf(buf, "run");
                OLED_ShowString(0, 2, (u8*)buf, 16);
                break;
            case UI_PAGE_TASK4:
                sprintf(buf, "stop");
                OLED_ShowString(0, 0, (u8*)buf, 16);
                sprintf(buf, "run");
                OLED_ShowString(0, 2, (u8*)buf, 16);
                break;
            case UI_PAGE_TASK5:
                sprintf(buf, "stop");
                OLED_ShowString(0, 0, (u8*)buf, 16);
                sprintf(buf, "run");
                OLED_ShowString(0, 2, (u8*)buf, 16);
                break;
            case UI_PAGE_TASK6:
                sprintf(buf, "stop");
                OLED_ShowString(0, 0, (u8*)buf, 16);
                sprintf(buf, "run");
                OLED_ShowString(0, 2, (u8*)buf, 16);
                break;
            default: break;
        }
        delay_ms(50);
    }
}


/*********************************主页面**********************************/
/************************************************************************/
//(MAIN)
s32 ui_page_main(s32 current_index)
{
    static u32 pos_index = UI_PAGE_BASIC;
    if (keys[0].singleFlag == 1)   // 上移按键，维持当前页面
    {
        keys[0].singleFlag = 0;
        pos_index = (pos_index - 1) < UI_PAGE_BASIC ? UI_PAGE_BASIC : (pos_index - 1);   // 目标页面指针更新
        sprintf(buf, "  ");
        OLED_ShowString(110, (pos_index - UI_PAGE_BASIC) * 2 + 2, (u8*)buf, 16);
        sprintf(buf, "<<");
        OLED_ShowString(110, (pos_index - UI_PAGE_BASIC) * 2, (u8*)buf, 16);
    }
    if (keys[1].singleFlag == 1)   // 下移按键，维持当前页面
    {
        keys[1].singleFlag = 0;
        pos_index = (pos_index + 1) > UI_PAGE_ADVANCE ? UI_PAGE_ADVANCE : (pos_index + 1);
        sprintf(buf, "  ");
        OLED_ShowString(110, (pos_index - UI_PAGE_BASIC) * 2 - 2, (u8*)buf, 16);
        sprintf(buf, "<<");
        OLED_ShowString(110, (pos_index - UI_PAGE_BASIC) * 2, (u8*)buf, 16);
    }
    if (keys[2].singleFlag == 1)   // 取消按键，返回上一页面
    {
        keys[2].singleFlag = 0;
        current_index = UI_PAGE_MAIN;
        pos_index = UI_PAGE_BASIC;
        OLED_Clear();
        sprintf(buf, "<<");
        OLED_ShowString(110, 0, (u8*)buf, 16);   // 屏幕指针复位
    }
    if (keys[3].singleFlag == 1)   // 确认按键，进入下一页面
    {
        keys[3].singleFlag = 0;
        current_index = pos_index;
        pos_index = UI_PAGE_BASIC;
        OLED_Clear();
        sprintf(buf, "<<");
        OLED_ShowString(110, 0, (u8*)buf, 16);   // 屏幕指针复位
    }
    return current_index;
}


/********************************一级页面*********************************/
/************************************************************************/
//(MAIN->BASIC)
s32 ui_page_basic(s32 current_index)
{
    static u32 pos_index = UI_PAGE_TASK1;
    if (keys[0].singleFlag == 1)   // 上移按键，维持当前页面
    {
        keys[0].singleFlag = 0;
        pos_index = (pos_index - 1) < UI_PAGE_TASK1 ? UI_PAGE_TASK1 : (pos_index - 1);
        sprintf(buf, "  ");
        OLED_ShowString(111, (pos_index - UI_PAGE_TASK1) * 2 + 2, (u8*)buf, 16);
        sprintf(buf, "<<");
        OLED_ShowString(111, (pos_index - UI_PAGE_TASK1) * 2, (u8*)buf, 16);
    }
    if (keys[1].singleFlag == 1)   // 下移按键，维持当前页面
    {
        keys[1].singleFlag = 0;
        pos_index = (pos_index + 1) > UI_PAGE_TASK3 ? UI_PAGE_TASK3 : (pos_index + 1);
        sprintf(buf, "  ");
        OLED_ShowString(111, (pos_index - UI_PAGE_TASK1) * 2 - 2, (u8*)buf, 16);
        sprintf(buf, "<<");
        OLED_ShowString(111, (pos_index - UI_PAGE_TASK1) * 2, (u8*)buf, 16);
    }
    if (keys[2].singleFlag == 1)   // 取消按键，返回上一页面
    {
        keys[2].singleFlag = 0;
        current_index = UI_PAGE_MAIN;
        pos_index = UI_PAGE_TASK1;
        OLED_Clear();
        sprintf(buf, "<<");
        OLED_ShowString(111, 0, (u8*)buf, 16);   // 屏幕指针复位
    }
    if (keys[3].singleFlag == 1)   // 确认按键，进入下一页面
    {
        keys[3].singleFlag = 0;
        current_index = pos_index;
        pos_index = UI_PAGE_TASK1;
        OLED_Clear();
    }
    return current_index;
}

//(MAIN->ADVANCE)
s32 ui_page_advance(s32 current_index)
{
    static u32 pos_index = UI_PAGE_TASK4;
    if (keys[0].singleFlag == 1)   // 上移按键，维持当前页面
    {
        keys[0].singleFlag = 0;
        pos_index = (pos_index - 1) < UI_PAGE_TASK4 ? UI_PAGE_TASK4 : (pos_index - 1);
        sprintf(buf, "  ");
        OLED_ShowString(111, (pos_index - UI_PAGE_TASK4) * 2 + 2, (u8*)buf, 16);
        sprintf(buf, "<<");
        OLED_ShowString(111, (pos_index - UI_PAGE_TASK4) * 2, (u8*)buf, 16);
    }
    if (keys[1].singleFlag == 1)   // 下移按键，维持当前页面
    {
        keys[1].singleFlag = 0;
        pos_index = (pos_index + 1) > UI_PAGE_TASK6 ? UI_PAGE_TASK6 : (pos_index + 1);
        sprintf(buf, "  ");
        OLED_ShowString(111, (pos_index - UI_PAGE_TASK4) * 2 - 2, (u8*)buf, 16);
        sprintf(buf, "<<");
        OLED_ShowString(111, (pos_index - UI_PAGE_TASK4) * 2, (u8*)buf, 16);
    }
    if (keys[2].singleFlag == 1)   // 取消按键，返回上一页面
    {
        keys[2].singleFlag = 0;
        current_index = UI_PAGE_MAIN;
        pos_index = UI_PAGE_TASK4;
        OLED_Clear();
        sprintf(buf, "<<");
        OLED_ShowString(111, 0, (u8*)buf, 16);   // 屏幕指针复位
    }
    if (keys[3].singleFlag == 1)   // 确认按键，进入下一页面
    {
        keys[3].singleFlag = 0;
        current_index = pos_index;
        pos_index = UI_PAGE_TASK4;
        OLED_Clear();
    }
    return current_index;
}



/********************************二级页面*********************************/
/************************************************************************/
// 二级页面(MAIN->ADVANCE->TASK1)
s32 ui_page_task1(s32 current_index)
{
    static int run_times = 1;
    static u32 pos_index = UI_PAGE_TASK1;
    sprintf(buf, "%d", run_times);
    OLED_ShowString(80, 0, (u8*)buf, 16);
    if (keys[0].singleFlag == 1)   // 上移按键，维持当前页面
    {
        keys[0].singleFlag = 0;
        run_times = run_times == 5 ? 1 : run_times + 1;
        sprintf(buf, "%d", run_times);
        OLED_ShowString(80, 0, (u8*)buf, 16);
    }
    if (keys[1].singleFlag == 1)   // 下移按键，维持当前页面
    {
        keys[1].singleFlag = 0;
        run_times = run_times == 1 ? 5 : run_times - 1;
        sprintf(buf, "%d", run_times);
        OLED_ShowString(80, 0, (u8*)buf, 16);
    }
    if (keys[2].singleFlag == 1)   // 取消按键，返回上一页面
    {
        keys[2].singleFlag = 0;
        run_times = 1;
        current_index = UI_PAGE_BASIC;
        pos_index = UI_PAGE_TASK1;
        OLED_Clear();
        sprintf(buf, "<<");
        OLED_ShowString(110, 0, (u8*)buf, 16);   // 屏幕指针复位
    }
    if (keys[3].singleFlag == 1)   // 确认按键，进入下一页面
    {
        keys[3].singleFlag = 0;
        target_turn_cnt = run_times * 4 + 1;
        run_times = 1;
        current_index = pos_index;
        pos_index = UI_PAGE_TASK1;
        OLED_Clear();
        /*发送串口信息给摄像头  小车发车  任务1启动


        */
        sprintf(buf, "task1 start");
        OLED_ShowString(20, 3, (u8*)buf, 16);   // 任务1启动，屏幕打印启动信息
        question = 1;                           // 当前问题编号
        is_ui_running = false;
    }
    // 显示当前页面
    return current_index;
}


s32 ui_page_task2(s32 current_index)
{
    static u32 pos_index = UI_PAGE_TASK2;
    if (keys[0].singleFlag == 1)   // 上移按键，维持当前页面
    {
        keys[0].singleFlag = 0;
    }
    if (keys[1].singleFlag == 1)   // 下移按键，维持当前页面
    {
        keys[1].singleFlag = 0;
    }
    if (keys[2].singleFlag == 1)   // 取消按键，返回上一页面
    {
        keys[2].singleFlag = 0;
        current_index = UI_PAGE_BASIC;
        pos_index = UI_PAGE_TASK2;
        OLED_Clear();
        sprintf(buf, "<<");
        OLED_ShowString(110, 0, (u8*)buf, 16);   // 屏幕指针复位
    }
    if (keys[3].singleFlag == 1)   // 确认按键，进入下一页面
    {
        keys[3].singleFlag = 0;
        current_index = pos_index;
        pos_index = UI_PAGE_TASK2;
        OLED_Clear();
        /*发送串口信息给摄像头  任务2启动


        */
        sprintf(buf, "task2 start");
        OLED_ShowString(20, 3, (u8*)buf, 16);   // 任务2启动，屏幕打印启动信息
        question = 2;                           // 当前问题编号
        is_ui_running = false;
    }
    // 显示当前页面
    return current_index;
}



s32 ui_page_task3(s32 current_index)
{
    static u32 pos_index = UI_PAGE_TASK3;
    if (keys[0].singleFlag == 1)   // 上移按键，维持当前页面
    {
        keys[0].singleFlag = 0;
    }
    if (keys[1].singleFlag == 1)   // 下移按键，维持当前页面
    {
        keys[1].singleFlag = 0;
    }
    if (keys[2].singleFlag == 1)   // 取消按键，返回上一页面
    {
        keys[2].singleFlag = 0;
        current_index = UI_PAGE_BASIC;
        pos_index = UI_PAGE_TASK3;
        OLED_Clear();
        sprintf(buf, "<<");
        OLED_ShowString(110, 0, (u8*)buf, 16);   // 屏幕指针复位
    }
    if (keys[3].singleFlag == 1)   // 确认按键，进入下一页面
    {
        keys[3].singleFlag = 0;
        current_index = pos_index;
        pos_index = UI_PAGE_TASK3;
        OLED_Clear();
        /*发送串口信息给摄像头  任务3启动


        */
        sprintf(buf, "task3 start");
        OLED_ShowString(20, 3, (u8*)buf, 16);   // 任务3启动，屏幕打印启动信息
        question = 3;                           // 当前问题编号
        is_ui_running = false;
    }
    // 显示当前页面
    return current_index;
}



s32 ui_page_task4(s32 current_index)
{
    static u32 pos_index = UI_PAGE_TASK4;
    if (keys[0].singleFlag == 1)   // 上移按键，维持当前页面
    {
        keys[0].singleFlag = 0;
    }
    if (keys[1].singleFlag == 1)   // 下移按键，维持当前页面
    {
        keys[1].singleFlag = 0;
    }
    if (keys[2].singleFlag == 1)   // 取消按键，返回上一页面
    {
        keys[2].singleFlag = 0;
        current_index = UI_PAGE_ADVANCE;
        pos_index = UI_PAGE_TASK4;
        OLED_Clear();
        sprintf(buf, "<<");
        OLED_ShowString(110, 0, (u8*)buf, 16);   // 屏幕指针复位
    }
    if (keys[3].singleFlag == 1)   // 确认按键，进入下一页面
    {
        keys[3].singleFlag = 0;
        current_index = pos_index;
        pos_index = UI_PAGE_TASK4;
        OLED_Clear();
        /*发送串口信息给摄像头  任务4启动


        */
        sprintf(buf, "task4 start");
        OLED_ShowString(20, 3, (u8*)buf, 16);   // 任务4启动，屏幕打印启动信息
        question = 4;                           // 当前问题编号
        is_ui_running = false;
    }
    // 显示当前页面
    return current_index;
}



s32 ui_page_task5(s32 current_index)
{
    static u32 pos_index = UI_PAGE_TASK5;
    if (keys[0].singleFlag == 1)   // 上移按键，维持当前页面
    {
        keys[0].singleFlag = 0;
    }
    if (keys[1].singleFlag == 1)   // 下移按键，维持当前页面
    {
        keys[1].singleFlag = 0;
    }
    if (keys[2].singleFlag == 1)   // 取消按键，返回上一页面
    {
        keys[2].singleFlag = 0;
        current_index = UI_PAGE_ADVANCE;
        pos_index = UI_PAGE_TASK5;
        OLED_Clear();
        sprintf(buf, "<<");
        OLED_ShowString(110, 0, (u8*)buf, 16);   // 屏幕指针复位
    }
    if (keys[3].singleFlag == 1)   // 确认按键，进入下一页面
    {
        keys[3].singleFlag = 0;
        current_index = pos_index;
        pos_index = UI_PAGE_TASK5;
        OLED_Clear();
        /*发送串口信息给摄像头  任务5启动


        */
        sprintf(buf, "task5 start");
        OLED_ShowString(20, 3, (u8*)buf, 16);   // 任务5启动，屏幕打印启动信息
        question = 5;                           // 当前问题编号
        is_ui_running = false;
    }
    // 显示当前页面
    return current_index;
}



s32 ui_page_task6(s32 current_index)
{
    static u32 pos_index = UI_PAGE_TASK6;
    if (keys[0].singleFlag == 1)   // 上移按键，维持当前页面
    {
        keys[0].singleFlag = 0;
    }
    if (keys[1].singleFlag == 1)   // 下移按键，维持当前页面
    {
        keys[1].singleFlag = 0;
    }
    if (keys[2].singleFlag == 1)   // 取消按键，返回上一页面
    {
        keys[2].singleFlag = 0;
        current_index = UI_PAGE_ADVANCE;
        pos_index = UI_PAGE_TASK6;
        OLED_Clear();
        sprintf(buf, "<<");
        OLED_ShowString(110, 0, (u8*)buf, 16);   // 屏幕指针复位
    }
    if (keys[3].singleFlag == 1)   // 确认按键，进入下一页面
    {
        keys[3].singleFlag = 0;
        current_index = pos_index;
        pos_index = UI_PAGE_TASK6;
        OLED_Clear();
        /*发送串口信息给摄像头  任务6启动


        */
        sprintf(buf, "task6 start");
        OLED_ShowString(20, 3, (u8*)buf, 16);   // 任务6启动，屏幕打印启动信息
        question = 6;                           // 当前问题编号
        is_ui_running = false;
    }
    // 显示当前页面
    return current_index;
}
