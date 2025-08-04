#include "Menu.h"
#include "string.h"
#include "device_matrix_key.h"
#include "driver_systick.h"

void fun_page1()
{
    while (1) {
        OLED_Clear();

        OLED_ShowString(0, 0, "X: ", OLED_8X16);
        // 300秒内完成一个不限定轨迹形状，且长度100cm


        OLED_Update();
        if (Menu_EnterEvent()) { return; }
    }
}

void fun_page2()
{
    int8_t x = 0, y = 0;
    int8_t flag    = 0;
    char   buf[10] = {'\0'};
    int8_t len     = 0;
    char   str[16];
    // 一直在这个循环里
    // 每次循环都是绘制新的一帧
    while (1) {
        OLED_Clear();

        // OLED_ShowString(0, 0, "Set Point Info", OLED_8X16);


        // 获取键值
        int8_t value = matrix_key_get_value();
        // 去除非法的值
        if (value >= 16 || value < 0) continue;
        char key = matrix_key_table[value];
        // 防止键盘过于灵敏
        systick_delay_ms(100);

        if (flag == 0) {   // 输入x值的状态
            // 取消就是删除
            if (key == MATRIX_KEY_VALUE_CANCEL) {
                // 只有长度大于0的时候才能删除
                if (len > 0) {
                    len--;
                    buf[len] = '\0';
                }
            }
            else if (key == MATRIX_KEY_VALUE_ENTER) {
                flag++;   // 进入到下一阶段
                // 如果长度大于3，那么就只要前三个数字
                if (len > 3) len = 3;
                // 将缓冲区中的字符转为数字
                for (int i = 0; i < len; i++) x += buf[i] - '0';
                len = 0;   // 清空缓冲区
            }
            else {
                // 必须是数字，而且要防止缓冲区溢出
                if (key <= '0' && key >= '0' && len <= 9) {
                    buf[len++] = key;
                    buf[len]   = '\0';
                }
            }
        }
        else if (flag == 1) {
            // 取消就是删除
            if (key == MATRIX_KEY_VALUE_CANCEL) {
                // 只有长度大于0的时候才能删除
                if (len > 0) {
                    len--;
                    buf[len] = '\0';
                }
            }
            else if (key == MATRIX_KEY_VALUE_ENTER) {
                flag++;   // 进入到下一阶段
                // 如果长度大于3，那么就只要前三个数字
                if (len > 3) len = 3;
                // 将缓冲区中的字符转为数字
                for (int i = 0; i < len; i++) y += buf[i] - '0';
                len = 0;   // 清空缓冲区
            }
            else {
                // 必须是数字，而且要防止缓冲区溢出
                if (key <= '0' && key >= '0' && len <= 9) {
                    buf[len++] = key;
                    buf[len]   = '\0';
                }
            }
        }
        else {
            if (Menu_EnterEvent()) { return; }
        }

        // 根据不同情况选择显示的内容
        if (flag == 0) {
            sprintf(str, "X:%s", buf);
            OLED_ShowStringArea(1, 1, 128, 32, str, OLED_6X8);
        }
        else if (flag == 1) {
            sprintf(str, "X:%d,y:%s", x, buf);
            OLED_ShowStringArea(1, 1, 128, 32, str, OLED_6X8);
        }
        else {
            sprintf(str, "X:%d,y:%d", x, y);
            OLED_ShowStringArea(1, 1, 128, 32, str, OLED_6X8);
        }

        OLED_ShowString(0, 0, "Set Point Info", OLED_8X16);
        // 设置坐标
        //        char key = matrix_key_table[matrix_key_get_value()];
        //        systick_delay_ms(100);

        //        char    buf[20];
        //        int16_t len = 0;
        //        int16_t x = 0, y = 0;
        //        int16_t value = matrix_key_get_value();
        //        systick_delay_ms(100);
        //        if (value >= 16 || value < 0) return;
        //        // 获取键值
        //        char key = matrix_key_table[value];
        //        // OLED_ShowChar(1, 1, key, OLED_6X8);

        //        if (key == MATRIX_KEY_VALUE_CANCEL) {
        //            len--;
        //            buf[len] = '\0';
        //        }
        //        else {
        //            buf[len++] = key;
        //            buf[len]   = '\0';
        //        }
        //        OLED_ShowStringArea(1, 1, 128, 32, buf, OLED_6X8);


        OLED_Update();
        // if (Menu_EnterEvent()) { return; }
    }
}

void Menu_RunMainMenu(void)
{
    static struct Option_Class Menu_StartOptionList[] = {
        //{"返回..."},
        {"submenu test...", Menu_RunSubMenu},   // 子菜单

        {"run 100cm.", fun_page1},   // 300秒内完成一个不限定轨迹形状，且长度100cm
        {"set point", fun_page2},    // 设置坐标

        {"param config ...", Menu_RunSettingMenu},   // 设置
        {"about ...", Menu_Information},             // 信息
        {"..."}};

    Menu_RunMenu(Menu_StartOptionList);
}

void subMenu_Fun1(void)
{
    while (1) {
        OLED_Clear();

        OLED_ShowString(0, 0, "Menu V1.8", OLED_8X16);
        OLED_ShowString(0, 48, "test submenu-1", OLED_8X16);

        OLED_Update();
        if (Menu_EnterEvent()) { return; }
    }
}

void subMenu_Fun2(void)
{
    while (1) {
        OLED_Clear();

        OLED_ShowString(0, 0, "Menu V1.8", OLED_8X16);
        OLED_ShowString(0, 48, "test submenu-2", OLED_8X16);

        OLED_Update();
        if (Menu_EnterEvent()) { return; }
    }
}

void Menu_RunSubMenu(void)
{
    static struct Option_Class Menu_SubMenuList[] = {{"back mainmenu..."},
                                                     {"submenu 1...", subMenu_Fun1},
                                                     {"submenu 2...", subMenu_Fun2},
                                                     {".."}};

    Menu_RunMenu(Menu_SubMenuList);
}

void Menu_Information(void)
{
    // uint8_t Angle = 233;
    while (1) {
        OLED_Clear();

        //狗头LOGO
        OLED_ShowImage(88, 24, 32, 32, goutou);

        //		OLED_Rotation_Block(88 + 16, 8 + 16, 16, Angle * 360 / 256);
        //		Angle += Menu_RollEvent() * 8;
        //		Angle += 2;

        OLED_ShowString(2, 0, "thanks", OLED_8X16);
        OLED_ShowString(2, 32, "Menu V1.8", OLED_8X16);
        OLED_ShowString(2, 48, "2024.5.3", OLED_8X16);

        OLED_Update();
        if (Menu_EnterEvent()) { return; }
    }
}

/**********************************************************/
