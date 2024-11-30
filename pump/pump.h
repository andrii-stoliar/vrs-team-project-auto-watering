/*
 * pump.h
 *
 *  Created on: Nov 30, 2024
 *      Author: andre
 */

#ifndef PUMP_H_
#define PUMP_H_

#include "main.h"

// Define the GPIO pin for the pump
#define PUMP_PIN GPIO_PIN_4
#define PUMP_PORT GPIOA

// Function prototypes
void Pump_Init(void); // Initializes the pump (turns it off initially)
void Pump_On(void);   // Turns the pump ON
void Pump_Off(void);  // Turns the pump OFF
void Pump_Toggle(uint32_t duration_ms); // Turns pump ON for a specific duration

#endif /* PUMP_H_ */
