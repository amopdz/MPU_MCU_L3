/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

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
#define RED1_Pin GPIO_PIN_4
#define RED1_GPIO_Port GPIOA
#define YELLOW1_Pin GPIO_PIN_5
#define YELLOW1_GPIO_Port GPIOA
#define GREEN1_Pin GPIO_PIN_6
#define GREEN1_GPIO_Port GPIOA
#define SEG00_Pin GPIO_PIN_0
#define SEG00_GPIO_Port GPIOB
#define SEG01_Pin GPIO_PIN_1
#define SEG01_GPIO_Port GPIOB
#define SEG02_Pin GPIO_PIN_2
#define SEG02_GPIO_Port GPIOB
#define EN2_Pin GPIO_PIN_10
#define EN2_GPIO_Port GPIOB
#define EN3_Pin GPIO_PIN_11
#define EN3_GPIO_Port GPIOB
#define SW0_Pin GPIO_PIN_8
#define SW0_GPIO_Port GPIOA
#define SW1_Pin GPIO_PIN_9
#define SW1_GPIO_Port GPIOA
#define SW2_Pin GPIO_PIN_10
#define SW2_GPIO_Port GPIOA
#define RED0_Pin GPIO_PIN_12
#define RED0_GPIO_Port GPIOA
#define YELLOW0_Pin GPIO_PIN_13
#define YELLOW0_GPIO_Port GPIOA
#define GREEN0_Pin GPIO_PIN_14
#define GREEN0_GPIO_Port GPIOA
#define SEG03_Pin GPIO_PIN_3
#define SEG03_GPIO_Port GPIOB
#define SEG04_Pin GPIO_PIN_4
#define SEG04_GPIO_Port GPIOB
#define SEG05_Pin GPIO_PIN_5
#define SEG05_GPIO_Port GPIOB
#define SEG06_Pin GPIO_PIN_6
#define SEG06_GPIO_Port GPIOB
#define EN0_Pin GPIO_PIN_8
#define EN0_GPIO_Port GPIOB
#define EN1_Pin GPIO_PIN_9
#define EN1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define TIMER_INTERRUPT_PERIOD	10
#define MAIN_MODE_PERIOD 100

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
