#include "app.h"
#include "controller.h"
#include "bsp.h"
#include "ui.h"
#include "usart.h"
#include "pid.h"

void app_setup(void)
{
    // wt901c_init();
    // key_init();
    debug_init(&huart1);
    debug_print("debug_init \n");
    Grayscale_Init();
    motor_init();
    wt901c_init();
    motor_set_speed(0, 0);
	  LCD_Init();
    Controller_Init();
	   
    LineControl(20);
    // ui_loop();
}

void app_loop(void)
{
    char str_x[6];
    //    char str_y[6];
    //    char str_z[6];
    //    uint16_t data_list[8] = {0};
    while (1) {
        
       

        //    HAL_Delay(1000);
        //		LCD_ShowIntNum(0,0,0,4,BLACK,WHITE,16);
        //		Emm_V5_Vel_Control(1, 1, 1500, 10, false);

        //HAL_Delay(1000);
        //LCD_ShowIntNum(0, 0, 0, 4, BLACK, WHITE, 16);
        //		Emm_V5_Vel_Control(1, 1, 1500, 10, false);

        //    Grayscale_SendCommand();
        // Grayscale_ReadAllChannels(data_list);
        //    LCD_ShowIntNum(0,0,data_list[0],4,BLACK,WHITE,16);
        // LCD_ShowIntNum(0,16,data_list[1],4,BLACK,WHITE,16);
        // LCD_ShowIntNum(0,32,data_list[2],4,BLACK,WHITE,16);
        // LCD_ShowIntNum(0,48,data_list[3],4,BLACK,WHITE,16);
        // LCD_ShowIntNum(0,64,data_list[4],4,BLACK,WHITE,16);
        // LCD_ShowIntNum(0,80,data_list[5],4,BLACK,WHITE,16);
        // LCD_ShowIntNum(0,96,data_list[6],4,BLACK,WHITE,16);
        // LCD_ShowIntNum(0,112,data_list[7],4,BLACK,WHITE,16);

        // uint8_t hello[3]={1,2,3};
        // HAL_UART_Transmit(&huart1,(uint8_t *)hello,3,0xffff);
        // delay_ms(500);

        // uint8_t channel[8]={0};
        // channel[0]=ADS7828_read_onedata(0);
        // HAL_UART_Transmit(&huart1,(uint8_t *)channel,8,0xffff);
        // delay_ms(50);
    }
}
