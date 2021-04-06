/*
 * analog_operation.h
 *
 *  Created on: Oct 12, 2020
 *      Author: Lenovo
 */

#ifndef ANALOG_OPERATION_H_
#define ANALOG_OPERATION_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_adc.h"
#include "stdbool.h"

typedef struct BATTERY_SENSOR_RAW
{
	uint16_t raw_battery_voltage;
	uint16_t raw_battery_current;
	uint16_t raw_battery_tempBat;
	uint16_t raw_battery_tempRel;
} BATTERY_SENSOR_RAW_t;

typedef struct BATTERY_SENSOR_AVG
{
	bool 		isValid;
	bool 		isBeingProcessed;
	uint8_t 	count;
	uint32_t	avg_battery_voltage;
	uint32_t	avg_battery_current;
	uint32_t	avg_battery_tempBat;
	uint32_t	avg_battery_tempRel;
} BATTERY_SENSOR_AVG_t;

typedef struct BATTERY_SENSOR
{
	float mBATPT;
	float mBATCT;
	uint8_t mBATTemp;
	uint8_t mRELTemp;
	int32_t mPower;
} BATTERY_SENSOR_t;

typedef struct BATTERY_ALARM
{
	bool is_OverVoltage;
	bool is_OverCurrent;
	bool is_OverTemperature;
	bool is_OverCharged;
	bool is_OverDischarged;
} BATTERY_ALARM_t;

void ADC_Init(ADC_HandleTypeDef* handler);
void ADC_Start(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
uint16_t ADC_Read(ADC_TypeDef* ADCx);
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);

#endif /* ANALOG_OPERATION_H_ */
