#include <mylib/device/device_motor_encoder.h>

// 用一个指针数组来存储编码器
motor_encoder_t* g_encoder[5];
int32_t          g_current_size = 0;
// 提前计算一些值
int      g_f1 = 0, g_f2 = 0;       // 标记当前编码器有没有属于GPIOA和GPIOB的
uint32_t g_pinA = 0, g_pinB = 0;   // GPIOA和GPIOB的pin的位与

// 添加一个编码器
void motor_encoder_add(motor_encoder_t* encoder)
{
    int idx                     = g_current_size;
    g_encoder[g_current_size++] = encoder;

    // 重新计算值
    if (g_encoder[idx]->port_a == GPIOA) {
        g_pinA = g_pinA | g_encoder[idx]->pin_a;
        g_f1   = 1;
    }
    if (g_encoder[idx]->port_a == GPIOB) {
        g_pinB = g_pinB | g_encoder[idx]->pin_a;
        g_f2   = 1;
    }
}

// 读取编码器的count
int32_t motor_encoder_get_count(motor_encoder_t* encoder)
{
    return encoder->count;
}

// 清空编码器的count
void motor_encoder_clear_count(motor_encoder_t* encoder)
{
    encoder->count = 0;
}

// 需要在外部中断里调用
void motor_encoder_irq_handler(void)
{
    uint32_t gpioA, gpioB;
    if (g_f1 != 0) {
        gpioA = DL_GPIO_getEnabledInterruptStatus(GPIOA, g_pinA);
        for (int i = 0; i < g_current_size; i++) {
            if (g_encoder[i]->port_a == GPIOA) {
                if (g_encoder[i]->pin_a & gpioA) {
                    if (DL_GPIO_readPins(g_encoder[i]->port_b, g_encoder[i]->pin_b))
                        g_encoder[i]->count--;
                    else
                        g_encoder[i]->count++;
                    DL_GPIO_clearInterruptStatus(g_encoder[i]->port_a, g_encoder[i]->pin_a);
                }
            }
        }
    }
    if (g_f2 != 0) {
        gpioB = DL_GPIO_getEnabledInterruptStatus(GPIOB, g_pinB);
        for (int i = 0; i < g_current_size; i++) {
            if (g_encoder[i]->port_a == GPIOB) {
                if (g_encoder[i]->pin_a & gpioB) {
                    if (DL_GPIO_readPins(g_encoder[i]->port_b, g_encoder[i]->pin_b))
                        g_encoder[i]->count--;
                    else
                        g_encoder[i]->count++;
                    DL_GPIO_clearInterruptStatus(g_encoder[i]->port_a, g_encoder[i]->pin_a);
                }
            }
        }
    }
}
