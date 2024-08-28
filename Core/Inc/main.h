/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>

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
#define LD3_Pin GPIO_PIN_13
#define LD3_GPIO_Port GPIOC
#define LD4_Pin GPIO_PIN_14
#define LD4_GPIO_Port GPIOC
#define LD5_Pin GPIO_PIN_15
#define LD5_GPIO_Port GPIOC
#define LD2_Pin GPIO_PIN_0
#define LD2_GPIO_Port GPIOA
#define LD1_Pin GPIO_PIN_1
#define LD1_GPIO_Port GPIOA
#define PS1_Pin GPIO_PIN_2
#define PS1_GPIO_Port GPIOA
#define PS2_Pin GPIO_PIN_3
#define PS2_GPIO_Port GPIOA
#define PS3_Pin GPIO_PIN_4
#define PS3_GPIO_Port GPIOA
#define PS4_Pin GPIO_PIN_5
#define PS4_GPIO_Port GPIOA
#define PS5_Pin GPIO_PIN_6
#define PS5_GPIO_Port GPIOA
#define PS6_Pin GPIO_PIN_7
#define PS6_GPIO_Port GPIOA
#define PS7_Pin GPIO_PIN_0
#define PS7_GPIO_Port GPIOB
#define PS8_Pin GPIO_PIN_1
#define PS8_GPIO_Port GPIOB
#define EC22_EXTI0_Pin GPIO_PIN_0
#define EC22_EXTI0_GPIO_Port GPIOD
#define EC22_EXTI0_EXTI_IRQn EXTI0_1_IRQn
#define EC21_EXTI1_Pin GPIO_PIN_1
#define EC21_EXTI1_GPIO_Port GPIOD
#define EC21_EXTI1_EXTI_IRQn EXTI0_1_IRQn
#define EC12_EXTI2_Pin GPIO_PIN_2
#define EC12_EXTI2_GPIO_Port GPIOD
#define EC12_EXTI2_EXTI_IRQn EXTI2_3_IRQn
#define EC11_EXTI3_Pin GPIO_PIN_3
#define EC11_EXTI3_GPIO_Port GPIOD
#define EC11_EXTI3_EXTI_IRQn EXTI2_3_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
