/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#define DRV8301_POWERGD_Pin GPIO_PIN_10
#define DRV8301_POWERGD_GPIO_Port GPIOB
#define DRV8301_OC_Pin GPIO_PIN_11
#define DRV8301_OC_GPIO_Port GPIOB
#define DRV8301_FAULT_Pin GPIO_PIN_12
#define DRV8301_FAULT_GPIO_Port GPIOB
#define DRV8301_SPISS_Pin GPIO_PIN_11
#define DRV8301_SPISS_GPIO_Port GPIOA
#define DRV8301_ENGATE_Pin GPIO_PIN_3
#define DRV8301_ENGATE_GPIO_Port GPIOB
#define HALL_A_Pin GPIO_PIN_6
#define HALL_A_GPIO_Port GPIOB
#define HALL_A_EXTI_IRQn EXTI9_5_IRQn
#define HALL_B_Pin GPIO_PIN_7
#define HALL_B_GPIO_Port GPIOB
#define HALL_B_EXTI_IRQn EXTI9_5_IRQn
#define HALL_C_Pin GPIO_PIN_8
#define HALL_C_GPIO_Port GPIOB
#define HALL_C_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
