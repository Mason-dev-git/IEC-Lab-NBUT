#include "test.h"


void test_main(void)
{
    debug_init(&huart1);
    
    // u8 data[] = {0x00};
    // while(1){
    //     HAL_I2C_Mem_Write(&hi2c3, 0x50, 0x01, 1, data, 1, 1000);
    // }
    


    // test_wt901c();
    // test_debuger();
    // test_motor();
    test_grayscale();
    // test_ui();
    // test_tof400f();
    // test_gyro();

    test_flash();
}

void test_debuger(void)
{
    debug_init(&huart1);
    while (1) {
        debug_print("Test!\r\n");
    }
}

void test_lcd(void)
{
    LCD_Init();

    LCD_ShowString(0, 0, (const uint8_t*)"Test!", BLACK, WHITE, 16, 0);
    LCD_ShowString(0, 16, (const uint8_t*)"Test1", BLACK, WHITE, 16, 0);
    LCD_ShowString(0, 32, (const uint8_t*)"Test2", BLACK, WHITE, 16, 0);
    LCD_ShowString(0, 48, (const uint8_t*)"Test3", BLACK, WHITE, 16, 0);
    while (1)
        ;
}

void test_ui(void)
{
    LCD_Init();
    ui_loop();
}

void test_led(void)
{
    while(1){
        led_on();
        delay_ms(1000);
        led_off();
        delay_ms(1000);
        led_toggle();
        delay_ms(1000);
        led_toggle();
        delay_ms(1000);
    }
}

void test_buzzer(void)
{
    while(1){
        buzzer_on();
        delay_ms(1000);
        buzzer_off();
        delay_ms(1000);
        buzzer_toggle();
        delay_ms(1000);
        buzzer_toggle();
        delay_ms(1000);
    }
}

void test_tof400f(void)
{
    tofxxf_init(&huart6);
    while (1) {
        tofxxf_requst();
        s32 dis;
        do {
            delay_ms(50);
            dis = tofxxf_get_distance();
        } while (dis == -1);

        debug_print("dis=%d\n", dis);
        delay_ms(200);
    }
}

void test_gyro(void)
{
    wt901c_init();
    float angle[3];
    while (1) {
        parse_angle_frame(angle);
        debug_print("%.2f,%.2f,%.2f\n", angle[0], angle[1], angle[2]);
    }
}

void test_grayscale(void)
{
    uint8_t Digtal;
    while (1) {
        Digtal = IIC_Get_Digtal();
        debug_print("%d,%d,%d,%d,%d,%d,%d,%d\n",
                    (Digtal >> 0) & 0x01,
                    (Digtal >> 1) & 0x01,
                    (Digtal >> 2) & 0x01,
                    (Digtal >> 3) & 0x01,
                    (Digtal >> 4) & 0x01,
                    (Digtal >> 5) & 0x01,
                    (Digtal >> 6) & 0x01,
                    (Digtal >> 7) & 0x01);
    }
}


void test_motor(void)
{
    motor_init();
    motor_set_speed(3000, 3000);
    while (1)
        ;
}

#define SFUD_DEMO_TEST_BUFFER_SIZE  1024
static uint8_t sfud_demo_test_buf[SFUD_DEMO_TEST_BUFFER_SIZE];
void sfud_demo(uint32_t addr, size_t size, uint8_t *data)
{
    sfud_err result = SFUD_SUCCESS;
    extern sfud_flash *sfud_dev;
    const sfud_flash *flash = sfud_get_device(SFUD_W25_DEVICE_INDEX);
    size_t i;
    /* prepare write data */
    for (i = 0; i < size; i++)
    {
        data[i] = i;
    }
    /* erase test */
    result = sfud_erase(flash, addr, size);
    if (result == SFUD_SUCCESS)
    {
        debug_print("Erase the %s flash data finish. Start from 0x%08X, size is %zu.\r\n", flash->name, addr, size);
    }
    else
    {
        debug_print("Erase the %s flash data failed.\r\n", flash->name);
        return;
    }
    /* write test */
    result = sfud_write(flash, addr, size, data);
    if (result == SFUD_SUCCESS)
    {
        debug_print("Write the %s flash data finish. Start from 0x%08X, size is %zu.\r\n", flash->name, addr, size);
    }
    else
    {
        debug_print("Write the %s flash data failed.\r\n", flash->name);
        return;
    }
    /* read test */
    result = sfud_read(flash, addr, size, data);
    if (result == SFUD_SUCCESS)
    {
        debug_print("Read the %s flash data success. Start from 0x%08X, size is %zu. The data is:\r\n", flash->name, addr, size);
        debug_print("Offset (h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\r\n");
        for (i = 0; i < size; i++)
        {
            if (i % 16 == 0)
            {
                debug_print("[%08X] ", addr + i);
            }
            debug_print("%02X ", data[i]);
            if (((i + 1) % 16 == 0) || i == size - 1)
            {
                debug_print("\r\n");
            }
        }
        debug_print("\r\n");
    }
    else
    {
        debug_print("Read the %s flash data failed.\r\n", flash->name);
    }
    /* data check */
    for (i = 0; i < size; i++)
    {
        if (data[i] != i % 256)
        {
            debug_print("Read and check write data has an error. Write the %s flash data failed.\r\n", flash->name);
            break;
        }
    }
    if (i == size)
    {
        debug_print("The %s flash test is success.\r\n", flash->name);
        // HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_RESET);
    }
}
void test_flash(void)
{
    debug_init(&huart1);
    
    debug_print("Test!\r\n");
    
    if (sfud_init() == SFUD_SUCCESS) {
        sfud_demo(0, sizeof(sfud_demo_test_buf), sfud_demo_test_buf);
    } else {
        // HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_RESET);
    }
    while(1);
}

void test_wt901c(void)
{
    wt901c_init();
    float datas[3];
    while (1) {
        parse_angle_frame(datas);
        debug_print("%.2f,%.2f,%.2f\n", datas[0], datas[1], datas[2]);
        delay_ms(50);
    }
}
