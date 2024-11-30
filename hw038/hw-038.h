/*
 * hw-038.h
 *
 *  Created on: Nov 24, 2024
 *      Author: andre
 */

#ifndef INC_HW_038_H_
#define INC_HW_038_H_

#include "stm32f3xx_hal.h"

void Hw_Init(GPIO_TypeDef *VCC_Port, uint16_t VCC_Pin, ADC_HandleTypeDef *hadc, uint32_t ADC_Channel);
uint16_t Hw_Read(void);
float Hw_GetCalibratedResult(void);

#endif /* INC_HW_038_H_ */
