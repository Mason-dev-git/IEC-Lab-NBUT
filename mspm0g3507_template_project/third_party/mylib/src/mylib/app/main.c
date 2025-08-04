#include "device_led.h"
#include "device_oled.h"
#include "driver_systick.h"
#include "stm32f10x.h"   // Device header
#include <string.h>
#include "menu.h"
#include "device_matrix_key.h"
#include "motor.h"
#include "FreeRTOS.h"
#include "task.h"

extern uint16_t tt_2_01;
extern uint16_t pulse2_num_01;
extern uint16_t tt_2_02;
extern uint16_t pulse2_num_02;
extern int stopconfig;   //引用lk.c定义的stopconfig，才能在main.c内使用stopconfig该变量

int sensor[4] = {
    0,
    0,
    0,
    0};   //定义一个数组，用于储存红外传感器返回的值，因为有九个传感器，所以定义数组内元素个数为4
int Prestopconfig = 0;   //定义一个变量，放到lk.c文件read_sensor函数里识别终点线的语句中

int    x = 0, y = 0;
int8_t flag = 0;
char   buf[20];
int    len = 0;

void test_task()
{
    while (1) { systick_delay_ms(300); }
}

void main_task()
{
    while (1) {
        // my_menu_run();
        //        step_on(&g_motor1);
        //        step_motor_angle(&g_motor1,angle1);
        //        step_on(&g_motor2);
        //        step_motor_angle(&g_motor2,angle2)oint aintmainoko [RJO


        Menu_RunMainMenu();
    }
}

void my_menu_run()
{
    // 清空
    OLED_Clear();
    int value = matrix_key_get_value();
    systick_delay_ms(100);
    if (value >= 16 || value < 0) return;
    // 获取键值
    char key = matrix_key_table[value];
    // OLED_ShowChar(1, 1, key, OLED_6X8);

    if (key == MATRIX_KEY_VALUE_CANCEL) {
        len--;
        buf[len] = '\0';
    }
    else {
        buf[len++] = key;
        buf[len]   = '\0';
    }
    OLED_ShowStringArea(1, 1, 128, 32, buf, OLED_6X8);

    // 刷新ui
    OLED_Update();

    //    for(int i = 0; i < len; i++){
    //        OLED_ShowChar(1+i,1,buf[i],OLED_6X8);
    //    }

    // x = x*10 + key-'0';
    // OLED_ShowNum(1,1,x,3,OLED_6X8);

    //    if(flag == 0){
    //        x = x*10 + key-'0';
    //    }
    //    else{
    //        y = y*10 + key-'0';
    //    }
    //    // 在缓冲区绘制ui
    //    OLED_ShowString(1,1,"x:",OLED_6X8);
    //    OLED_ShowNum(13,1,x,3,OLED_6X8);
    //    if(key == MATRIX_KEY_VALUE_ENTER){
    //        flag = 1;
    //        OLED_ShowString(1,2,"y:",OLED_6X8);
    //    }
    //    OLED_ShowNum(13,9,y,3,OLED_6X8);
}



int main(void)
{
    //    led_t led = {.gpio_pin = gpio_pin_c13, .valid = gpio_level_low};

    //    led_init(&led, led_state_on);

    //    int32_t angle1 = 180, angle2 = 360;
    //    int32_t angle1 = 180;   //右边电机
    //    int32_t angle2 = 180;   //左边电机
    systick_init();

    OLED_Init();
    matrix_key_init();


    motor_Init(&g_motor1);
    motor_Init(&g_motor2);


    xTaskCreate((TaskFunction_t)test_task, "test_task", 1000, NULL, 1, NULL);
    xTaskCreate((TaskFunction_t)main_task, "main_task", 1000, NULL, 1, NULL);


    /* Start the scheduler. */
    vTaskStartScheduler();

    // motor_set_direction(motor2, step_motor_dir_forward);

    // motor_start(motor2, 200);

    while (1) {
        //        if(motor2->pulse_count <= 0){
        //            //systick_delay_ms(5000);
        //            motor_set_direction(motor2, step_motor_dir_forward);
        //
        //            motor_start(motor2, 200);
        //        }
    }
    step_direction(&g_motor1, 1);
    //    step_direction(&g_motor2,1);



    //    while (1) {
    //        //开启电机
    //
    //       step_on(&g_motor1);
    //        step_on(&g_motor2);
    //// step_motor_angle(&g_motor1, angle1);
    //
    //        // 获取输入的坐标
    //        // ...

    //        // 计算需要的角度



    // 开始旋转电机方向
    // motor_set_direction(motor1, step_motor_dir_forward);


    //
    ////        motor_set_direction(&g_motor2, step_motor_dir_reversal);
    ////        motor_start(&g_motor2, angle2);

    //        // my_menu_run();
    ////        Menu_RunMainMenu();
    //        // Key_Reset_All();
    //    }






    //    OLED_Init();
    //    LED_Init();
    //    Motor_Init();
    //    //INFRARED_Init();
    //    ULTRASONIC_Init();
    //    USART_Config_Init();



    //

    //    OLED_Clear();
    // SysTick_Delay_Ms(2000);

    // OLED_Fill(0x00);
    // SysTick_Delay_Ms(2000);
    //    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    //    GPIO_InitTypeDef GPIO_InitStructure;
    //    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    //    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;
    //    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //    // GPIO_SetBits(GPIOC,GPIO_Pin_13);
    //    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    //    float dis = Get_distance();
    //    OLED_Clear();
    //    char  str[16]={0};
    //    sprintf(str,"%f",dis);
    //
    //    OLED_ShowString(0,0,(unsigned char*)str,16);
    //    while (1) {

    // 读取红外信号
    //        read_sensor();
    //
    ////        // OLED_ShowStr(0,0,"hello",1);
    // OLED_ShowString(12,3,(unsigned char*)"hello",16);
    //        Motor_Run();
    //        // LED1_ON();
    //        SysTick_Delay_Ms(1000);   //延时100ms
    //        Motor_Stop();
    //        SysTick_Delay_Ms(1000);
    //        Motor_Run();
    //        //Motor_Back();
    //        // LED1_OFF();
    //        SysTick_Delay_Ms(1000);
    //        Motor_Stop();
    //        SysTick_Delay_Ms(1000);
    //    }
}
