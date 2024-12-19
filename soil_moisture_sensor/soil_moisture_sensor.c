/*
 * soil_moisture_sensor.c
 *
 *  Created on: Nov 16, 2024
 *      Author: andre
 */

#include "soil_moisture_sensor.h"

// Static variables for storing the configuration
static GPIO_TypeDef *Sensor_VCC_Port;
static uint16_t Sensor_VCC_Pin;
static ADC_HandleTypeDef *Sensor_hadc;
static uint32_t Sensor_ADC_Channel;

// Define calibration parameters
#define SOIL_MOISTURE_MIN 4000
#define SOIL_MOISTURE_MAX 1600

void SoilMoistureSensor_Init(GPIO_TypeDef *VCC_Port, uint16_t VCC_Pin, ADC_HandleTypeDef *hadc, uint32_t ADC_Channel) {
    Sensor_VCC_Port = VCC_Port;
    Sensor_VCC_Pin = VCC_Pin;
    Sensor_hadc = hadc;
    Sensor_ADC_Channel = ADC_Channel;

    HAL_GPIO_WritePin(Sensor_VCC_Port, Sensor_VCC_Pin, GPIO_PIN_RESET);
}

uint16_t SoilMoistureSensor_Read(void) {
	// Power on the soil moisture sensor
	HAL_GPIO_WritePin(Sensor_VCC_Port, Sensor_VCC_Pin, GPIO_PIN_SET);
	HAL_Delay(10);  // Wait for stabilization

	// Configure ADC for soil moisture sensor channel
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = Sensor_ADC_Channel;  // ADC_CHANNEL_2 for soil moisture
	sConfig.Rank = ADC_REGULAR_RANK_1;
	HAL_ADC_ConfigChannel(Sensor_hadc, &sConfig);

	// Start ADC conversion
	HAL_ADC_Start(Sensor_hadc);
	HAL_ADC_PollForConversion(Sensor_hadc, HAL_MAX_DELAY);
	uint16_t adc_value = HAL_ADC_GetValue(Sensor_hadc);
	HAL_ADC_Stop(Sensor_hadc);

	// Power off the soil moisture sensor
	HAL_GPIO_WritePin(Sensor_VCC_Port, Sensor_VCC_Pin, GPIO_PIN_RESET);

	return adc_value;
}

float SoilMoistureSensor_GetCalibratedResult(void) {
    uint16_t raw_value = SoilMoistureSensor_Read();
    float calibrated_value = ((float)(raw_value - SOIL_MOISTURE_MIN) / (SOIL_MOISTURE_MAX - SOIL_MOISTURE_MIN)) * 100.0f;

    if (calibrated_value < 0) calibrated_value = 0;
    if (calibrated_value > 100) calibrated_value = 100;

    return calibrated_value;
}


