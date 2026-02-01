/*
 * forcefeedback.c
 *
 *  Created on: 20.10.2021
 *      Author: Andreafff
 */


#include "forcefeedback.h"






//++++++++++++++++++++++++++++++++++++++++++ TYPEDEFS ++++++++++++++++++++++++++++++++++++++++++++++

struct _envelope_block
{
	uint8_t attackLevel; // 0-100
	uint8_t fadeLevel; // 0-100
	uint16_t attackTime; // 0-1000 ms
	uint16_t fadeTime; // 0-1000 ms
};

struct _condition_block
{
	uint8_t parameterBlockOffset; // 0-1
	int8_t cpOffset; // -100-100
	uint8_t positiveCoefficient; // 0-100
	uint8_t negativeCoefficient; // 0-100
	uint8_t positiveSaturation; // 0-100
	uint8_t negativeSaturation; // 0-100
	uint8_t deadBand; // 0-100
};

struct _periodic_block
{
	uint8_t magnitude; // 0-100
	uint8_t offset; // 0-100
	uint16_t phase; // 0-360 degrees
	uint16_t period; // 1-1000 ms
};

struct _constant_force_block
{
	int16_t magnitude; // -100-100
};

struct _ramp_force_block
{
	uint8_t rampStart; // 0-100
	uint8_t rampEnd; // 0-100
};

struct _effect_block
{
	uint8_t effectType; // 1-12
	uint16_t duration; // 0-10000 ms
	uint16_t triggerRepeatInterval; // 0-10000 ms
	uint16_t samplePeriod; // 0-10000 ms
	float gain; // 0.01-1
	uint8_t triggerButton; // 1-8
	uint8_t xEnable; // 0-1
	uint8_t yEnable; // 0-1
	uint8_t directionEnable; // 0-1
	uint8_t typeSpecificBlockOffset1; // 0-1
	uint8_t typeSpecificBlockOffset2; // 0-1
	uint16_t directionInstance1; // 0-360 degrees
	uint16_t directionInstance2; // 0-360 degrees
	uint16_t startDelay; // 0-32767 ms
	struct _envelope_block envelope_block;
	struct _condition_block condition_block[2];
	struct _periodic_block periodic_block;
	struct _constant_force_block constant_force_block;
	struct _ramp_force_block ramp_force_block;
};







//++++++++++++++++++++++++++++++++++++++++++ VARIABLES ++++++++++++++++++++++++++++++++++++++++++++++

extern USBD_HandleTypeDef hUsbDeviceFS;

uint8_t isEnabledActuator; // 0-1
float deviceGain; // 0-20.47

// effect_block[0] will be completely ignored, as if it did not exist
struct _effect_block effect_block[1 + MAX_EFFECT_INDEX];

uint8_t biggestEffectIndex;
uint8_t nextNewEffectIndex;

// effectStarted[0] will be completely ignored, as if it did not exist
uint8_t effectStarted[1 + MAX_EFFECT_INDEX];

uint8_t pid_block_load_report[4];
uint8_t pid_pool_report[6];


// PWM = 2047 = gain_factor[100] * deviceGain_factor[255] * 100
// 2047 / 100 = gain_factor[100] * deviceGain_factor[255]
// 2047 / (100 * gain_factor[100]) = deviceGain_factor[255] = 20.47

//uint8_t gain_factor[101] =
//{
//	0.00, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09,
//	0.10, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18, 0.19,
//	0.20, 0.21, 0.22, 0.23, 0.24, 0.25, 0.26, 0.27, 0.28, 0.29,
//	0.30, 0.31, 0.32, 0.33, 0.34, 0.35, 0.36, 0.37, 0.38, 0.39,
//	0.40, 0.41, 0.42, 0.43, 0.44, 0.45, 0.46, 0.47, 0.48, 0.49,
//	0.50, 0.51, 0.52, 0.53, 0.54, 0.55, 0.56, 0.57, 0.58, 0.59,
//	0.60, 0.61, 0.62, 0.63, 0.64, 0.65, 0.66, 0.67, 0.68, 0.69,
//	0.70, 0.71, 0.72, 0.73, 0.74, 0.75, 0.76, 0.77, 0.78, 0.79,
//	0.80, 0.81, 0.82, 0.83, 0.84, 0.85, 0.86, 0.87, 0.88, 0.89,
//	0.90, 0.91, 0.92, 0.93, 0.94, 0.95, 0.96, 0.97, 0.98, 0.99,
//	1.00
//};

//uint8_t deviceGain_factor[256] =
//{
//	0.00, 0.08, 0.16, 0.24, 0.32, 0.40, 0.48, 0.56, 0.64, 0.72,
//	0.80, 0.88, 0.96, 1.04, 1.12, 1.20, 1.28, 1.36, 1.44, 1.53,
//	1.61, 1.69, 1.77, 1.85, 1.93, 2.01, 2.09, 2.17, 2.25, 2.33,
//	2.41, 2.49, 2.57, 2.65, 2.73, 2.81, 2.89, 2.97, 3.05, 3.13,
//	3.21, 3.29, 3.37, 3.45, 3.53, 3.61, 3.69, 3.77, 3.85, 3.93,
//	4.01, 4.09, 4.17, 4.25, 4.33, 4.42, 4.50, 4.58, 4.66, 4.74,
//	4.82, 4.90, 4.98, 5.06, 5.14, 5.22, 5.30, 5.38, 5.46, 5.54,
//	5.62, 5.70, 5.78, 5.86, 5.94, 6.02, 6.10, 6.18, 6.26, 6.34,
//	6.42, 6.50, 6.58, 6.66, 6.74, 6.82, 6.90, 6.98, 7.06, 7.14,
//	7.22, 7.30, 7.39, 7.47, 7.55, 7.63, 7.71, 7.79, 7.87, 7.95,
//	8.03, 8.11, 8.19, 8.27, 8.35, 8.43, 8.51, 8.59, 8.67, 8.75,
//	8.83, 8.91, 8.99, 9.07, 9.15, 9.23, 9.31, 9.39, 9.47, 9.55,
//	9.63, 9.71, 9.79, 9.87, 9.95, 10.03, 10.11, 10.19, 10.28, 10.36,
//	10.44, 10.52, 10.60, 10.68, 10.76, 10.84, 10.92, 11.00, 11.08, 11.16,
//	11.24, 11.32, 11.40, 11.48, 11.56, 11.64, 11.72, 11.80, 11.88, 11.96,
//	12.04, 12.12, 12.20, 12.28, 12.36, 12.44, 12.52, 12.60, 12.68, 12.76,
//	12.84, 12.92, 13.00, 13.08, 13.17, 13.25, 13.33, 13.41, 13.49, 13.57,
//	13.65, 13.73, 13.81, 13.89, 13.97, 14.05, 14.13, 14.21, 14.29, 14.37,
//	14.45, 14.53, 14.61, 14.69, 14.77, 14.85, 14.93, 15.01, 15.09, 15.17,
//	15.25, 15.33, 15.41, 15.49, 15.57, 15.65, 15.73, 15.81, 15.89, 15.97,
//	16.05, 16.14, 16.22, 16.30, 16.38, 16.46, 16.54, 16.62, 16.70, 16.78,
//	16.86, 16.94, 17.02, 17.10, 17.18, 17.26, 17.34, 17.42, 17.50, 17.58,
//	17.66, 17.74, 17.82, 17.90, 17.98, 18.06, 18.14, 18.22, 18.30, 18.38,
//	18.46, 18.54, 18.62, 18.70, 18.78, 18.86, 18.94, 19.03, 19.11, 19.19,
//	19.27, 19.35, 19.43, 19.51, 19.59, 19.67, 19.75, 19.83, 19.91, 19.99,
//	20.07, 20.15, 20.23, 20.31, 20.39, 20.47
//};








//++++++++++++++++++++++++++++++++++++++++++ PRIVATE FUNCTION DECLARATIONS ++++++++++++++++++++++++++++++++++++++++++++++

void device_control(uint8_t *pid_device_control_report);

void getNewEffectIndex(void);

void prepare_pid_block_load_report(uint8_t *create_new_effect_report);

void effect_operation(uint8_t *effect_operation_report);

void start_effect(uint8_t effectIndex);

void stop_effect(uint8_t effectIndex);

void enable_actuator(void);

void disable_actuator(void);

void device_reset(void);

void stop_all_effects(void);

void pause_all_effects(void);

void continue_all_effects(void);

void pid_block_free(uint8_t effectIndex);

void equate_pwm(uint8_t effectIndex);

void add_pwm(uint8_t effectIndex);

void set_effect(uint8_t *set_effect_report);

void set_envelope(uint8_t *set_envelope_report);

void set_condition(uint8_t *set_condition_report);

void set_periodic(uint8_t *set_periodic_report);

void set_constant_force(uint8_t *set_constant_force_report);

void set_ramp_force(uint8_t *set_ramp_force_report);

void nullify_effect_block(uint8_t effectIndex);








//++++++++++++++++++++++++++++++++++++++++++ FUNCTION DEFINITIONS ++++++++++++++++++++++++++++++++++++++++++++++

void ffb_init(void)
{
	isEnabledActuator = FALSE;
	deviceGain = 0;
	nextNewEffectIndex = 0;

	for (uint8_t i = 1; i <= MAX_EFFECT_INDEX; i++)
	{
		effectStarted[i] = FALSE;
		effect_block[i].effectType = ET_NONE;
	}

	LPWM = 0;
	RPWM = 0;
}



void ffb_setup_set_report_callback(uint8_t *report)     // is called in usbd_custom_hid_if.c in CUSTOM_HID_OutEvent_FS()
{
	switch (report[0])
	{
		case 0x03: // Set Effect Report
			set_effect(report);
			break;

		case 0x04: // Set Envelope Report
			set_envelope(report);
			break;

		case 0x05: // Set Condition Report
			set_condition(report);
			break;

		case 0x06: // Set Periodic Report
			set_periodic(report);
			break;

		case 0x07: // Set Constant Force Report
			set_constant_force(report);
			break;

		case 0x08: // Set Ramp Force Report
			set_ramp_force(report);
			break;

		case 0x09: // Custom Force Data Report (not supported)
			break;

		case 0x0a: // Download Force Sample (not supported)
			break;

		case 0x0b: // Effect Operation Report
			effect_operation(report);
			break;

		case 0x0c: // PID Device Control
			device_control(report);
			break;

		case 0x0d: // Device Gain Report
			deviceGain = 0.08027 * report[1];  // report[1] = Device Gain, 0.08027 = 2047 / (100*255)
			break;

		case 0x0e: // Set Custom Force Report (not supported)
			break;

		case 0x0f: // Create New Effect Report
			prepare_pid_block_load_report(report);
			break;

		case 0x19: // PID Block Free Report
			pid_block_free(report[1]);  // report[1] = Effect Block Index
			break;
	}
}


void ffb_setup_get_report_callback(uint16_t reqValue)     // is called in usbd_customhid.c in USBD_CUSTOM_HID_Setup()
{
	switch (LOBYTE(reqValue))
	{
		case 0x10:  // REPORT ID 16  PID_BLOCK_LOAD_STATUS
			switch(HIBYTE(reqValue))  // REPORT TYPE
			{
				 case 0x01:  // INPUT REPORT
				 break;
				 case 0x02:  // OUTPUT, the host wants to send me the data
				 break;
				 case 0x03:  // FEATURE, the host wants ME to send the data
					 USBD_CtlSendData(&hUsbDeviceFS, pid_block_load_report, 4);
				 break;
			}
			break;

		case 0x22:  // REPORT ID 34  pid_pool_report
			pid_pool_report[0] = 0x22;	// report ID 34
			pid_pool_report[1] = 120;   // RAM POOL SIZE 120
			pid_pool_report[2] = 0;		// ROM POOL SIZE 0
			pid_pool_report[3] = 0;		// ROM EFFECT BLOCK COUNT 0
			pid_pool_report[4] = 2;		// SIMULTANEOUS EFFECT MAX 2
			pid_pool_report[5] = 0b00000001;  // DEVICE MANAGED POOL/NO SHARED PARAMETER BLOCKS

			switch(HIBYTE(reqValue))  // REPORT TYPE
			{
				 case 0x01:  // INPUT REPORT
				 break;
				 case 0x02:  // OUTPUT, the host wants to send me the data
				 break;
				 case 0x03:  // FEATURE, the host wants ME to send the data
						USBD_CtlSendData(&hUsbDeviceFS, pid_pool_report, 6);
				 break;
			}
			break;
	}
}


void ffb_play_effects()
{
	uint8_t effectIndex = 1;
	uint8_t isFirstEffect = TRUE;

	LPWM = 0;
	RPWM = 0;

	if (isEnabledActuator == FALSE)
	{
		return;
	}

	while (effectIndex <= MAX_EFFECT_INDEX &&
		   effect_block[effectIndex].effectType != ET_NONE)
	{
		switch (effect_block[effectIndex].effectType)
		{
			case ET_CONSTANT_FORCE:

				if (isFirstEffect)
				{
					isFirstEffect = FALSE;
					equate_pwm(effectIndex);
				}
				else
				{
					add_pwm(effectIndex);
				}
				break;

			case ET_RAMP_FORCE:

				break;

			case ET_SQUARE:

				break;

			case ET_SINE:

				break;

			case ET_TRIANGLE:

				break;

			case ET_SAWTOOTH_UP:

				break;

			case ET_SAWTOOTH_DOWN:

				break;

			case ET_SPRING:

				break;

			case ET_DAMPER:

				break;

			case ET_INERTIA:

				break;

			case ET_FRICTION:

				break;

			case ET_CUSTOM_FORCE: // NOT SUPPORTED BY THIS DEVICE !

				break;
		}

		effectIndex++;
	}
}


void enable_actuator(void)
{
	isEnabledActuator = TRUE;
	HAL_GPIO_WritePin(R_EN_GPIO_Port, R_EN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(L_EN_GPIO_Port, L_EN_Pin, GPIO_PIN_SET);
}


void disable_actuator(void)
{
	stop_all_effects();
	isEnabledActuator = FALSE;
	LPWM = 0;
	RPWM = 0;
	HAL_GPIO_WritePin(R_EN_GPIO_Port, R_EN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(L_EN_GPIO_Port, L_EN_Pin, GPIO_PIN_RESET);
}


void device_control(uint8_t *pid_device_control_report)
{
	/*
		pid_device_control_report[0] // report id PID Device Control Report
		pid_device_control_report[1] // PID Device Control
	*/
	switch(pid_device_control_report[1])
	{
		case 1: // Enable Actuators
			enable_actuator();
			break;

		case 2: // Disable Actuators
			disable_actuator();
			break;

		case 3: // Stop All Effects
			stop_all_effects();
			break;

		case 4: // Device Reset
			device_reset();
			break;

		case 5: // Device Pause
			pause_all_effects();
			break;

		case 6: // Device Continue
			continue_all_effects();
			break;
	}
}


void getNewEffectIndex(void)
{
	for (uint8_t i = 1; i <= MAX_EFFECT_INDEX; i++){
		if (effect_block[i].effectType == ET_NONE)
		{
			nextNewEffectIndex = i;
			break;
		}
	}
}


void prepare_pid_block_load_report(uint8_t *create_new_effect_report)
{
	/*
		create_new_effect_report[0] // report id PID BLOCK LOAD STATUS
		create_new_effect_report[1] // effect type

		pid_block_load_report[0] // report id PID BLOCK LOAD STATUS
		pid_block_load_report[1] // effect block index
		pid_block_load_report[2] // block load status 1-3 (Block Load Success, Block Load Full, Block Load Error)
		pid_block_load_report[3] // ram pool available 0-150
	*/
	pid_block_load_report[0] = 0x10;   // report id PID BLOCK LOAD STATUS



	switch (create_new_effect_report[1]) // switch effect type
	{
		case ET_CONSTANT_FORCE:
			getNewEffectIndex();
			pid_block_load_report[1] = nextNewEffectIndex; // 0:can't create effect | 1-40  =  effect block index
			pid_block_load_report[2] = 1; // 3:can't create effect | 1: can create effect
			pid_block_load_report[3] = 0xFF; // i still have ram...
			break;

		case ET_RAMP_FORCE:
			getNewEffectIndex();
			pid_block_load_report[1] = nextNewEffectIndex; // 0:can't create effect | 1-40  =  effect block index
			pid_block_load_report[2] = 1; // 3:can't create effect | 1: can create effect
			pid_block_load_report[3] = 0xFF; // i still have ram...
			break;

		case ET_SQUARE:
			getNewEffectIndex();
			pid_block_load_report[1] = nextNewEffectIndex; // 0:can't create effect | 1-40  =  effect block index
			pid_block_load_report[2] = 1; // 3:can't create effect | 1: can create effect
			pid_block_load_report[3] = 0xFF; // i still have ram...
			break;

		case ET_SINE:
			getNewEffectIndex();
			pid_block_load_report[1] = nextNewEffectIndex; // 0:can't create effect | 1-40  =  effect block index
			pid_block_load_report[2] = 1; // 3:can't create effect | 1: can create effect
			pid_block_load_report[3] = 0xFF; // i still have ram...
			break;

		case ET_TRIANGLE:
			getNewEffectIndex();
			pid_block_load_report[1] = nextNewEffectIndex; // 0:can't create effect | 1-40  =  effect block index
			pid_block_load_report[2] = 1; // 3:can't create effect | 1: can create effect
			pid_block_load_report[3] = 0xFF; // i still have ram...
			break;

		case ET_SAWTOOTH_UP:
			getNewEffectIndex();
			pid_block_load_report[1] = nextNewEffectIndex; // 0:can't create effect | 1-40  =  effect block index
			pid_block_load_report[2] = 1; // 3:can't create effect | 1: can create effect
			pid_block_load_report[3] = 0xFF; // i still have ram...
			break;

		case ET_SAWTOOTH_DOWN:
			getNewEffectIndex();
			pid_block_load_report[1] = nextNewEffectIndex; // 0:can't create effect | 1-40  =  effect block index
			pid_block_load_report[2] = 1; // 3:can't create effect | 1: can create effect
			pid_block_load_report[3] = 0xFF; // i still have ram...
			break;

		case ET_SPRING:
			getNewEffectIndex();
			pid_block_load_report[1] = nextNewEffectIndex; // 0:can't create effect | 1-40  =  effect block index
			pid_block_load_report[2] = 1; // 3:can't create effect | 1: can create effect
			pid_block_load_report[3] = 0xFF; // i still have ram...
			break;

		case ET_DAMPER:
			getNewEffectIndex();
			pid_block_load_report[1] = nextNewEffectIndex; // 0:can't create effect | 1-40  =  effect block index
			pid_block_load_report[2] = 1; // 3:can't create effect | 1: can create effect
			pid_block_load_report[3] = 0xFF; // i still have ram...
			break;

		case ET_INERTIA:
			getNewEffectIndex();
			pid_block_load_report[1] = nextNewEffectIndex; // 0:can't create effect | 1-40  =  effect block index
			pid_block_load_report[2] = 1; // 3:can't create effect | 1: can create effect
			pid_block_load_report[3] = 0xFF; // i still have ram...
			break;

		case ET_FRICTION:
			getNewEffectIndex();
			pid_block_load_report[1] = nextNewEffectIndex; // 0:can't create effect | 1-40  =  effect block index
			pid_block_load_report[2] = 1; // 3:can't create effect | 1: can create effect
			pid_block_load_report[3] = 0xFF; // i still have ram...
			break;

		case ET_CUSTOM_FORCE: // NOT SUPPORTED BY THIS DEVICE !
			pid_block_load_report[1] = 0; // 0:can't create effect | 1-40  =  effect block index
			pid_block_load_report[2] = 3; // 3:can't create effect | 1: can create effect
			pid_block_load_report[3] = 0x00; // no more ram
			break;
	}
	// now, when the host issues a get_report , i send the pid_block_load_report
}


void effect_operation(uint8_t *effect_operation_report)
{
	/*
		effect_operation_report[0] // report id effect_operation_report
		effect_operation_report[1] // effect block index
		effect_operation_report[2] // effect operation 1-3 (Effect Start, Effect Start Solo, Effect Stop)
		effect_operation_report[3] // loop count 0-255
	*/

	uint8_t index = effect_operation_report[1];

	switch (effect_operation_report[2])
	{
		case 1: // Effect Start
			start_effect(index);
			break;

		case 2: // Effect Start Solo
			stop_all_effects();
			start_effect(index);
			break;

		case 3: // Effect Stop
			stop_effect(index);
			break;
	}
}


void start_effect(uint8_t effectIndex)
{
	effectStarted[effectIndex] = TRUE;
}


void stop_effect(uint8_t effectIndex)
{
	effectStarted[effectIndex] = FALSE;
}


void device_reset(void)
{

}


void stop_all_effects(void)
{
	for(uint8_t i = 0; i < MAX_EFFECT_INDEX; i++)
	{
		stop_effect(i);
	}
}


void pause_all_effects(void)
{

}


void continue_all_effects(void)
{

}


void pid_block_free(uint8_t effectIndex)
{
	stop_effect(effectIndex);
	nullify_effect_block(effectIndex);
}


void equate_pwm(uint8_t effectIndex)
{
	if (effect_block[effectIndex].constant_force_block.magnitude >= 0)
	{
		LPWM = (uint16_t)
				(effect_block[effectIndex].constant_force_block.magnitude
				* effectStarted[effectIndex]
				* deviceGain
				* effect_block[effectIndex].gain
				+ 0.5);
	}
	else // magnitude < 0
	{
		RPWM = (uint16_t)
				(abs(effect_block[effectIndex].constant_force_block.magnitude)
				* effectStarted[effectIndex]
				* deviceGain
				* effect_block[effectIndex].gain
				+ 0.5);
	}
}


void add_pwm(uint8_t effectIndex)
{
	uint16_t tmp = 0;
	int16_t pwmDifference = 0;

	pwmDifference = (int16_t)
				 (effect_block[effectIndex].constant_force_block.magnitude
				 * effectStarted[effectIndex]
				 * deviceGain
				 * effect_block[effectIndex].gain
				 + 5000.5) - 5000;

	if (pwmDifference >= 0)  // positive -> left
	{
		if (LPWM > 0 && RPWM == 0) // motor is rotating left
		{
			LPWM += pwmDifference;
		}
		else if (LPWM == 0 && RPWM >= pwmDifference) // motor is rotating right fast
		{
			RPWM -= pwmDifference;
		}
		else if (LPWM == 0 && RPWM < pwmDifference) // motor is rotating right slow
		{
			tmp = RPWM;
			RPWM = 0;
			LPWM = pwmDifference - tmp;
		}
	}
	else // pwmDifference < 0, negative -> right
	{
		if (RPWM > 0 && LPWM == 0) // motor is rotating right
		{
			RPWM += abs(pwmDifference);
		}
		else if (RPWM == 0 && LPWM >= abs(pwmDifference)) // motor is rotating left fast
		{
			LPWM -= abs(pwmDifference);
		}
		else if (RPWM == 0 && LPWM < abs(pwmDifference)) // motor is rotating left slow
		{
			tmp = LPWM;
			LPWM = 0;
			RPWM = abs(pwmDifference) - tmp;
		}
	}
}


void set_effect(uint8_t *set_effect_report)
{
	uint8_t index = set_effect_report[1];
	effect_block[index].effectType = set_effect_report[2];
	effect_block[index].duration = (uint16_t) set_effect_report[3] | (uint16_t) (set_effect_report[4] << 8);
	effect_block[index].triggerRepeatInterval = (uint16_t) set_effect_report[5] | (uint16_t) (set_effect_report[6] << 8);
	effect_block[index].samplePeriod = (uint16_t) set_effect_report[7] | (uint16_t) (set_effect_report[8] << 8);
	effect_block[index].gain = 0.01 * set_effect_report[9];
	effect_block[index].triggerButton = set_effect_report[10];
	effect_block[index].xEnable = set_effect_report[11] & 0b00000001;
	effect_block[index].yEnable = (set_effect_report[11] & 0b00000010) >> 1;
	effect_block[index].directionEnable = (set_effect_report[11] & 0b00000100) >> 2;
	effect_block[index].typeSpecificBlockOffset1 = (set_effect_report[11] & 0b00110000) >> 4;
	effect_block[index].typeSpecificBlockOffset2 = (set_effect_report[11] & 0b11000000) >> 6;
	effect_block[index].directionInstance1 = (uint16_t) set_effect_report[12] | (uint16_t) (set_effect_report[13] << 8);
	effect_block[index].directionInstance2 = (uint16_t) set_effect_report[14] | (uint16_t) (set_effect_report[15] << 8);
	effect_block[index].startDelay = (uint16_t) set_effect_report[16] | (uint16_t) (set_effect_report[17] << 8);
}


void set_envelope(uint8_t *set_envelope_report)
{
	uint8_t index = set_envelope_report[1];
	effect_block[index].envelope_block.attackLevel = set_envelope_report[2];
	effect_block[index].envelope_block.fadeLevel = set_envelope_report[3];
	effect_block[index].envelope_block.attackTime = (uint16_t) set_envelope_report[4] | (uint16_t) (set_envelope_report[5] << 8);
	effect_block[index].envelope_block.fadeTime = (uint16_t) set_envelope_report[6] | (uint16_t) (set_envelope_report[7] << 8);
}


void set_condition(uint8_t *set_condition_report)
{
	uint8_t index = set_condition_report[1];
	uint8_t pBOffset = set_condition_report[2] & 0b00001111;
	effect_block[index].condition_block[pBOffset].parameterBlockOffset = pBOffset;
	effect_block[index].condition_block[pBOffset].cpOffset = set_condition_report[3];
	effect_block[index].condition_block[pBOffset].positiveCoefficient = set_condition_report[4];
	effect_block[index].condition_block[pBOffset].negativeCoefficient = set_condition_report[5];
	effect_block[index].condition_block[pBOffset].positiveSaturation = set_condition_report[6];
	effect_block[index].condition_block[pBOffset].negativeSaturation = set_condition_report[7];
	effect_block[index].condition_block[pBOffset].deadBand = set_condition_report[8];
}


void set_periodic(uint8_t *set_periodic_report)
{
	uint8_t index = set_periodic_report[1];
	effect_block[index].periodic_block.magnitude = set_periodic_report[2];
	effect_block[index].periodic_block.offset = set_periodic_report[3];
	effect_block[index].periodic_block.phase = (uint16_t) set_periodic_report[4] | (uint16_t) (set_periodic_report[5] << 8);
	effect_block[index].periodic_block.period = (uint16_t) set_periodic_report[6] | (uint16_t) (set_periodic_report[7] << 8);
}


void set_constant_force(uint8_t *set_constant_force_report)
{
	uint8_t index = set_constant_force_report[1];
	effect_block[index].constant_force_block.magnitude =
			(int16_t) set_constant_force_report[2] | (int16_t) (set_constant_force_report[3] << 8);
}


void set_ramp_force(uint8_t *set_ramp_force_report)
{
	uint8_t index = set_ramp_force_report[1];
	effect_block[index].ramp_force_block.rampStart = set_ramp_force_report[2];
	effect_block[index].ramp_force_block.rampEnd = set_ramp_force_report[3];
}


void nullify_effect_block(uint8_t effectIndex)
{
	effect_block[effectIndex].effectType = ET_NONE;
	effect_block[effectIndex].duration = 0;
	effect_block[effectIndex].triggerRepeatInterval = 0;
	effect_block[effectIndex].samplePeriod = 0;
	effect_block[effectIndex].gain = 0;
	effect_block[effectIndex].triggerButton = 0;
	effect_block[effectIndex].xEnable = FALSE;
	effect_block[effectIndex].yEnable = FALSE;
	effect_block[effectIndex].directionEnable = FALSE;
	effect_block[effectIndex].typeSpecificBlockOffset1 = 0;
	effect_block[effectIndex].typeSpecificBlockOffset2 = 0;
	effect_block[effectIndex].directionInstance1 = 0;
	effect_block[effectIndex].directionInstance2 = 0;
	effect_block[effectIndex].startDelay = 0;
	effect_block[effectIndex].envelope_block.attackLevel = 0;
	effect_block[effectIndex].envelope_block.fadeLevel = 0;
	effect_block[effectIndex].envelope_block.attackTime = 0;
	effect_block[effectIndex].envelope_block.fadeTime = 0;
	effect_block[effectIndex].condition_block[0].parameterBlockOffset = 0;
	effect_block[effectIndex].condition_block[0].cpOffset = 0;
	effect_block[effectIndex].condition_block[0].positiveCoefficient = 0;
	effect_block[effectIndex].condition_block[0].negativeCoefficient = 0;
	effect_block[effectIndex].condition_block[0].positiveSaturation = 0;
	effect_block[effectIndex].condition_block[0].negativeSaturation = 0;
	effect_block[effectIndex].condition_block[0].deadBand = 0;
	effect_block[effectIndex].condition_block[1].parameterBlockOffset = 0;
	effect_block[effectIndex].condition_block[1].cpOffset = 0;
	effect_block[effectIndex].condition_block[1].positiveCoefficient = 0;
	effect_block[effectIndex].condition_block[1].negativeCoefficient = 0;
	effect_block[effectIndex].condition_block[1].positiveSaturation = 0;
	effect_block[effectIndex].condition_block[1].negativeSaturation = 0;
	effect_block[effectIndex].condition_block[1].deadBand = 0;
	effect_block[effectIndex].periodic_block.magnitude = 0;
	effect_block[effectIndex].periodic_block.offset = 0;
	effect_block[effectIndex].periodic_block.phase = 0;
	effect_block[effectIndex].periodic_block.period = 0;
	effect_block[effectIndex].constant_force_block.magnitude = 0;
	effect_block[effectIndex].ramp_force_block.rampStart = 0;
	effect_block[effectIndex].ramp_force_block.rampEnd = 0;
}







