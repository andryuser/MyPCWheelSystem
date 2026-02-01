/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @version        : v2.0_Cube
  * @brief          : USB Device Custom HID interface file.
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

/* Includes ------------------------------------------------------------------*/
#include "usbd_custom_hid_if.h"

/* USER CODE BEGIN INCLUDE */
#include "forcefeedback.h"
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_CUSTOM_HID
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions USBD_CUSTOM_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */


/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Defines USBD_CUSTOM_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Macros USBD_CUSTOM_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Variables USBD_CUSTOM_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
  /* USER CODE BEGIN 0 */
	0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
	0x09, 0x04,                    // USAGE (Joystick)
	0xa1, 0x01,                    // COLLECTION (Application)
	0xa1, 0x02,                    //   COLLECTION (Logical)
	0x85, 0x01,                    // 	  Report ID 1

	0x95, 0x01,                    //     REPORT_COUNT (1)
	0x75, 0x10,                    //     REPORT_SIZE (16)
	0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
	0x26, 0x10, 0x27,              //     LOGICAL_MAXIMUM (10000)
	0x09, 0x30,                    //     USAGE (X)                    ***wheel (16 bits)
	0x81, 0x02,                    //     INPUT (Data,Var,Abs)

	0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
	0x95, 0x03,                    //     REPORT_COUNT (3)
	0x75, 0x10,                    //     REPORT_SIZE (16)
	0x26, 0xff, 0x0f,              //     LOGICAL_MAXIMUM (4095)  2¹²-1 (12-bit-ADC)
	0x09, 0x33,                    //     USAGE (Rx)                   ***accelerator (16 bits)
	0x09, 0x34,                    //     USAGE (Ry)                   ***brake (16 bits)
	0x09, 0x35,                    //     USAGE (Rz)                   ***clutch (16 bits)
	0x81, 0x02,                    //     INPUT (Data,Var,Abs)

	0x95, 0x10,                    //     REPORT_COUNT (16)
	0x75, 0x01,                    //     REPORT_SIZE (1)
	0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
	0x05, 0x09,                    //     USAGE_PAGE (Button)
	0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
	0x29, 0x10,                    //     USAGE_MAXIMUM (Button 16)
	0x81, 0x02,                    //     INPUT (Data,Var,Abs)         ***buttons (16 bits)

	0xc0,                          //   END_COLLECTION



	//-------------------------------------------------------------------------------------------------
	// +++++++++++++++++++++++++++++++	START OF PID State Report	+++++++++++++++++++++++++++++++
	//-------------------------------------------------------------------------------------------------


	 /*
	  Input
	  Collection Datalink (sub-collection)
	  Physical Interface (Usage: PID State report)
	  ID: 2
	  state report: 5X1bit
	  Padding: 3bit
	  PID Device Control: 1bit
	  Effect Block Index: 7bit
	 */

	0x05,0x0F, // Usage Page Physical Interface
	0x09,0x92, // Usage PID State report
	0xA1,0x02, // Collection Datalink (logical)

		0x85,0x02, // Report ID 2
		0x09,0x9F, // Usage Device is Pause
		0x09,0xA0, // Usage Actuators Enabled
		0x09,0xA4, // Usage Safety Switch
		0x09,0xA5, // Usage Actuator Override Switch
		0x09,0xA6, // Usage Actuator Power
		0x15,0x00, // Logical Minimum 0
		0x25,0x01, // Logical Maximum 1
		0x35,0x00, // Physical Minimum 0
		0x45,0x01, // Physical Maximum 1
		0x75,0x01, // Report Size 1
		0x95,0x05, // Report Count 5
		0x81,0x02, // Input (Variable)
		0x95,0x03, // Report Count 3
		0x81,0x03, // Input (Constant, Variable)

		0x09,0x22, // Usage Effect Block Index
		0x15,0x01, // Logical Minimum 1
		0x25,0x28, // Logical Maximum 28h (40d)
		0x35,0x01, // Physical Minimum 1
		0x45,0x28, // Physical Maximum 28h (40d)
		0x75,0x07, // Report Size 7
		0x95,0x01, // Report Count 1
		0x81,0x02, // Input (Variable)

		0x09,0x94, // Usage Effect Playing
		0x15,0x00, // Logical Minimum 0
		0x25,0x01, // Logical Maximum 1
		0x35,0x00, // Physical Minimum 0
		0x45,0x01, // Physical Maximum 1
		0x75,0x01, // Report Size 1
		0x95,0x01, // Report Count 1
		0x81,0x02, // Input (Variable)


	0xC0, // End Collection



	//-------------------------------------------------------------------------------------------------
	// +++++++++++++++++++++++++++++++	START OF Set Effect Report	+++++++++++++++++++++++++++++++
	//-------------------------------------------------------------------------------------------------


	 /*
	  Output
	  Collection Datalink:
	  Usage Set Effect Report

	  ID:3
	  Effect Block Index: 8bit

	  subcollection Effect Type
	  12 effect types, 8bit each
	 */

	0x09,0x21, // Usage Set Effect Report
	0xA1,0x02, // Collection Datalink (Logical)

		0x85,0x03, // Report ID 3

		0x09,0x22, // Usage Effect Block Index
		0x15,0x01, // Logical Minimum 1
		0x25,0x28, // Logical Maximum 28h (40d)
		0x35,0x01, // Physical Minimum 1
		0x45,0x28, // Physical Maximum 28h (40d)
		0x75,0x08, // Report Size 8
		0x95,0x01, // Report Count 1
		0x91,0x02, // Output (Variable)

		0x09,0x25, // Usage Effect Type
		0xA1,0x02, // Collection Datalink

			0x09,0x26, // Usage ET Constant Force		1
			0x09,0x27, // Usage ET Ramp				2
			0x09,0x30, // Usage ET Square				3
			0x09,0x31, // Usage ET Sine				4
			0x09,0x32, // Usage ET Triangle			5
			0x09,0x33, // Usage ET Sawtooth Up		6
			0x09,0x34, // Usage ET Sawtooth Down		7
			0x09,0x40, // Usage ET Spring				8
			0x09,0x41, // Usage ET Damper				9
			0x09,0x42, // Usage ET Inertia			10
			0x09,0x43, // Usage ET Friction			11
			0x09,0x28, // Usage ET Custom Force Data	12
			0x25,0x0C, // Logical Maximum Ch (12d)
			0x15,0x01, // Logical Minimum 1
			0x35,0x01, // Physical Minimum 1
			0x45,0x0C, // Physical Maximum Ch (12d)
			0x75,0x08, // Report Size 8
			0x95,0x01, // Report Count 1
			0x91,0x00, // Output

		0xC0, // End Collection


		0x09,0x50, // Usage Duration
		0x09,0x54, // Usage Trigger Repeat Interval
		0x09,0x51, // Usage Sample Period

		0x15,0x00, 		 // Logical Minimum 0
		0x26,0x10,0x27, // Logical Maximum 10000d
		0x35,0x00, 		 // Physical Minimum 0
		0x46,0x10,0x27, // Physical Maximum 10000d
									//0			 0			0		   1 sec		  0	    0		  1SI linear
		0x66,0x01,0x10, // Unit (lum.intensity) (current) (temperature)	(time)		(mass) (length) (system)
		0x55,0x0d, 		// Unit Exponent -3, so we report the duration, trigger repeat interval and sample period in ms

		0x75,0x10, // Report Size 10h (16d)
		0x95,0x03, // Report Count 3

		0x91,0x02, // Output (Variable)


		0x09,0x52, 		// Usage Gain

		0x15,0x00, 		// Logical Minimum 0
		0x25,0x64, 		// Logical Maximum 100d
		0x35,0x00, 		// Physical Minimum 0
		0x45,0x64, 		// Physical Maximum 100
		0x65,0x00,      // Unit None
		0x55,0x00,      // Unit Exponent 0

		0x75,0x08, // Report Size 8
		0x95,0x01, // Report Count 1

		0x91,0x02, // Output (Variable)


		0x09,0x53, // Usage Trigger Button

		0x15,0x01, // Logical Minimum 1
		0x25,0x08, // Logical Maximum 8
		0x35,0x01, // Physical Minimum 1
		0x45,0x08, // Physical Maximum 8

		0x75,0x08, // Report Size 8
		0x95,0x01, // Report Count 1

		0x91,0x02, // Output (Variable)


		0x09,0x55, // Usage Axes Enable

		0xA1,0x02, // Collection Datalink

			0x05,0x01, // Usage Page Generic Desktop
			0x09,0x01, // Usage POINTER
			0xA1,0x00, // COLLECTION PHYSICAL

				0x09,0x30, // Usage X
				0x09,0x31, // Usage Y
				0x15,0x00, // Logical Minimum 0
				0x25,0x01, // Logical Maximum 1
				0x75,0x01, // Report Size 1
				0x95,0x02, // Report Count 2
				0x91,0x02, // Output (Variable)

			0xC0, // End Collection

		0xC0, // End Collection

		0x05,0x0F, // Usage Page Physical Interface
		0x09,0x56, // Usage Direction Enable

		0x95,0x01, // Report Count 1
		0x91,0x02, // Output (Variable)


		0x95,0x01, // Report Count 1                1-bit pad
		0x91,0x03, // Output (Constant, Variable)


		0x09,0x58, // Usage Type Specific Block Offset

		0xA1,0x02, // Start Collection Datalink : Usage Type Specific Block Offset ++++++++++SCD: TSBO+++++++++++

			0x0B,0x01,0x00,0x0A,0x00, // Usage Ordinals: Instance 1
			0x0B,0x02,0x00,0x0A,0x00, // Usage Ordinals: Instance 2

			0x15,0x00, // Logical Minimum 0
			0x25,0x01, // Logical Maximum 1
			0x35,0x00, // Physical Minimum 0
			0x45,0x01, // Physical Maximum 1

			0x75,0x02, // Report Size 2
			0x95,0x02, // Report Count 2

			0x91,0x02, // Output (Variable)

		0xC0, // End Collection Datalink : Usage Type Specific Block Offset ------------------ECD: TSBO-----------


		0x09,0x57, // Usage Direction

		0xA1,0x02, // Collection Datalink  : Usage Direction				++++++++++++++++SCD UD++++++++++++++++

			0x0B,0x01,0x00,0x0A,0x00, // Usage Ordinals: Instance 1
			0x0B,0x02,0x00,0x0A,0x00, // Usage Ordinals: Instance 2

			0x65,0x14, 					// Unit System : English, Rotation 360 degrees
			0x55,0x01, 					// Unit Exponent 1

			0x15,0x00, 					// Logical Minimum 0
			0x26,0x68,0x01, 			// Logical Maximum 360d

			0x35,0x00, 					// Physical Minimum 0
			0x46,0x68,0x01, 			// Physical Maximum 360d

			0x75,0x10, 					// Report Size 16
			0x95,0x02, 					// Report Count 2

			0x91,0x02, 					// Output (Variable)

			0x55,0x00, 					// Unit Exponent 0
			0x65,0x00,      			// Unit 0

		0xC0, // End Collection : Usage Direction				+----------------ECD UD---------------------

		0x05,0x0F, // Usage Page Physical Interface
		0x09,0xA7, // Usage Start Delay

		0x66,0x03,0x10, // Unit 1003h (sec)
		0x55,0x0D, 		// Unit Exponent -3 (ms)
		0x15,0x00, 		// Logical Minimum 0
		0x26,0xFF,0x7F, // Logical Maximum 7FFFh (32767d)
		0x35,0x00, 		// Physical Minimum 0
		0x46,0xFF,0x7F, // Physical Maximum 7FFFh (32767d)

		0x75,0x10, 		// Report Size 10h (16d)
		0x95,0x01,		// Report Count 1

		0x91,0x02, 		// Output (Variable)

		0x65,0x00,      // Unit 0
		0x55,0x00, 		// Unit Exponent 0

	0xC0, // End Collection Usage Set Effect Report



	//-------------------------------------------------------------------------------------------------
	// +++++++++++++++++++++++++++++++	START OF Set Envelope Report	+++++++++++++++++++++++++++++++
	//-------------------------------------------------------------------------------------------------


	0x05,0x0F, // Usage Page Physical Interface
	0x09,0x5A, // Usage Set Envelope Report

	0xA1,0x02, // Collection Datalink	-	Usage Set Envelope Report ++++++++++SCD: USER+++++++++++

		0x85,0x04, // Report ID 0x04                       Usage Set Envelope Report

		0x09,0x22, 		// Usage Effect Block Index

		0x15,0x01, 		// Logical Minimum 1
		0x25,0x28, 		// Logical Maximum 0x28h (40d)
		0x35,0x01, 		// Physical Minimum 1
		0x45,0x28, 		// Physical Maximum 0x28h (40d)

		0x75,0x08, 		// Report Size 8
		0x95,0x01, 		// Report Count 1

		0x91,0x02, 		// Output (Variable)


		0x09,0x5B, 		// Usage Attack Level
		0x09,0x5D, 		// Usage Fade Level

		0x15,0x00, 		// Logical Minimum 0
		0x25,0x64,		// Logical Maximum 100d
		0x35,0x00, 		// Physical Minimum 0
		0x45,0x64, 		// Physical Maximum 100

		0x95,0x02, 		// Report Count 2

		0x91,0x02, 		// Output (Variable)


		0x09,0x5C, 		// Usage Attack Time
		0x09,0x5E, 		// Usage Fade Time

		0x66,0x01,0x10, // Unit second, SI linear
		0x55,0x0D, 		// Unit Exponent -3  ( we get time in milliseconds)

		0x26,0xE8,0x03, // Logical Maximum 1000 ( maximum 1000 milliseconds)
		0x46,0xE8,0x03, // Physical Maximum 1000 ( maximum 1000 milliseconds)

		0x75,0x10, 		// Report Size 10h (16d)

		0x91,0x02, 		// Output (Variable)

		0x65,0x00,      // Unit 0
		0x55,0x00, 		// Unit Exponent 0

	0xC0, 		// End Collection Set Envelope Report								 ----------ECD: USER----------



	//-------------------------------------------------------------------------------------------------
	// +++++++++++++++++++++++++++++++	START OF Set Condition Report	+++++++++++++++++++++++++++++++
	//-------------------------------------------------------------------------------------------------


	0x09,0x5F, // Usage Set Condition Report
	0xA1,0x02, // Start Collection Datalink : Usage Set Condition Report ++++++++++SCD: USCR+++++++++++

		0x85,0x05, // Report ID 5

		0x09,0x22, // Usage Effect Block Index

		0x15,0x01, // Logical Minimum 1
		0x25,0x28, // Logical Maximum 0x28 (40d)
		0x35,0x01, // Physical Minimum 1
		0x45,0x28, // Physical Maximum 0x28 (40d)

		0x75,0x08, // Report Size 8
		0x95,0x01, // Report Count 1

		0x91,0x02, // Output (Variable)


		0x09,0x23, // Usage Parameter Block Offset

		0x15,0x00, // Logical Minimum 0
		0x25,0x01, // Logical Maximum 1
		0x35,0x00, // Physical Minimum 0
		0x45,0x01, // Physical Maximum 1

		0x75,0x04, // Report Size 4
		0x95,0x01, // Report Count 1

		0x91,0x02, // Output (Variable)


		0x95,0x04, // Report Count 4                4-bit pad
		0x91,0x03, // Output (Constant, Variable)


		0x09,0x60, // Usage CP Offset

		//--------------------------------------------- maybe put them before cp offset,??????
		0x15,0x9C, 		// Logical Minimum -100
		0x25,0x64, 		// Logical Maximum 100

		0x35,0x9C, 		// Physical Minimum -100
		0x45,0x64, 		// Physical Maximum +100

		0x75,0x08, 		// Report Size 8
		0x95,0x01, 		// Report Count 1

		0x91,0x02, 		// Output (Variable)


		0x09,0x61, // Usage Positive Coefficient
		0x09,0x62, // Usage Negative Coefficient

		0x15,0x00, 		// Logical Minimum 0
		0x25,0x64, 		// Logical Maximum 100
		0x35,0x00, 		// Physical Minimum 0
		0x45,0x64, 		// Physical Maximum 100

		0x75,0x08, 		// Report Size 8
		0x95,0x02, 		// Report Count 2

		0x91,0x02, 		// Output (Variable)


		0x09,0x63, // Usage Positive Saturation
		0x09,0x64, // Usage Negative Saturation

		0x15,0x00, 		// Logical Minimum 0
		0x25,0x64, 		// Logical Maximum 100
		0x35,0x00, 		// Physical Minimum 0
		0x45,0x64, 		// Physical Maximum 100

		0x75,0x08, 		// Report Size 8
		0x95,0x02, 		// Report Count 2

		0x91,0x02, 		// Output (Variable)


		0x09,0x65, // Usage Dead Band

		0x15,0x00, 		// Logical Minimum 0
		0x25,0x64, 		// Logical Maximum 0
		0x35,0x00, 		// Physical Minimum 0
		0x45,0x64, 		// Physical Maximum 0

		0x75,0x08, 		// Report Size 8
		0x95,0x01, 		// Report Count 1

		0x91,0x02, 		// Output (Variable)

	0xC0, // End  Collection Datalink : Usage Set Condition Report ----------ECD: USCR----------



	//-------------------------------------------------------------------------------------------------
	// +++++++++++++++++++++++++++++++	START OF Set Periodic Report	+++++++++++++++++++++++++++++++
	//-------------------------------------------------------------------------------------------------


	0x09,0x6E, // Usage Set Periodic Report

	0xA1,0x02, // START Collection Datalink : Usage Set Periodic Report:			++++++++++SCD: USPR+++++++++++

		0x85,0x06, // Report ID 6

		0x09,0x22, // Usage Effect Block Index

		0x15,0x01, // Logical Minimum 1
		0x25,0x28, // Logical Maximum 28h (40d)
		0x35,0x01, // Physical Minimum 1
		0x45,0x28, // Physical Maximum 28h (40d)

		0x75,0x08, // Report Size 8
		0x95,0x01, // Report Count 1

		0x91,0x02, // Output (Variable)


		0x09,0x70, // Usage Magnitude

		0x15,0x00, 		// Logical Minimum 0
		0x25,0x64, 		// Logical Maximum 100
		0x35,0x00, 		// Physical Minimum 0
		0x45,0x64, 		// Physical Maximum 100

		0x75,0x08, // Report Size 8
		0x95,0x01, // Report Count 1

		0x91,0x02, // Output (Variable)


		0x09,0x6F, // Usage Offset

		0x15,0x00, 		// Logical Minimum 0
		0x25,0x64, 		// Logical Maximum 100
		0x35,0x00, 		// Physical Minimum 0
		0x45,0x64, 		// Physical Maximum 100

		0x75,0x08, // Report Size 8
		0x95,0x01, // Report Count 1

		0x91,0x02, // Output (Variable)


		0x09,0x71, // Usage Phase

		0x65,0x44, // Unit english, rotation, degrees
		0x55,0x01, // Unit Exponent 1

		0x15,0x00, 			// Logical Minimum 0
		0x26,0x68,0x01, 	// Logical Maximum 360
		0x35,0x00, 			// Physical Minimum 0
		0x46,0x68,0x01,		// Physical Maximum 360

		0x75,0x10, // Report Size 16
		0x95,0x01, // Report Count 1

		0x91,0x02, // Output (Variable)


		0x09,0x72, // Usage Period

		0x66,0x01,0x10, // Unit second, SI linear
		0x55,0x0D, 		// Unit Exponent -3  ( we get time in milliseconds)
		0x15,0x01, 		// Logical Minimum 1
		0x35,0x01, 		// Physical Minimum 1
		0x26,0xE8,0x03, // Logical Maximum 1000 ( maximum 1000 milliseconds)
		0x46,0xE8,0x03, // Physical Maximum 1000 ( maximum 1000 milliseconds)


		0x75,0x10, // Report Size 10h (16d)
		0x95,0x01, // Report Count 1

		0x91,0x02, // Output (Variable)

		0x65,0x00,      // Unit 0
		0x55,0x00, 		// Unit Exponent 0

	0xC0, // END Collection Datalink : Usage Set Periodic Report:				---------------ECD: USPR--------------



	//-------------------------------------------------------------------------------------------------
	// +++++++++++++++++++++++++++++++	START OF Set Constant Force Report	+++++++++++++++++++++++++++++++
	//-------------------------------------------------------------------------------------------------


	0x09,0x73, // Usage Set Constant Force Report
	0xA1,0x02, // START Collection Datalink : Set Constant Force Report  			++++++++++SCD: USFR+++++++++++

		0x85,0x07, // Report ID 7

		0x09,0x22, // Usage Effect Block Index

		0x15,0x01, // Logical Minimum 1
		0x25,0x28, // Logical Maximum 28h (40d)
		0x35,0x01, // Physical Minimum 1
		0x45,0x28, // Physical Maximum 28h (40d)

		0x75,0x08, // Report Size 8
		0x95,0x01, // Report Count 1

		0x91,0x02, // Output (Variable)


		0x09,0x70, // Usage Magnitude

		0x15,0x9C, // Logical Minimum 0x9C (-100d)  ( attention, use small endian notation)
		0x25,0x64, // Logical Maximum 0x64 (+100d)
		0x35,0x9C, // Physical Minimum 0x9C (-100d)
		0x45,0x64, // Physical Maximum 0x64 (+100d)

		0x75,0x10, // Report Size 10h (16d) // i leave a 2 byte field for when i will need to make finer adjustments in the 1000's. for now is good +-100
		0x95,0x01, // Report Count 1

		0x91,0x02, // Output (Variable)

	0xC0, //END Collection Datalink : : Set Constant Force Report 					---------------ECD: USFR--------------



	//-------------------------------------------------------------------------------------------------
	// +++++++++++++++++++++++++++++++	START OF Set Ramp Force Report	+++++++++++++++++++++++++++++++
	//-------------------------------------------------------------------------------------------------


	0x09,0x74, // Usage Set Ramp Force Report
	0xA1,0x02, // Collection Datalink

		0x85,0x08, // Report ID 8

		0x09,0x22, // Usage Effect Block Index

		0x15,0x01, // Logical Minimum 1
		0x25,0x28, // Logical Maximum 28h (40d)
		0x35,0x01, // Physical Minimum 1
		0x45,0x28, // Physical Maximum 28h (40d)

		0x75,0x08, // Report Size 8
		0x95,0x01, // Report Count 1

		0x91,0x02, // Output (Variable)


		0x09,0x75, // Usage Ramp Start
		0x09,0x76, // Usage Ramp End

		0x15,0x00, 		// Logical Minimum 0
		0x25,0x64, 		// Logical Maximum 100
		0x35,0x00, 		// Physical Minimum 0
		0x45,0x64, 		// Physical Maximum 100

		0x75,0x08, // Report Size 8
		0x95,0x02, // Report Count 2

		0x91,0x02, // Output (Variable)

	0xC0, // End Collection



	//-------------------------------------------------------------------------------------------------
	// +++++++++++++++++++++++++++++++	START OF Custom Force Data Report	+++++++++++++++++++++++++++
	//-------------------------------------------------------------------------------------------------


	0x09,0x68, // Usage Custom Force Data Rep...
	0xA1,0x02, // Collection Datalink

		0x85,0x09, // Report ID 9

		0x09,0x22, // Usage Effect Block Index

		0x15,0x01, // Logical Minimum 1
		0x25,0x28, // Logical Maximum 28h (40d)
		0x35,0x01, // Physical Minimum 1
		0x45,0x28, // Physical Maximum 28h (40d)

		0x75,0x08, // Report Size 8
		0x95,0x01, // Report Count 1

		0x91,0x02, // Output (Variable)


		0x09,0x6C, // Usage Custom Force Data Offset

		0x15,0x00, // Logical Minimum 0
		0x26,0x10,0x27, // Logical Maximum 2710h (10000d)
		0x35,0x00, // Physical Minimum 0
		0x46,0x10,0x27, // Physical Maximum 2710h (10000d)

		0x75,0x10, // Report Size 10h (16d)
		0x95,0x01, // Report Count 1

		0x91,0x02, // Output (Variable)


		0x09,0x69, // Usage Custom Force Data

		0x15,0x81, // Logical Minimum 81h (-127d)
		0x25,0x7F, // Logical Maximum 7Fh (127d)
		0x35,0x00, // Physical Minimum 0
		0x46,0xFF,0x00, // Physical Maximum FFh (255d)

		0x75,0x08, // Report Size 8
		0x95,0x0C, // Report Count Ch (12d)

		0x92,0x02,0x01, // Output (Variable, Buffered)

	0xC0, // End Collection



	//-------------------------------------------------------------------------------------------------
	// +++++++++++++++++++++++++++++++	START OF Download Force Sample	+++++++++++++++++++++++++++++++
	//-------------------------------------------------------------------------------------------------


	0x09,0x66, // Usage Download Force Sample
	0xA1,0x02, // Collection Datalink
	   0x85,0x0A, // Report ID 10
	   0x05,0x01, // Usage Page Generic Desktop
	   0x09,0x30, // Usage X
	   0x09,0x31, // Usage Y
	   0x15,0x81, // Logical Minimum 81h (-127d)
	   0x25,0x7F, // Logical Maximum 7Fh (127d)
	   0x35,0x00, // Physical Minimum 0
	   0x46,0xFF,0x00, // Physical Maximum FFh (255d)
	   0x75,0x08, // Report Size 8
	   0x95,0x02, // Report Count 2
	   0x91,0x02, // Output (Variable)
	0xC0, // End Collection



	//-------------------------------------------------------------------------------------------------
	// +++++++++++++++++++++++++++++++	START OF Effect Operation Report	+++++++++++++++++++++++++++
	//-------------------------------------------------------------------------------------------------


	0x05,0x0F, // Usage Page Physical Interface
	0x09,0x77, // Usage Effect Operation Report
	0xA1,0x02, // Collection Datalink
	   0x85,0x0B, // Report ID 11
	   0x09,0x22, // Usage Effect Block Index
	   0x15,0x01, // Logical Minimum 1
	   0x25,0x28, // Logical Maximum 28h (40d)
	   0x35,0x01, // Physical Minimum 1
	   0x45,0x28, // Physical Maximum 28h (40d)

	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)

	   0x09,0x78, // Usage Effect Operation
	   0xA1,0x02, // Collection Datalink
		  0x09,0x79, // Usage Op Effect Start
		  0x09,0x7A, // Usage Op Effect Start Solo
		  0x09,0x7B, // Usage Op Effect Stop
		  0x15,0x01, // Logical Minimum 1
		  0x25,0x03, // Logical Maximum 3

		  0x75,0x08, // Report Size 8
		  0x95,0x01, // Report Count 1
		  0x91,0x00, // Output
	   0xC0, // End Collection

	   0x09,0x7C, // Usage Loop Count
	   0x15,0x00, // Logical Minimum 0
	   0x26,0xFF,0x00, // Logical Maximum FFh (255d)
	   0x35,0x00, // Physical Minimum 0
	   0x46,0xFF,0x00, // Physical Maximum FFh (255d)

		0x91,0x02, // Output (Variable)
	0xC0, // End Collection



	//-------------------------------------------------------------------------------------------------
	// +++++++++++++++++++++++++++++++	START OF PID Block Free Report	+++++++++++++++++++++++++++++++
	//-------------------------------------------------------------------------------------------------


	0x09,0x90, // Usage PID Block Free Report
	0xA1,0x02, // Collection Datalink
	   0x85,0x19, // Report ID 25
	   0x09,0x22, // Usage Effect Block Index
	   0x25,0x28, // Logical Maximum 28h (40d)
	   0x15,0x01, // Logical Minimum 1
	   0x35,0x01, // Physical Minimum 1
	   0x45,0x28, // Physical Maximum 28h (40d)
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	0xC0, // End Collection



	//-------------------------------------------------------------------------------------------------
	// +++++++++++++++++++++++++++++++	START OF PID Device Control Report	+++++++++++++++++++++++++++++++++++
	//-------------------------------------------------------------------------------------------------


	0x09,0x95, // Usage PID Device Control Report
	0xA1,0x02, // Collection Datalink
	   0x85,0x0C, // Report ID 12
	   0x09,0x96, // Usage PID Device Control
	   0xA1,0x02, // Collection Datalink
		   0x09,0x97, // Usage DC Enable Actuators
		   0x09,0x98, // Usage DC Disable Actuators
		   0x09,0x99, // Usage DC Stop All Effects
		   0x09,0x9A, // Usage DC Device Reset
		   0x09,0x9B, // Usage DC Device Pause
		   0x09,0x9C, // Usage DC Device Continue
		   0x15,0x01, // Logical Minimum 1
		   0x25,0x06, // Logical Maximum 6
		   0x75,0x08, // Report Size 8
		   0x95,0x01, // Report Count 1
		   0x91,0x00, // Output
	   0xC0, // End Collection
	0xC0, // End Collection



	//-------------------------------------------------------------------------------------------------
	// +++++++++++++++++++++++++++++++	START OF Device Gain Report	+++++++++++++++++++++++++++++++++++
	//-------------------------------------------------------------------------------------------------


	0x09,0x7D, // Usage Device Gain Report
	0xA1,0x02, // Collection Datalink
	   0x85,0x0d, // Report ID 13 0x0D // modified for test
	   0x09,0x7E, // Usage Device Gain
	   0x15,0x00, // Logical Minimum 0
	   0x26,0xFF,0x00, // Logical Maximum FFh (255d)
	   0x35,0x00, // Physical Minimum 0
	   0x46,0xFF,0x00, // Physical Maximum 255
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)
	0xC0, // End Collection



	//-------------------------------------------------------------------------------------------------
	// +++++++++++++++++++++++++++++++	START OF Set Custom Force Report	+++++++++++++++++++++++++++
	//-------------------------------------------------------------------------------------------------


	0x09,0x6B, // Usage Set Custom Force Report
	0xA1,0x02, // Collection Datalink
	   0x85,0x0E, // Report ID 14

	   0x09,0x22, // Usage Effect Block Index
	   0x15,0x01, // Logical Minimum 1
	   0x25,0x28, // Logical Maximum 28h (40d)
	   0x35,0x01, // Physical Minimum 1
	   0x45,0x28, // Physical Maximum 28h (40d)
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)

	   0x09,0x6D, // Usage Sample Count
	   0x15,0x00, // Logical Minimum 0
	   0x26,0xFF,0x00, // Logical Maximum FFh (255d)
	   0x35,0x00, // Physical Minimum 0
	   0x46,0xFF,0x00, // Physical Maximum FFh (255d)
	   0x75,0x08, // Report Size 8
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)

	   0x09,0x51, // Usage Sample Period
	   0x66,0x03,0x10, // Unit 1003h (4099d)
	   0x55,0xFD, // Unit Exponent FDh (253d)
	   0x15,0x00, // Logical Minimum 0
	   0x26,0xFF,0x7F, // Logical Maximum 7FFFh (32767d)
	   0x35,0x00, // Physical Minimum 0
	   0x46,0xFF,0x7F, // Physical Maximum 7FFFh (32767d)
	   0x75,0x10, // Report Size 10h (16d)
	   0x95,0x01, // Report Count 1
	   0x91,0x02, // Output (Variable)

	   0x55,0x00, // Unit Exponent 0
	   0x65,0x00, // Unit 0
	0xC0, // End Collection



	//-------------------------------------------------------------------------------------------------
	// +++++++++++++++++++++++++++++++	START OF Create New Effect Report	+++++++++++++++++++++++++++
	//-------------------------------------------------------------------------------------------------


	0x09,0xAB, // Usage Create New Effect Report
	0xA1,0x02, // Collection Datalink
		0x85,0x0F, // Report ID 15
		0x09,0x25, // Usage Effect Type
		0xA1,0x02, // Collection Datalink
			0x09,0x26, // Usage ET Constant Force
			0x09,0x27, // Usage ET Ramp
			0x09,0x30, // Usage ET Square
			0x09,0x31, // Usage ET Sine
			0x09,0x32, // Usage ET Triangle
			0x09,0x33, // Usage ET Sawtooth Up
			0x09,0x34, // Usage ET Sawtooth Down
			0x09,0x40, // Usage ET Spring
			0x09,0x41, // Usage ET Damper
			0x09,0x42, // Usage ET Inertia
			0x09,0x43, // Usage ET Friction
			0x09,0x28, // Usage ET Custom Force Data
			0x25,0x0C, // Logical Maximum Ch (12d)
			0x15,0x01, // Logical Minimum 1
			0x35,0x01, // Physical Minimum 1
			0x45,0x0C, // Physical Maximum Ch (12d)
			0x75,0x08, // Report Size 8
			0x95,0x01, // Report Count 1
			0xB1,0x00, // Feature
		0xC0, // End Collection

		0x05,0x01, // Usage Page Generic Desktop
		0x09,0x3B, // Usage Reserved
		0x15,0x00, // Logical Minimum 0
		0x26,0xFF,0x01, // Logical Maximum 1FFh (511d)
		0x35,0x00, // Physical Minimum 0
		0x46,0xFF,0x01, // Physical Maximum 1FFh (511d)
		0x75,0x0A, // Report Size Ah (10d)
		0x95,0x01, // Report Count 1
		0xB1,0x02, // Feature (Variable)
		0x75,0x06, // Report Size 6
		0xB1,0x01, // Feature (Constant)
	0xC0, // End Collection



	//-------------------------------------------------------------------------------------------------
	// +++++++++++++++++++++++++++++++	START OF PID Block Load Status	+++++++++++++++++++++++++++++++
	//-------------------------------------------------------------------------------------------------


	0x05,0x0F, // Usage Page Physical Interface
	0x09,0x89, // Usage PID Block Load Status
	0xA1,0x02, // Collection Datalink
		0x85,0x10, // Report ID 16
		0x09,0x22, // Usage Effect Block Index
		0x25,0x28, // Logical Maximum 28h (40d)
		0x15,0x01, // Logical Minimum 1
		0x35,0x01, // Physical Minimum 1
		0x45,0x28, // Physical Maximum 28h (40d)
		0x75,0x08, // Report Size 8
		0x95,0x01, // Report Count 1
		0xB1,0x02, // Feature (Variable)

		0x09,0x8B, // Usage Block Load Status
		0xA1,0x02, // Collection Datalink
			0x09,0x8C, // Usage Block Load Success
			0x09,0x8D, // Usage Block Load Full
			0x09,0x8E, // Usage Block Load Error
			0x25,0x03, // Logical Maximum 3
			0x15,0x01, // Logical Minimum 1
			0x35,0x01, // Physical Minimum 1
			0x45,0x03, // Physical Maximum 3
			0x75,0x08, // Report Size 8
			0x95,0x01, // Report Count 1
			0xB1,0x00, // Feature

			0x09,0xAC,               //   USAGE RAM POOL AVAILABLE
			0x15,0x00,               //   LOGICAL_MINIMUM 0
			0x26,0x96,0x00,          //   LOGICAL_MAXIMUM 150
			0x75,0x08,               //   REPORT_SIZE 8
			0x95,0x01,               //   REPORT_COUNT 1
	        0xB1,0x02, // Feature (Variable)
	    0xC0, // End Collection

	0xC0, // End Collection



	//-------------------------------------------------------------------------------------------------
	// +++++++++++++++++++++++++++++++	START OF PID Pool Report	+++++++++++++++++++++++++++++++++++
	//-------------------------------------------------------------------------------------------------


	0x09,0x7F, // Usage PID Pool Report
	0xA1,0x02, // Collection Datalink
		0x85,0x22, // Report ID 34
		0x09,0x80, // Usage RAM Pool size
		0x75,0x08, // Report Size 8
		0x95,0x01, // Report Count 1
		0x15,0x00, // Logical Minimum 0
		0x35,0x00, // Physical Minimum 0
		//-4 byte
		0x26,0x96,0x00, // Logical Maximum FFFFh (150d)
		0x46,0x96,0x00, // Physical Maximum FFFFh (150d)
		0xB1,0x02, // Feature (Variable)

	/*
	+13 bytes
	ADDING ROM SIZE 0
	*/
		0x09,0x81,               //   [0x0B]USAGE(ROM POOL SIZE)
		0x75,0x08,               //   [0x0F]REPORT_SIZE 8
		0x15,0x00,               //   [0x0C]LOGICAL_MINIMUM 0
		0x26,0x96,0x00,               //   [0x0D]LOGICAL_MAXIMUM 150
		0x95,0x01,               //   [0x0E]REPORT_COUNT 1
	    0xB1,0x02, 				 //   Feature (Variable)
	//+13
	//-------------------------------end adding rom size
		0x09,0x82,               //   [0x13]USAGE(ROM EFFECT BLOCK COUNT)
		0x75,0x08,               //   [0x14]REPORT_SIZE 8
		0x95,0x01,               //   [0x17]REPORT_COUNT 1
		0x15,0x00,               //   [0x15]LOGICAL_MINIMUM 0
		0x26,0x96,0x00,          //   [0x16]LOGICAL_MAXIMUM 150
		0xB1,0x02,               //   [0x18]FEATURE (Variable)

	//13+13-4=+22
	//-------------------------------end adding rom effect block count
	    0x09,0x83, // Usage Simultaneous Effects Max
	    0x26,0xFF,0x00, // Logical Maximum FFh (255d)
	    0x46,0xFF,0x00, // Physical Maximum FFh (255d)
	    0x75,0x08, // Report Size 8
	    0x95,0x01, // Report Count 1
	    0xB1,0x02, // Feature (Variable)

	    0x09,0xA9, // Usage Device Managed Pool
	    0x09,0xAA, // Usage Shared Parameter Blocks
	    0x75,0x01, // Report Size 1
	    0x95,0x02, // Report Count 2
	    0x15,0x00, // Logical Minimum 0
	    0x25,0x01, // Logical Maximum 1
	    0x35,0x00, // Physical Minimum 0
	    0x45,0x01, // Physical Maximum 1
	    0xB1,0x02, // Feature (Variable)

	    0x75,0x06, // Report Size 6
	    0x95,0x01, // Report Count 1
	    0xB1,0x03, // Feature (Constant, Variable)
	0xC0, // End Collection

  /* USER CODE END 0 */
  0xC0    /*     END_COLLECTION	             */
};

/* USER CODE BEGIN PRIVATE_VARIABLES */
int col = 0;
/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;


/* USER CODE BEGIN EXPORTED_VARIABLES */

//extern uint8_t out_buffer[20];

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CUSTOM_HID_Init_FS(void);
static int8_t CUSTOM_HID_DeInit_FS(void);
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t* out_data);

/**
  * @}
  */

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS =
{
  CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS
};

/** @defgroup USBD_CUSTOM_HID_Private_Functions USBD_CUSTOM_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Manage the CUSTOM HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t* out_data)
{
  /* USER CODE BEGIN 6 */
    ffb_setup_set_report_callback(out_data);
    return (USBD_OK);
  /* USER CODE END 6 */
}

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
/*
static int8_t USBD_CUSTOM_HID_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len);
}
*/
/* USER CODE END 7 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

