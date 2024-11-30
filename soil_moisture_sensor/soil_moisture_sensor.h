/*
 * soil_moisture_sensor.h
 *
 *  Created on: Nov 16, 2024
 *      Author: andre
 */

#ifndef INC_SOIL_MOISTURE_SENSOR_H_
#define INC_SOIL_MOISTURE_SENSOR_H_

#include "stm32f3xx_hal.h"

void SoilMoistureSensor_Init(GPIO_TypeDef *VCC_Port, uint16_t VCC_Pin, ADC_HandleTypeDef *hadc, uint32_t ADC_Channel);
uint16_t SoilMoistureSensor_Read(void);
float SoilMoistureSensor_GetCalibratedResult(void);

#endif /* INC_SOIL_MOISTURE_SENSOR_H_ */
