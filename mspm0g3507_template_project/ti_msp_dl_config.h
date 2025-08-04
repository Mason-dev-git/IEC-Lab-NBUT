/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)



#define CPUCLK_FREQ                                                     32000000



/* Defines for PWM_FRONT_MOTOR */
#define PWM_FRONT_MOTOR_INST                                               TIMG0
#define PWM_FRONT_MOTOR_INST_IRQHandler                         TIMG0_IRQHandler
#define PWM_FRONT_MOTOR_INST_INT_IRQN                           (TIMG0_INT_IRQn)
#define PWM_FRONT_MOTOR_INST_CLK_FREQ                                   32000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_FRONT_MOTOR_C0_PORT                                       GPIOA
#define GPIO_PWM_FRONT_MOTOR_C0_PIN                               DL_GPIO_PIN_12
#define GPIO_PWM_FRONT_MOTOR_C0_IOMUX                            (IOMUX_PINCM34)
#define GPIO_PWM_FRONT_MOTOR_C0_IOMUX_FUNC             IOMUX_PINCM34_PF_TIMG0_CCP0
#define GPIO_PWM_FRONT_MOTOR_C0_IDX                          DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_FRONT_MOTOR_C1_PORT                                       GPIOA
#define GPIO_PWM_FRONT_MOTOR_C1_PIN                               DL_GPIO_PIN_13
#define GPIO_PWM_FRONT_MOTOR_C1_IOMUX                            (IOMUX_PINCM35)
#define GPIO_PWM_FRONT_MOTOR_C1_IOMUX_FUNC             IOMUX_PINCM35_PF_TIMG0_CCP1
#define GPIO_PWM_FRONT_MOTOR_C1_IDX                          DL_TIMER_CC_1_INDEX



/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMA0)
#define TIMER_0_INST_IRQHandler                                 TIMA0_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMA0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                         (49999U)



/* Defines for UART_DEBUG */
#define UART_DEBUG_INST                                                    UART0
#define UART_DEBUG_INST_FREQUENCY                                       32000000
#define UART_DEBUG_INST_IRQHandler                              UART0_IRQHandler
#define UART_DEBUG_INST_INT_IRQN                                  UART0_INT_IRQn
#define GPIO_UART_DEBUG_RX_PORT                                            GPIOA
#define GPIO_UART_DEBUG_TX_PORT                                            GPIOA
#define GPIO_UART_DEBUG_RX_PIN                                    DL_GPIO_PIN_11
#define GPIO_UART_DEBUG_TX_PIN                                    DL_GPIO_PIN_10
#define GPIO_UART_DEBUG_IOMUX_RX                                 (IOMUX_PINCM22)
#define GPIO_UART_DEBUG_IOMUX_TX                                 (IOMUX_PINCM21)
#define GPIO_UART_DEBUG_IOMUX_RX_FUNC                  IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_DEBUG_IOMUX_TX_FUNC                  IOMUX_PINCM21_PF_UART0_TX
#define UART_DEBUG_BAUD_RATE                                            (115200)
#define UART_DEBUG_IBRD_32_MHZ_115200_BAUD                                  (17)
#define UART_DEBUG_FBRD_32_MHZ_115200_BAUD                                  (23)
/* Defines for UART_Camera */
#define UART_Camera_INST                                                   UART1
#define UART_Camera_INST_FREQUENCY                                      32000000
#define UART_Camera_INST_IRQHandler                             UART1_IRQHandler
#define UART_Camera_INST_INT_IRQN                                 UART1_INT_IRQn
#define GPIO_UART_Camera_RX_PORT                                           GPIOA
#define GPIO_UART_Camera_TX_PORT                                           GPIOA
#define GPIO_UART_Camera_RX_PIN                                   DL_GPIO_PIN_18
#define GPIO_UART_Camera_TX_PIN                                   DL_GPIO_PIN_17
#define GPIO_UART_Camera_IOMUX_RX                                (IOMUX_PINCM40)
#define GPIO_UART_Camera_IOMUX_TX                                (IOMUX_PINCM39)
#define GPIO_UART_Camera_IOMUX_RX_FUNC                 IOMUX_PINCM40_PF_UART1_RX
#define GPIO_UART_Camera_IOMUX_TX_FUNC                 IOMUX_PINCM39_PF_UART1_TX
#define UART_Camera_BAUD_RATE                                           (115200)
#define UART_Camera_IBRD_32_MHZ_115200_BAUD                                 (17)
#define UART_Camera_FBRD_32_MHZ_115200_BAUD                                 (23)
/* Defines for UART_WIT */
#define UART_WIT_INST                                                      UART2
#define UART_WIT_INST_FREQUENCY                                         32000000
#define UART_WIT_INST_IRQHandler                                UART2_IRQHandler
#define UART_WIT_INST_INT_IRQN                                    UART2_INT_IRQn
#define GPIO_UART_WIT_RX_PORT                                              GPIOB
#define GPIO_UART_WIT_RX_PIN                                      DL_GPIO_PIN_16
#define GPIO_UART_WIT_IOMUX_RX                                   (IOMUX_PINCM33)
#define GPIO_UART_WIT_IOMUX_RX_FUNC                    IOMUX_PINCM33_PF_UART2_RX
#define UART_WIT_BAUD_RATE                                              (115200)
#define UART_WIT_IBRD_32_MHZ_115200_BAUD                                    (17)
#define UART_WIT_FBRD_32_MHZ_115200_BAUD                                    (23)





/* Defines for ADC_VOLTAGE */
#define ADC_VOLTAGE_INST                                                    ADC0
#define ADC_VOLTAGE_INST_IRQHandler                              ADC0_IRQHandler
#define ADC_VOLTAGE_INST_INT_IRQN                                (ADC0_INT_IRQn)
#define ADC_VOLTAGE_ADCMEM_0                                  DL_ADC12_MEM_IDX_0
#define ADC_VOLTAGE_ADCMEM_0_REF                 DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC_VOLTAGE_ADCMEM_0_REF_VOLTAGE_V                                     3.3
#define GPIO_ADC_VOLTAGE_C0_PORT                                           GPIOA
#define GPIO_ADC_VOLTAGE_C0_PIN                                   DL_GPIO_PIN_27



/* Defines for DMA_WIT */
#define DMA_WIT_CHAN_ID                                                      (0)
#define UART_WIT_INST_DMA_TRIGGER                            (DMA_UART2_RX_TRIG)


/* Port definition for Pin Group GPIO_GRP_MISC */
#define GPIO_GRP_MISC_PORT                                               (GPIOB)

/* Defines for PIN_KEY_1: GPIOB.0 with pinCMx 12 on package pin 47 */
#define GPIO_GRP_MISC_PIN_KEY_1_PIN                              (DL_GPIO_PIN_0)
#define GPIO_GRP_MISC_PIN_KEY_1_IOMUX                            (IOMUX_PINCM12)
/* Defines for PIN_KEY_2: GPIOB.1 with pinCMx 13 on package pin 48 */
#define GPIO_GRP_MISC_PIN_KEY_2_PIN                              (DL_GPIO_PIN_1)
#define GPIO_GRP_MISC_PIN_KEY_2_IOMUX                            (IOMUX_PINCM13)
/* Defines for PIN_KEY_3: GPIOB.4 with pinCMx 17 on package pin 52 */
#define GPIO_GRP_MISC_PIN_KEY_3_PIN                              (DL_GPIO_PIN_4)
#define GPIO_GRP_MISC_PIN_KEY_3_IOMUX                            (IOMUX_PINCM17)
/* Defines for PIN_KEY_4: GPIOB.5 with pinCMx 18 on package pin 53 */
#define GPIO_GRP_MISC_PIN_KEY_4_PIN                              (DL_GPIO_PIN_5)
#define GPIO_GRP_MISC_PIN_KEY_4_IOMUX                            (IOMUX_PINCM18)
/* Port definition for Pin Group GPIO_GRP_MOTOR */
#define GPIO_GRP_MOTOR_PORT                                              (GPIOB)

/* Defines for PIN_FRONT_LEFT_A: GPIOB.8 with pinCMx 25 on package pin 60 */
#define GPIO_GRP_MOTOR_PIN_FRONT_LEFT_A_PIN                      (DL_GPIO_PIN_8)
#define GPIO_GRP_MOTOR_PIN_FRONT_LEFT_A_IOMUX                    (IOMUX_PINCM25)
/* Defines for PIN_FRONT_RIGHT_A: GPIOB.9 with pinCMx 26 on package pin 61 */
#define GPIO_GRP_MOTOR_PIN_FRONT_RIGHT_A_PIN                     (DL_GPIO_PIN_9)
#define GPIO_GRP_MOTOR_PIN_FRONT_RIGHT_A_IOMUX                   (IOMUX_PINCM26)
/* Defines for PIN_FRONT_LEFT_B: GPIOB.10 with pinCMx 27 on package pin 62 */
#define GPIO_GRP_MOTOR_PIN_FRONT_LEFT_B_PIN                     (DL_GPIO_PIN_10)
#define GPIO_GRP_MOTOR_PIN_FRONT_LEFT_B_IOMUX                    (IOMUX_PINCM27)
/* Defines for PIN_FRONT_RIGHT_B: GPIOB.11 with pinCMx 28 on package pin 63 */
#define GPIO_GRP_MOTOR_PIN_FRONT_RIGHT_B_PIN                    (DL_GPIO_PIN_11)
#define GPIO_GRP_MOTOR_PIN_FRONT_RIGHT_B_IOMUX                   (IOMUX_PINCM28)
/* Defines for PIN_E1A: GPIOB.6 with pinCMx 23 on package pin 58 */
#define GPIO_GRP_ENCODER_PIN_E1A_PORT                                    (GPIOB)
// pins affected by this interrupt request:["PIN_E1A","PIN_E2A"]
#define GPIO_GRP_ENCODER_INT_IRQN                               (GPIOB_INT_IRQn)
#define GPIO_GRP_ENCODER_INT_IIDX               (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define GPIO_GRP_ENCODER_PIN_E1A_IIDX                        (DL_GPIO_IIDX_DIO6)
#define GPIO_GRP_ENCODER_PIN_E1A_PIN                             (DL_GPIO_PIN_6)
#define GPIO_GRP_ENCODER_PIN_E1A_IOMUX                           (IOMUX_PINCM23)
/* Defines for PIN_E1B: GPIOA.8 with pinCMx 19 on package pin 54 */
#define GPIO_GRP_ENCODER_PIN_E1B_PORT                                    (GPIOA)
#define GPIO_GRP_ENCODER_PIN_E1B_PIN                             (DL_GPIO_PIN_8)
#define GPIO_GRP_ENCODER_PIN_E1B_IOMUX                           (IOMUX_PINCM19)
/* Defines for PIN_E2A: GPIOB.7 with pinCMx 24 on package pin 59 */
#define GPIO_GRP_ENCODER_PIN_E2A_PORT                                    (GPIOB)
#define GPIO_GRP_ENCODER_PIN_E2A_IIDX                        (DL_GPIO_IIDX_DIO7)
#define GPIO_GRP_ENCODER_PIN_E2A_PIN                             (DL_GPIO_PIN_7)
#define GPIO_GRP_ENCODER_PIN_E2A_IOMUX                           (IOMUX_PINCM24)
/* Defines for PIN_E2B: GPIOA.9 with pinCMx 20 on package pin 55 */
#define GPIO_GRP_ENCODER_PIN_E2B_PORT                                    (GPIOA)
#define GPIO_GRP_ENCODER_PIN_E2B_PIN                             (DL_GPIO_PIN_9)
#define GPIO_GRP_ENCODER_PIN_E2B_IOMUX                           (IOMUX_PINCM20)
/* Defines for PIN_SENSER_1: GPIOA.15 with pinCMx 37 on package pin 8 */
#define GPIO_GRP_GRAY_PIN_SENSER_1_PORT                                  (GPIOA)
#define GPIO_GRP_GRAY_PIN_SENSER_1_PIN                          (DL_GPIO_PIN_15)
#define GPIO_GRP_GRAY_PIN_SENSER_1_IOMUX                         (IOMUX_PINCM37)
/* Defines for PIN_SENSER_2: GPIOA.16 with pinCMx 38 on package pin 9 */
#define GPIO_GRP_GRAY_PIN_SENSER_2_PORT                                  (GPIOA)
#define GPIO_GRP_GRAY_PIN_SENSER_2_PIN                          (DL_GPIO_PIN_16)
#define GPIO_GRP_GRAY_PIN_SENSER_2_IOMUX                         (IOMUX_PINCM38)
/* Defines for PIN_SENSER_3: GPIOB.14 with pinCMx 31 on package pin 2 */
#define GPIO_GRP_GRAY_PIN_SENSER_3_PORT                                  (GPIOB)
#define GPIO_GRP_GRAY_PIN_SENSER_3_PIN                          (DL_GPIO_PIN_14)
#define GPIO_GRP_GRAY_PIN_SENSER_3_IOMUX                         (IOMUX_PINCM31)
/* Defines for PIN_SENSER_4: GPIOB.12 with pinCMx 29 on package pin 64 */
#define GPIO_GRP_GRAY_PIN_SENSER_4_PORT                                  (GPIOB)
#define GPIO_GRP_GRAY_PIN_SENSER_4_PIN                          (DL_GPIO_PIN_12)
#define GPIO_GRP_GRAY_PIN_SENSER_4_IOMUX                         (IOMUX_PINCM29)
/* Defines for PIN_SENSER_5: GPIOB.3 with pinCMx 16 on package pin 51 */
#define GPIO_GRP_GRAY_PIN_SENSER_5_PORT                                  (GPIOB)
#define GPIO_GRP_GRAY_PIN_SENSER_5_PIN                           (DL_GPIO_PIN_3)
#define GPIO_GRP_GRAY_PIN_SENSER_5_IOMUX                         (IOMUX_PINCM16)
/* Defines for PIN_SENSER_6: GPIOB.2 with pinCMx 15 on package pin 50 */
#define GPIO_GRP_GRAY_PIN_SENSER_6_PORT                                  (GPIOB)
#define GPIO_GRP_GRAY_PIN_SENSER_6_PIN                           (DL_GPIO_PIN_2)
#define GPIO_GRP_GRAY_PIN_SENSER_6_IOMUX                         (IOMUX_PINCM15)
/* Defines for PIN_SENSER_7: GPIOA.7 with pinCMx 14 on package pin 49 */
#define GPIO_GRP_GRAY_PIN_SENSER_7_PORT                                  (GPIOA)
#define GPIO_GRP_GRAY_PIN_SENSER_7_PIN                           (DL_GPIO_PIN_7)
#define GPIO_GRP_GRAY_PIN_SENSER_7_IOMUX                         (IOMUX_PINCM14)
/* Port definition for Pin Group GPIO_GRP_OLED */
#define GPIO_GRP_OLED_PORT                                               (GPIOA)

/* Defines for PIN_SCL: GPIOA.31 with pinCMx 6 on package pin 39 */
#define GPIO_GRP_OLED_PIN_SCL_PIN                               (DL_GPIO_PIN_31)
#define GPIO_GRP_OLED_PIN_SCL_IOMUX                               (IOMUX_PINCM6)
/* Defines for PIN_SDA: GPIOA.28 with pinCMx 3 on package pin 35 */
#define GPIO_GRP_OLED_PIN_SDA_PIN                               (DL_GPIO_PIN_28)
#define GPIO_GRP_OLED_PIN_SDA_IOMUX                               (IOMUX_PINCM3)



/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWM_FRONT_MOTOR_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_UART_DEBUG_init(void);
void SYSCFG_DL_UART_Camera_init(void);
void SYSCFG_DL_UART_WIT_init(void);
void SYSCFG_DL_ADC_VOLTAGE_init(void);
void SYSCFG_DL_DMA_init(void);

void SYSCFG_DL_SYSTICK_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
