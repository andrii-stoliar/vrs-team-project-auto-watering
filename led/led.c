/*
 * led.c
 *
 *  Created on: Dec 19, 2024
 *      Author: godmaze
 */

#include "led.h"


void ledInit(GPIO_TypeDef* gpio_in, uint8_t in, GPIO_TypeDef* gpio_out, uint8_t out){
	LED_IN = in;
	LED_OUT = out;
	GPIO_IN = gpio_in;
	GPIO_OUT = gpio_out;
}

void ledOn(){
	HAL_GPIO_WritePin(GPIO_OUT, LED_OUT, SET);
}

void ledOff(){
	HAL_GPIO_WritePin(GPIO_OUT, LED_OUT, RESET);
}

GPIO_PinState isLight() {
	return HAL_GPIO_ReadPin(GPIO_IN, LED_IN);
}
