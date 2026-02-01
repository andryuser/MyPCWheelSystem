/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

//										                	NACH JEDER ÄNDERUNG DER IOC-DATEI DIE WERTE IN usbd_conf.h KORRIGIEREN!!!

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_customhid.h"
#include "pcf8574events.h"
#include "forcefeedback.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//volatile char btn_action = 0;
//volatile char gear1 = 0;

extern USBD_HandleTypeDef hUsbDeviceFS;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern ADC_HandleTypeDef hadc2;
extern I2C_HandleTypeDef hi2c1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
int16_t xToDegree(int x);
uint16_t degreeToX(int16_t degree);
void initLED(void);
void toggleLED(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	struct pc_wheel_report_t
	{
		uint8_t report_id;

		uint8_t wheelX_LO;
		uint8_t wheelX_HI;

		uint8_t accelRx_LO;
		uint8_t accelRx_HI;

		uint8_t brakeRy_LO;
		uint8_t brakeRy_HI;

		uint8_t clutchRz_LO;
		uint8_t clutchRz_HI;

		uint8_t buttons_1;
		uint8_t buttons_2;
	};

	uint16_t wheelValue = 0;

	uint16_t gasdata_origin = 0;
	uint16_t brakedata_origin = 0;
	uint16_t clutchdata_origin = 0;
	uint16_t gasdata = 0;  // 12 bit adc
	uint16_t brakedata = 0;  // 12 bit adc
	uint16_t clutchdata = 0;  // 12 bit adc

	uint8_t pcf_data = 0x00;
	uint8_t pcf_data_old = 0x00;
	uint8_t wiper = 0;
	uint8_t wiper_old = 0;
	uint8_t wiper_increment = 0;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  initLED();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  MX_ADC2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
//  SET_BIT(GPIOC->ODR, GPIO_ODR_ODR13);

  struct pc_wheel_report_t myPCWheelReport;
  myPCWheelReport.report_id = 0x01;
  myPCWheelReport.wheelX_LO = 0;
  myPCWheelReport.wheelX_HI = 0;
  myPCWheelReport.accelRx_LO = 0;
  myPCWheelReport.accelRx_HI = 0;
  myPCWheelReport.brakeRy_LO = 0;
  myPCWheelReport.brakeRy_HI = 0;
  myPCWheelReport.clutchRz_LO = 0;
  myPCWheelReport.clutchRz_HI = 0;
  myPCWheelReport.buttons_1 = 0;
  myPCWheelReport.buttons_2 = 0;


  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);  // Timer für Lenkrad-Encoder

  TIM1->CNT = 5000;  // Encoder Reset Mittelpunkt

  // Gas, Bremse, Kupplung Reset
  HAL_ADCEx_InjectedStart(&hadc2);
  HAL_ADCEx_InjectedPollForConversion(&hadc2, 100);
  gasdata_origin = HAL_ADCEx_InjectedGetValue(&hadc2, ADC_INJECTED_RANK_1);
  brakedata_origin = HAL_ADCEx_InjectedGetValue(&hadc2, ADC_INJECTED_RANK_2);
  clutchdata_origin = HAL_ADCEx_InjectedGetValue(&hadc2, ADC_INJECTED_RANK_3);
  HAL_ADCEx_InjectedStop(&hadc2);

  // PCF8574 auf 0 setzen
  HAL_I2C_Master_Transmit(&hi2c1, 0x20 << 1, &pcf_data, 1, 200);

  // FFB initialisieren
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // PWM aktivieren
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  ffb_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  // Lenkrad

	  wheelValue = (uint16_t) TIM1->CNT;

	  if (wheelValue > 35000)
		  wheelValue = 0;
	  if (wheelValue > 10000 && wheelValue <= 35000)
		  wheelValue = 10000;

	  myPCWheelReport.wheelX_LO = LOBYTE(wheelValue);
	  myPCWheelReport.wheelX_HI = HIBYTE(wheelValue);


	  // Gas, Bremse, Kupplung
	  HAL_ADCEx_InjectedStart(&hadc2);
	  HAL_ADCEx_InjectedPollForConversion(&hadc2, 100);

	  gasdata = gasdata_origin - (uint16_t) HAL_ADCEx_InjectedGetValue(&hadc2, ADC_INJECTED_RANK_1);
	  brakedata = (uint16_t) HAL_ADCEx_InjectedGetValue(&hadc2, ADC_INJECTED_RANK_2) - brakedata_origin;
	  clutchdata = clutchdata_origin - (uint16_t) HAL_ADCEx_InjectedGetValue(&hadc2, ADC_INJECTED_RANK_3);

	  if (gasdata > 6000)
		  gasdata = 0;
	  if (brakedata > 6000)
		  brakedata = 0;
	  if (clutchdata > 6000)
		  clutchdata = 0;

	  gasdata *= 1.6;
	  brakedata *= 2;
	  clutchdata *=2.6;

	  HAL_ADCEx_InjectedStop(&hadc2);

//	  gasdata *= 1.5;
//	  clutchdata *= 2;

	  myPCWheelReport.accelRx_LO = LOBYTE(gasdata);
	  myPCWheelReport.accelRx_HI = HIBYTE(gasdata);
	  myPCWheelReport.brakeRy_LO = LOBYTE(brakedata);
	  myPCWheelReport.brakeRy_HI = HIBYTE(brakedata);
	  myPCWheelReport.clutchRz_LO = LOBYTE(clutchdata);
	  myPCWheelReport.clutchRz_HI = HIBYTE(clutchdata);


	  // Blinker, Fernlicht, Scheibenwischer
	  reset_highbeam_blinker_wiper_buttons(&myPCWheelReport.buttons_2);
	  HAL_I2C_Master_Receive(&hi2c1, 0x20 << 1, &pcf_data, 1, 200);
	  repair_pcf8574(&pcf_data, &hi2c1);
	  set_highbeam_blinker_buttons(&myPCWheelReport.buttons_2, pcf_data, pcf_data_old);
	  wiper = get_wiper(pcf_data);
	  pcf_data_old = pcf_data;
	  set_wiper_buttons(&myPCWheelReport.buttons_2, wiper, &wiper_old, &wiper_increment);


	  // Gänge
	  myPCWheelReport.buttons_1 = ~(HAL_GPIO_ReadPin(GEAR1_GPIO_Port, GEAR1_Pin) << 0
			              | HAL_GPIO_ReadPin(GEAR2_GPIO_Port, GEAR2_Pin) << 1
						  | HAL_GPIO_ReadPin(GEAR3_GPIO_Port, GEAR3_Pin) << 2
						  | HAL_GPIO_ReadPin(GEAR4_GPIO_Port, GEAR4_Pin) << 3
						  | HAL_GPIO_ReadPin(GEAR5_GPIO_Port, GEAR5_Pin) << 4
						  | HAL_GPIO_ReadPin(GEAR6_GPIO_Port, GEAR6_Pin) << 5
						  | HAL_GPIO_ReadPin(GEAR_R_GPIO_Port, GEAR_R_Pin) << 6
						  | HAL_GPIO_ReadPin(HORN_GPIO_Port, HORN_Pin) << 7);

	  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t*) &myPCWheelReport, sizeof(myPCWheelReport));
	  HAL_Delay(25);

	  ffb_play_effects();
  }  // end while loop
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

int16_t xToDegree(int x)    // x: -2^31...2^31-1, degree: -450...450
{
	return (int16_t) (x * 0.01373 - 450);         // denn 900/65535 = 0,01373
}

uint16_t degreeToX(int16_t degree)    // degree: -2^15...2^15-1, x: 0...65535
{
	return (uint16_t) ((degree + 450) * 72.81667);        // denn 65535/900 = 72,81667
}

void initLED(void)
{
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);          //enable IO port C clock

	CLEAR_BIT(GPIOC->CRH, GPIO_CRH_CNF13_1);          //General purpose output push-pull
	CLEAR_BIT(GPIOC->CRH, GPIO_CRH_CNF13_0);

	SET_BIT(GPIOC->CRH, GPIO_CRH_MODE13_1);           //output mode, max speed 2 MHz
	CLEAR_BIT(GPIOC->CRH, GPIO_CRH_MODE13_0);

	SET_BIT(GPIOC->ODR, GPIO_ODR_ODR13);             //LED off
}

void toggleLED(void)
{
	if(READ_BIT(GPIOC->ODR, GPIO_ODR_ODR13) == 0)
	{
		SET_BIT(GPIOC->ODR, GPIO_ODR_ODR13);
	}
	else
	{
		CLEAR_BIT(GPIOC->ODR, GPIO_ODR_ODR13);
	}
}

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	if(GPIO_Pin == GEAR1_Pin)
//	{
//		if(!btn_action)
//		{
//			btn_action = 1;
//			__HAL_TIM_SET_COUNTER(&htim2, 0);
//		}
//	}
//}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	if (htim == &htim2) //for button debounce, runs every 10 ms and 10 ms after valid edge
//	{
//		if(btn_action && !HAL_GPIO_ReadPin(GEAR1_GPIO_Port, GEAR1_Pin))    //valid rising edge
//		{
//			gear1 = 1;
//			CLEAR_BIT(GPIOC->ODR, GPIO_ODR_ODR13);
//			btn_action = 0;
//		}
//		else if(btn_action && HAL_GPIO_ReadPin(GEAR1_GPIO_Port, GEAR1_Pin))    //valid falling edge
//		{
//			gear1 = 0;
//			SET_BIT(GPIOC->ODR, GPIO_ODR_ODR13);
//			btn_action = 0;
//		}
//	}
//
////	if (htim == &htim3) // for PCF8574 events
////	{
////
////	}
//}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
