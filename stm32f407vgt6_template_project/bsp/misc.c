#include "misc.h"

// #define BUZZER_Pin GPIO_PIN_11
extern TIM_HandleTypeDef htim14;

void led_on(void)
{
    HAL_GPIO_WritePin(GPIOC, LED_1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, LED_2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, LED_3_Pin, GPIO_PIN_SET);
}

void led_off(void)
{
    HAL_GPIO_WritePin(GPIOC, LED_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, LED_2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, LED_3_Pin, GPIO_PIN_RESET);
}

void led_toggle(void)
{
    HAL_GPIO_TogglePin(GPIOC, LED_1_Pin);
    HAL_GPIO_TogglePin(GPIOC, LED_2_Pin);
    HAL_GPIO_TogglePin(GPIOC, LED_3_Pin);
}

static u8 led_timer = 0;

void led_open_auto_close(u8 times)
{
    led_on();
    led_timer = times;
}

void led_irq_handler(void)
{
    if (led_timer == 0) {
        led_off();
        return;
    }
    if (led_timer > 0) {
        led_timer--;
    }
    led_toggle();
}

void buzzer_on(void)
{
    HAL_GPIO_WritePin(GPIOD, BUZZER_Pin, GPIO_PIN_SET);
}

void buzzer_off(void)
{
    HAL_GPIO_WritePin(GPIOD, BUZZER_Pin, GPIO_PIN_RESET);
}

void buzzer_toggle(void)
{
    HAL_GPIO_TogglePin(GPIOD, BUZZER_Pin);
}

static u8 buzzer_timer = 0;

void buzzer_open_auto_close(u8 times)
{
    buzzer_on();
    buzzer_timer = times;
}

void buzzer_irq_handler(void)
{
    if (buzzer_timer == 0) {
        buzzer_off();
    }
    if (buzzer_timer > 0) {
        buzzer_timer--;
    }
}


// 激光笔
void laser_on(void)
{
    HAL_GPIO_WritePin(LASER_GPIO_Port, LASER_Pin, GPIO_PIN_SET);
}

void laser_off(void)
{
    HAL_GPIO_WritePin(LASER_GPIO_Port, LASER_Pin, GPIO_PIN_RESET);
}

// 获取电源电压
f32 get_power_voltage(void)
{
    // 启动ADC转换
    HAL_ADC_Start(&hadc1);
    // 等待转换完成
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    // 获取转换结果
    u16 val = HAL_ADC_GetValue(&hadc1);
    return (f32)val * 3.3 / 4096;
}

// 获取角度传感器的数据
f32 get_angle_data(void)
{
    // 启动ADC转换
    HAL_ADC_Start(&hadc1);
    // 等待转换完成
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    // 获取转换结果
    u16 val = HAL_ADC_GetValue(&hadc1);
    // 这里还没有进行角度转换
    return (f32)val;
}

struct key_t keys[5] = {0};

// 按键初始化
void key_init(void)
{
    HAL_TIM_Base_Start_IT(&htim9);   // Internal 10ms timer for key scan
}

// 按键扫描函数，(TIM9-10ms中断内部使用)
void scan_key(void)
{
    keys[1].keyState = HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin);   // 上移
    keys[2].keyState = HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin);   // 下移
    keys[3].keyState = HAL_GPIO_ReadPin(KEY_3_GPIO_Port, KEY_3_Pin);   // 取消
    keys[4].keyState = HAL_GPIO_ReadPin(KEY_3_GPIO_Port, KEY_4_Pin);   // 确认
    for (u32 i = 1; i <= 4; i++) {
        switch (keys[i].judgeState) {
            case 0:
                if (keys[i].keyState == 0) {
                    keys[i].judgeState = 1;
                    keys[i].time = 0;
                }
                break;
            case 1:
                if (keys[i].keyState == 0) {
                    keys[i].judgeState = 2;
                }
                else {
                    keys[i].judgeState = 0;
                }
                break;
            case 2:
                if (keys[i].keyState == 0) {
                    keys[i].time++;
                }
                else {
                    if (keys[i].time < 200) {
                        keys[i].singleFlag = 1;
                    }
                    else {
                        keys[i].longFlag = 1;
                    }
                    keys[i].judgeState = 0;
                }
                break;
        }
    }
}
