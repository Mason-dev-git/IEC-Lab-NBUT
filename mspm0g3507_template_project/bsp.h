#ifndef MYLIB_CONFIG_H
#define MYLIB_CONFIG_H

// 使用debug需要定义这个宏
#define USE_DEBUG_MODE 1


#ifdef __MSPM0G3507__

// mspm0g3507
#    define PLATFORM_MSPM0 1

#endif   // __MSPM0G3507__

// mspm0使用下面这个头文件
#if PLATFORM_MSPM0

#    include <ti/devices/msp/msp.h>
#    include <ti/driverlib/driverlib.h>
#    include <ti/driverlib/m0p/dl_core.h>

#endif

// 低电平有效
#define VALID_ON_LOW_LEVEL 0
// 高电平有效
#define VALID_ON_HIGH_LEVEL 1

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include "datatype.h"
//#include "pid_old.h"
#include "pid.h"

#include "fifgray.h"
#include "oled.h"
#include "debug.h"
#include "key.h"
#include "jy61p.h"
#include "wit.h"
#include "motor.h"
#include "soft_i2c.h"
#include "systick.h"

#include "ui.h"
#include "main.h"


#endif   // !MYLIB_CONFIG_H
