#include <mylib/device/device_rgb_led.h>

void rgb_led_set(rgb_led_t* led, bool r, bool g, bool b)
{
    if (r) {
        if (VALID_ON_LOW_LEVEL == led->valid) { DL_GPIO_clearPins(led->port_r, led->pin_r); }
        else {
            DL_GPIO_setPins(led->port_r, led->pin_r);
        }
    }
    else {
        if (VALID_ON_LOW_LEVEL == led->valid) { DL_GPIO_setPins(led->port_r, led->pin_r); }
        else {
            DL_GPIO_clearPins(led->port_r, led->pin_r);
        }
    }

    if (g) {
        if (VALID_ON_LOW_LEVEL == led->valid) { DL_GPIO_clearPins(led->port_g, led->pin_g); }
        else {
            DL_GPIO_setPins(led->port_g, led->pin_g);
        }
    }
    else {
        if (VALID_ON_LOW_LEVEL == led->valid) { DL_GPIO_setPins(led->port_g, led->pin_g); }
        else {
            DL_GPIO_clearPins(led->port_g, led->pin_g);
        }
    }

    if (b) {
        if (VALID_ON_LOW_LEVEL == led->valid) { DL_GPIO_clearPins(led->port_b, led->pin_b); }
        else {
            DL_GPIO_setPins(led->port_b, led->pin_b);
        }
    }
    else {
        if (VALID_ON_LOW_LEVEL == led->valid) { DL_GPIO_setPins(led->port_b, led->pin_b); }
        else {
            DL_GPIO_clearPins(led->port_b, led->pin_b);
        }
    }
}
