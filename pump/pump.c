/*
 * pump.c
 *
 *  Created on: Nov 30, 2024
 *      Author: andre
 */

#include "pump.h"

// Initialize the pump by turning it off
void Pump_Init(void) {
    HAL_GPIO_WritePin(PUMP_PORT, PUMP_PIN, GPIO_PIN_RESET); // Ensure pump is off
}

// Turn the pump ON
void Pump_On(void) {
    HAL_GPIO_WritePin(PUMP_PORT, PUMP_PIN, GPIO_PIN_SET); // Set pin high
}

// Turn the pump OFF
void Pump_Off(void) {
    HAL_GPIO_WritePin(PUMP_PORT, PUMP_PIN, GPIO_PIN_RESET); // Set pin low
}

// Toggle the pump ON for a specific duration (blocking)
void Pump_Toggle(uint32_t duration_ms) {
    Pump_On();
    HAL_Delay(duration_ms); // Wait for the specified duration
    Pump_Off();
}
