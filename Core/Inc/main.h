/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define GEAR1_Pin GPIO_PIN_3
#define GEAR1_GPIO_Port GPIOA
#define GEAR2_Pin GPIO_PIN_4
#define GEAR2_GPIO_Port GPIOA
#define GEAR3_Pin GPIO_PIN_5
#define GEAR3_GPIO_Port GPIOA
#define GEAR4_Pin GPIO_PIN_6
#define GEAR4_GPIO_Port GPIOA
#define GEAR5_Pin GPIO_PIN_7
#define GEAR5_GPIO_Port GPIOA
#define GEAR6_Pin GPIO_PIN_0
#define GEAR6_GPIO_Port GPIOB
#define GEAR_R_Pin GPIO_PIN_1
#define GEAR_R_GPIO_Port GPIOB
#define HORN_Pin GPIO_PIN_10
#define HORN_GPIO_Port GPIOB
#define R_EN_Pin GPIO_PIN_12
#define R_EN_GPIO_Port GPIOB
#define L_EN_Pin GPIO_PIN_13
#define L_EN_GPIO_Port GPIOB
#define RPWM_Pin GPIO_PIN_15
#define RPWM_GPIO_Port GPIOA
#define LPWM_Pin GPIO_PIN_3
#define LPWM_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
//#define GEAR1 0x01
//#define GEAR2 0x02
//#define GEAR3 0x04
//#define GEAR4 0x08
//#define GEAR5 0x10
//#define GEAR6 0x20
//#define GEAR7 0x40
//#define GEAR8 0x80
//#define GEAR9 0x01
//#define GEAR10 0x02
//#define GEAR11 0x04
//#define GEAR12 0x08
//#define GEAR13 0x10
//#define GEAR14 0x20
//#define GEAR15 0x40
//#define GEAR16 0x80
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
