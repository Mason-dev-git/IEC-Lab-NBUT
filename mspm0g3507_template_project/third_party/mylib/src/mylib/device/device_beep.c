#include <mylib/device/device_beep.h>

void beep_on(beep_t* beep)
{
    if (VALID_ON_LOW_LEVEL == beep->valid) { DL_GPIO_clearPins(beep->port, beep->pin); }
    else {
        DL_GPIO_setPins(beep->port, beep->pin);
    }
}

void beep_off(beep_t* beep)
{
    if (VALID_ON_LOW_LEVEL == beep->valid) { DL_GPIO_setPins(beep->port, beep->pin); }
    else {
        DL_GPIO_clearPins(beep->port, beep->pin);
    }
}
