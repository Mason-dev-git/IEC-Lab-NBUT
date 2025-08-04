#include "Menu.h"
#include "string.h"

struct Option_Class Menu_SettingOptionList[];

void Set_CursorStyle(void)
{
    if (Menu_Global.CursorStyle == reverse) {
        Menu_Global.CursorStyle          = mouse;
        Menu_SettingOptionList[1].String = "光标风格[鼠标]";
    }
    else if (Menu_Global.CursorStyle == mouse) {
        Menu_Global.CursorStyle          = frame;
        Menu_SettingOptionList[1].String = "光标风格[矩形]";
    }
    else {
        Menu_Global.CursorStyle          = reverse;
        Menu_SettingOptionList[1].String = "光标风格[反相]";
    }
}

void Set_animation_speed(void)
{
    static int8_t State = 0;

    if (State == 0) {
        Menu_Global.Cursor_ActSpeed      = 0.2f;
        Menu_Global.Slide_ActSpeed       = 4.0f;
        Menu_SettingOptionList[2].String = "动画速度[快]";
        State++;
    }
    else if (State == 1) {
        Menu_Global.Cursor_ActSpeed      = 1.0f;
        Menu_Global.Slide_ActSpeed       = 16.0f;
        Menu_SettingOptionList[2].String = "动画速度[关]";
        State++;
    }
    else if (State == 2) {
        Menu_Global.Cursor_ActSpeed      = 0.1f;
        Menu_Global.Slide_ActSpeed       = 1.26f;
        Menu_SettingOptionList[2].String = "动画速度[慢]";
        State++;
    }
    else {
        Menu_Global.Cursor_ActSpeed      = 0.15f;
        Menu_Global.Slide_ActSpeed       = 2.0f;
        Menu_SettingOptionList[2].String = "动画速度[中]";
        State                            = 0;
    }
}

//其他参数，编码器按一下累加一次，达到10后重新累加
void Set_OtherParam(void)
{
    int8_t      State = Menu_Global.Other_param;
    static char str[20];

    if ((++State) > 10) State = 0;

    sprintf(str, "其他参数 [%02d]", State);

    Menu_SettingOptionList[3].String = str;
    Menu_Global.Other_param          = State;
}

struct Option_Class Menu_SettingOptionList[] = {{"返回主菜单..."},
                                                {"光标风格", Set_CursorStyle},
                                                {"动画速度", Set_animation_speed},
                                                {"其他参数", Set_OtherParam},
                                                {".."}};

//参数设置函数入口，这里新增一个 其他参数并初始化
void Menu_RunSettingMenu(void)
{
    //初始化其他参数
    static char str[20];
    sprintf(str, "其他参数 [%02d]", (int8_t)Menu_Global.Other_param);
    Menu_SettingOptionList[3].String = str;

    //初始化光标类型
    if (Menu_Global.CursorStyle == mouse) { Menu_SettingOptionList[1].String = "光标风格[鼠标]"; }
    else if (Menu_Global.CursorStyle == frame) {
        Menu_SettingOptionList[1].String = "光标风格[矩形]";
    }
    else if (Menu_Global.CursorStyle == reverse) {
        Menu_Global.CursorStyle          = reverse;
        Menu_SettingOptionList[1].String = "光标风格[反相]";
    }
    else {
        Menu_SettingOptionList[1].String = "光标风格[???]";
    }

    //初始化动画速度
    if (Menu_Global.Cursor_ActSpeed == 0.2f && Menu_Global.Slide_ActSpeed == 4.0f) {
        Menu_SettingOptionList[2].String = "动画速度[快]";
    }
    else if (Menu_Global.Cursor_ActSpeed == 1.0f && Menu_Global.Slide_ActSpeed == 16.0f) {
        Menu_SettingOptionList[2].String = "动画速度[关]";
    }
    else if (Menu_Global.Cursor_ActSpeed == 0.1f && Menu_Global.Slide_ActSpeed == 1.26f) {
        Menu_SettingOptionList[2].String = "动画速度[慢]";
    }
    else if (Menu_Global.Cursor_ActSpeed == 0.15f && Menu_Global.Slide_ActSpeed == 2.0f) {
        Menu_SettingOptionList[2].String = "动画速度[中]";
    }
    else {
        Menu_SettingOptionList[2].String = "动画速度[???]";
    }

    //显示【参数设置】菜单页面
    Menu_RunMenu(Menu_SettingOptionList);
}
