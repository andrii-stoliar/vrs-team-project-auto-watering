/*
 * led.h
 *
 *  Created on: Dec 19, 2024
 *      Author: godmaze
 */

#ifndef LED_H_
#define LED_H_

#include "stm32f3xx_hal.h"

static uint8_t LED_IN, LED_OUT;
static GPIO_TypeDef* GPIO_IN;
static GPIO_TypeDef* GPIO_OUT;

void ledInit(GPIO_TypeDef* gpio_in, uint8_t in, GPIO_TypeDef* gpio_out, uint8_t out);
void ledOn();
void ledOff();

GPIO_PinState isLight();

#endif /* LED_H_ */
