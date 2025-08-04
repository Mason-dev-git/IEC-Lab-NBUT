/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_1_Pin GPIO_PIN_0
#define LED_1_GPIO_Port GPIOC
#define LED_2_Pin GPIO_PIN_1
#define LED_2_GPIO_Port GPIOC
#define LED_3_Pin GPIO_PIN_2
#define LED_3_GPIO_Port GPIOC
#define LCD_SCL_Pin GPIO_PIN_5
#define LCD_SCL_GPIO_Port GPIOA
#define LCD_SDA_Pin GPIO_PIN_7
#define LCD_SDA_GPIO_Port GPIOA
#define VOLTAGE_Pin GPIO_PIN_0
#define VOLTAGE_GPIO_Port GPIOB
#define ANGEL_Pin GPIO_PIN_1
#define ANGEL_GPIO_Port GPIOB
#define SERVO_1_Pin GPIO_PIN_9
#define SERVO_1_GPIO_Port GPIOE
#define SERVO_2_Pin GPIO_PIN_11
#define SERVO_2_GPIO_Port GPIOE
#define SERVO_3_Pin GPIO_PIN_13
#define SERVO_3_GPIO_Port GPIOE
#define SERVO_4_Pin GPIO_PIN_14
#define SERVO_4_GPIO_Port GPIOE
#define Flash_CS_Pin GPIO_PIN_15
#define Flash_CS_GPIO_Port GPIOE
#define Flash_SCK_Pin GPIO_PIN_10
#define Flash_SCK_GPIO_Port GPIOB
#define Flash_MISO_Pin GPIO_PIN_14
#define Flash_MISO_GPIO_Port GPIOB
#define Flash_MOSI_Pin GPIO_PIN_15
#define Flash_MOSI_GPIO_Port GPIOB
#define Gyro_RX_Pin GPIO_PIN_8
#define Gyro_RX_GPIO_Port GPIOD
#define Gyro_TX_Pin GPIO_PIN_9
#define Gyro_TX_GPIO_Port GPIOD
#define BUZZER_Pin GPIO_PIN_10
#define BUZZER_GPIO_Port GPIOD
#define LASER_Pin GPIO_PIN_11
#define LASER_GPIO_Port GPIOD
#define MOTOR_PWM_A_Pin GPIO_PIN_12
#define MOTOR_PWM_A_GPIO_Port GPIOD
#define MOTOR_PWM_B_Pin GPIO_PIN_13
#define MOTOR_PWM_B_GPIO_Port GPIOD
#define KEY_1_Pin GPIO_PIN_14
#define KEY_1_GPIO_Port GPIOD
#define KEY_2_Pin GPIO_PIN_15
#define KEY_2_GPIO_Port GPIOD
#define TOF_RX_Pin GPIO_PIN_6
#define TOF_RX_GPIO_Port GPIOC
#define TOF_TX_Pin GPIO_PIN_7
#define TOF_TX_GPIO_Port GPIOC
#define GRY_SDA_Pin GPIO_PIN_9
#define GRY_SDA_GPIO_Port GPIOC
#define GRY_SCL_Pin GPIO_PIN_8
#define GRY_SCL_GPIO_Port GPIOA
#define Debug_RX_Pin GPIO_PIN_9
#define Debug_RX_GPIO_Port GPIOA
#define Debug_TX_Pin GPIO_PIN_10
#define Debug_TX_GPIO_Port GPIOA
#define KEY_4_Pin GPIO_PIN_11
#define KEY_4_GPIO_Port GPIOA
#define KEY_3_Pin GPIO_PIN_12
#define KEY_3_GPIO_Port GPIOA
#define Encoder1_A_Pin GPIO_PIN_15
#define Encoder1_A_GPIO_Port GPIOA
#define LCD_BL_Pin GPIO_PIN_10
#define LCD_BL_GPIO_Port GPIOC
#define LCD_CS_Pin GPIO_PIN_11
#define LCD_CS_GPIO_Port GPIOC
#define LCD_DC_Pin GPIO_PIN_12
#define LCD_DC_GPIO_Port GPIOC
#define LCD_RES_Pin GPIO_PIN_0
#define LCD_RES_GPIO_Port GPIOD
#define MA_IN1_Pin GPIO_PIN_1
#define MA_IN1_GPIO_Port GPIOD
#define MB_IN1_Pin GPIO_PIN_2
#define MB_IN1_GPIO_Port GPIOD
#define MA_IN2_Pin GPIO_PIN_3
#define MA_IN2_GPIO_Port GPIOD
#define MB_IN2_Pin GPIO_PIN_4
#define MB_IN2_GPIO_Port GPIOD
#define Camera_RX_Pin GPIO_PIN_5
#define Camera_RX_GPIO_Port GPIOD
#define Camera_TX_Pin GPIO_PIN_6
#define Camera_TX_GPIO_Port GPIOD
#define Encoder1_B_Pin GPIO_PIN_3
#define Encoder1_B_GPIO_Port GPIOB
#define Encoder2_A_Pin GPIO_PIN_4
#define Encoder2_A_GPIO_Port GPIOB
#define Encoder2_B_Pin GPIO_PIN_5
#define Encoder2_B_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_6
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_7
#define OLED_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
