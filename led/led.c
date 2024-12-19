/*
 * led.c
 *
 *  Created on: Dec 19, 2024
 *      Author: godmaze
 */

#include "led.h"


void ledInit(GPIO_TypeDef* gpio_in, uint16_t in, GPIO_TypeDef* gpio_out, uint16_t out){
	LED_IN = in;
	LED_OUT = out;
	GPIO_IN = GPIOA;
	GPIO_OUT = GPIOA;
}

void ledOn(){
	HAL_GPIO_WritePin(GPIO_OUT, LED_OUT, SET);
}

void ledOff(){
	HAL_GPIO_WritePin(GPIO_OUT, LED_OUT, RESET);
}

void ledToggle(){
	HAL_GPIO_TogglePin(GPIOA, LED_OUT);
}

GPIO_PinState isLight() {
	return HAL_GPIO_ReadPin(GPIO_IN, LED_IN);
}
