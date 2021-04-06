/*
 * math_operation.c
 *
 *  Created on: Oct 12, 2020
 *      Author: Lenovo
 */

#include "math_operation.h"

void MATH_MovingAverage(BATTERY_SENSOR_AVG_t* sensorAvg, BATTERY_SENSOR_RAW_t* sensorRaw)
{
	static BATTERY_SENSOR_AVG_t sensorAvgBuff;
	static uint8_t count = 0;

	if(!sensorAvg->isValid)
	{
		sensorAvgBuff.avg_battery_voltage = sensorAvgBuff.avg_battery_voltage + sensorRaw->raw_battery_voltage;
		sensorAvgBuff.avg_battery_current = sensorAvgBuff.avg_battery_current + sensorRaw->raw_battery_current;
		sensorAvgBuff.avg_battery_tempBat = sensorAvgBuff.avg_battery_tempBat + sensorRaw->raw_battery_tempBat;
		sensorAvgBuff.avg_battery_tempRel = sensorAvgBuff.avg_battery_tempRel + sensorRaw->raw_battery_tempRel;

		if(count == 63)
		{
			sensorAvg->avg_battery_voltage = sensorAvgBuff.avg_battery_voltage >> 6;
			sensorAvg->avg_battery_current = sensorAvgBuff.avg_battery_current >> 6;
			sensorAvg->avg_battery_tempBat = sensorAvgBuff.avg_battery_tempBat >> 6;
			sensorAvg->avg_battery_tempRel = sensorAvgBuff.avg_battery_tempRel >> 6;
			sensorAvgBuff.avg_battery_voltage = 0;
			sensorAvgBuff.avg_battery_current = 0;
			sensorAvgBuff.avg_battery_tempBat = 0;
			sensorAvgBuff.avg_battery_tempRel = 0;
			sensorAvg->isValid = true;
			count = 0;
		}
		else
		{
			count++;
		}
	}

}
