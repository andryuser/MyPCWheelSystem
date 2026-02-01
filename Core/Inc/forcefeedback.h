/*
 * forcefeedback.h
 *
 *  Created on: 20.10.2021
 *      Author: Andreafff
 */

#ifndef INC_FORCEFEEDBACK_H_
#define INC_FORCEFEEDBACK_H_

#endif /* INC_FORCEFEEDBACK_H_ */

#include "main.h"
#include "usb_device.h"
#include "usbd_customhid.h"
#include "usbd_ctlreq.h"


//+++++++++++++++++++++++++++++++++++++++++++ DEFINES ++++++++++++++++++++++++++++++++++++++++++++++

#define MAX_EFFECT_INDEX 40

#define FALSE 0
#define TRUE 1

#define RPWM TIM2->CCR1
#define LPWM TIM2->CCR2

#define ET_NONE 0
#define ET_CONSTANT_FORCE 1
#define ET_RAMP_FORCE 2
#define ET_SQUARE 3
#define ET_SINE 4
#define ET_TRIANGLE 5
#define ET_SAWTOOTH_UP 6
#define ET_SAWTOOTH_DOWN 7
#define ET_SPRING 8
#define ET_DAMPER 9
#define ET_INERTIA 10
#define ET_FRICTION 11
#define ET_CUSTOM_FORCE 12



//++++++++++++++++++++++++++++++++++++++ FUNCTION PROTOTYPES +++++++++++++++++++++++++++++++++++++++

void ffb_init(void);

void ffb_setup_set_report_callback(uint8_t *report);

void ffb_setup_get_report_callback(uint16_t reqValue);

void ffb_play_effects(void);

//
//
////---------------------check  if force is on ----------------
//
//unsigned short int api_is_constant_force_effect_on(void);
//unsigned short int api_is_sin_force_effect_on(void);
//unsigned short int api_is_triangle_force_effect_on(void);
//unsigned short int api_is_sawtooth_up_force_effect_on(void);
//unsigned short int api_is_spring_force_effect_on(void);
//
////------------------------------------------------------------
//short int api_get_constant_force_effect_magnitude(void);
//short int api_get_constant_force_effect_axis_x_direction(void);
//
//
////---------------------------SPRING EFFECT INTERFACE FUNCTIONS----------------------------------
////--------------------------- X AXIS-----------------------------------
//short int api_get_spring_effect_x_axis_cp_offset(void);
//short int api_get_spring_effect_x_axis_positive_coefficient(void);
//short int api_get_spring_effect_x_axis_negative_coefficient(void);
//short int api_get_spring_effect_x_axis_positive_saturation(void);
//short int api_get_spring_effect_x_axis_negative_saturation(void);
//short int api_get_spring_effect_x_axis_dead_band(void);
//
////--------------------------- Y AXIS-----------------------------------
//short int api_get_spring_effect_y_axis_cp_offset(void);
//short int api_get_spring_effect_y_axis_positive_coefficient(void);
//short int api_get_spring_effect_y_axis_negative_coefficient(void);
//short int api_get_spring_effect_y_axis_positive_saturation(void);
//short int api_get_spring_effect_y_axis_negative_saturation(void);
//short int api_get_spring_effect_y_axis_dead_band(void);
//
//
//
//
//short int api_get_sin_force_x_magnitude(void);
//short int api_get_triangle_force_x_magnitude(void);
//short int api_get_sawtooth_up_force_x_magnitude(void);
//
//
//
//short int api_get_sin_force_x_period(void);
//short int api_get_triangle_force_x_period(void);
//short int api_get_sawtooth_up_force_x_period(void);
