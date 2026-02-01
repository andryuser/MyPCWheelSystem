/*
 * pcf8574events.c
 *
 *  Created on: 29.04.2021
 *      Author: aerfurt
 */

#include "pcf8574events.h"


void reset_highbeam_blinker_wiper_buttons(uint8_t *buttons)
{
	*buttons = *buttons & (~HIGHBEAM);
	*buttons = *buttons & (~BLINKER_R);
	*buttons = *buttons & (~BLINKER_L);
	*buttons = *buttons & (~WIPER_UP);
	*buttons = *buttons & (~WIPER_DOWN);
}



void repair_pcf8574(uint8_t *pcf_data_ptr, I2C_HandleTypeDef *hi2c_ptr)
{
	if(*pcf_data_ptr == 0xff){       // Falls komischerweise alle Eingänge 1 sind (was nicht sein soll)
		*pcf_data_ptr = 0x00;
		HAL_I2C_Master_Transmit(hi2c_ptr, 0x20 << 1, pcf_data_ptr, 1, 200);
	}
}


void set_highbeam_blinker_buttons(uint8_t *buttons, uint8_t pcf_data, uint8_t pcf_data_old)
{
	if( (pcf_data & HIGHBEAM) != (pcf_data_old & HIGHBEAM) ){
		*buttons = *buttons | HIGHBEAM;
	}
	if( (pcf_data & BLINKER_R) != (pcf_data_old & BLINKER_R) ){
		*buttons = *buttons | BLINKER_R;
	}
	if( (pcf_data & BLINKER_L) != (pcf_data_old & BLINKER_L) ){
		*buttons = *buttons | BLINKER_L;
	}
}


uint8_t get_wiper(uint8_t pcf_data)
{
	uint8_t wiper = 0;

	if( (pcf_data & WIPER_1) != 0 ) {
	  wiper = 1;
	}
	else if( (pcf_data & WIPER_2) != 0 ){
	  wiper = 2;
	}
	else if( (pcf_data & WIPER_3) != 0 ){
	  wiper = 3;
	}

	return wiper;
}


void set_wiper_buttons(uint8_t *buttons, uint8_t wiper, uint8_t *wiper_old, uint8_t *wiper_increment)
{
	if(wiper == *wiper_old + 1){
	  *buttons = *buttons | WIPER_UP;
	  *wiper_old = wiper;
	}
	if(wiper == *wiper_old - 1){
		*buttons = *buttons | WIPER_DOWN;
	  *wiper_old = wiper;
	}
	if(wiper == *wiper_old + 2){
	  if(*wiper_increment != 1){
		  *buttons = *buttons | WIPER_UP;
	  }
	  (*wiper_increment)++;
	  if(*wiper_increment == 3){
		  *wiper_old = wiper;
		  *wiper_increment = 0;
	  }
	}
	if(wiper == *wiper_old - 2){
	  if(*wiper_increment != 1){
		  *buttons = *buttons | WIPER_DOWN;
	  }
	  (*wiper_increment)++;
	  if(*wiper_increment == 3){
		  *wiper_old = wiper;
		  *wiper_increment = 0;
	  }
	}
}
