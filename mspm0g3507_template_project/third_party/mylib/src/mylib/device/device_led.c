//#include "device_led.h"
//#include "stm32f10x.h"
//#include <stdint.h>


///******************************************************************************
// * @brief      初始化led
// *
// * @param[in]  led   :  led结构体指针
// * @param[in]  stat  :  led的初始状态
// *
// * @return     无
// *
// ******************************************************************************/
// void led_init(led_t* led, led_state_enum state)
//{
//    gpio_level_enum level;
//    if ((led_state_on == state && led->valid == gpio_level_high) ||
//        (led_state_off == state && led->valid == gpio_level_low)) {
//        level = gpio_level_high;
//    }
//    else {
//        level = gpio_level_low;
//    }

//    gpio_init(led->gpio_pin, gpio_dir_out, level, GPIO_PIN_DEFAULT_CONFIG);

//    if (led_state_on == state) { led_on(led); }
//    else {
//        led_off(led);
//    }
//}

///******************************************************************************
// * @brief      打开led
// *
// * @param[in]  led   :  led结构体指针
// *
// * @return     无
// *
// ******************************************************************************/
// void led_on(led_t* led)
//{
//    gpio_write(led->gpio_pin, led->valid);
//}

///******************************************************************************
// * @brief      关闭led
// *
// * @param[in]  led   :  led结构体指针
// *
// * @return     无
// *
// ******************************************************************************/
// void led_off(led_t* led)
//{
//    gpio_write(led->gpio_pin, led->valid == gpio_level_low ? gpio_level_high : gpio_level_low);
//}
