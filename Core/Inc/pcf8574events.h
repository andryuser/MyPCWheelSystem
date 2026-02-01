/*
 * pcf8574events.h
 *
 *  Created on: 29.04.2021
 *      Author: aerfurt
 */

#ifndef INC_PCF8574EVENTS_H_
#define INC_PCF8574EVENTS_H_

#include "main.h"


#define HIGHBEAM 1 << 1
#define BLINKER_R 1 << 2
#define BLINKER_L 1 << 3
#define WIPER_3 1 << 4
#define WIPER_2 1 << 5
#define WIPER_1 1 << 6
#define WIPER_UP 1 << 4
#define WIPER_DOWN 1 << 5


void reset_highbeam_blinker_wiper_buttons(uint8_t *buttons);

void repair_pcf8574(uint8_t *pcf_data_ptr, I2C_HandleTypeDef *hi2c_ptr);

void set_highbeam_blinker_buttons(uint8_t *buttons, uint8_t pcf_data, uint8_t pcf_data_old);

uint8_t get_wiper(uint8_t pcf_data);

void set_wiper_buttons(uint8_t *buttons, uint8_t wiper, uint8_t *wiper_old, uint8_t *wiper_increment);


#endif /* INC_PCF8574EVENTS_H_ */
