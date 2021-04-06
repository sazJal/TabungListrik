/*
 * global_variables.c
 *
 *  Created on: Oct 12, 2020
 *      Author: Lenovo
 */

#include "global_variables.h"

SSD_Handler_t ssdHandler;

char DSG_LABEL[3] = "DSG";
char CHG_LABEL[3] = "CHG";
char IDLE_LABEL[3]= "   ";
char SOC_LABEL[5] = "SoC-%";
char POWER_LABEL[8]= "power-W ";
char TEMP_LABEL[7]= "temp-~C";
char CURR_LABEL[6]= "curr-A";
char VOLT_LABEL[8]= "volt.-V ";

char OCHG_LABEL[16] = "Battery is Full";
char ODSG_LABEL[16] = "Battery is Low";
char OV_LABEL[16]   = "Voltage too High";
char OT_LABEL[16]   = "Battery is Hot";
char OC_LABEL[16]   = "Current too High";

BATTERY_SENSOR_RAW_t batterySensorRaw;
BATTERY_SENSOR_AVG_t batterySensorAvg;
BATTERY_SENSOR_t batterySensor;
BATTERY_ALARM_t batteryAlarm;

uint32_t batt_power;
uint8_t socInt = 0;
uint8_t soc0Int = 0;;
float soc0;
float socSum;
float soct;
float Rintern;
float ocv;
//bool startupFlag = true;
//bool rintFlag = false;
float v0, vt;
uint8_t socFlash;

/* Flash Object */
float 	 socCycle = 0.0;
uint8_t  socCycleMem = 0;
uint16_t CycleCount = 0;

/* variables for Coulomb Counting */
float capConst = 1512.0;
float SoH = 100.0;
const float capacity = 42.0;

uint32_t* dataPtr;
uint32_t buffWrPtr=0;
uint32_t buffRdPtr=0;


BMS_DATA_RAW_t bmsData;
