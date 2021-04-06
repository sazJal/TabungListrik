/*
 * global_variables.h
 *
 *  Created on: Oct 12, 2020
 *      Author: Lenovo
 */

#ifndef GLOBAL_VARIABLES_H_
#define GLOBAL_VARIABLES_H_

#include "hc05_operation.h"
#include "analog_operation.h"
#include "math_operation.h"
#include "bms_operation.h"
#include "ssd1306_operation.h"
#include "eeprom_operation.h"

SSD_Handler_t ssdHandler;

char DSG_LABEL[3];
char CHG_LABEL[3];
char IDLE_LABEL[3];
char SOC_LABEL[5];
char POWER_LABEL[8];
char TEMP_LABEL[7];
char CURR_LABEL[6];
char VOLT_LABEL[8];

char OCHG_LABEL[16];
char ODSG_LABEL[16];
char OV_LABEL[16];
char OT_LABEL[16];
char OC_LABEL[16];

BATTERY_SENSOR_RAW_t batterySensorRaw;
BATTERY_SENSOR_AVG_t batterySensorAvg;
BATTERY_SENSOR_t batterySensor;
BATTERY_ALARM_t batteryAlarm;

uint32_t batt_power;
uint8_t socInt;
uint8_t soc0Int;
float soc0;
float socSum;
float soct;
float Rintern;
float ocv;
float v0, vt;

/* variables for Coulomb Counting */
float capConst;
float SoH;
const float capacity;
uint8_t socFlash;

/* Flash Object */
float socCycle;
uint8_t  socCycleMem;
uint16_t CycleCount;
uint32_t* dataPtr;
uint32_t buffWrPtr;
uint32_t buffRdPtr;


//bool startupFlag;
//bool rintFlag;
BMS_DATA_RAW_t bmsData;

#endif /* GLOBAL_VARIABLES_H_ */
