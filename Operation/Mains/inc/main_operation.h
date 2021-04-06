/*
 * main_operation.h
 *
 *  Created on: Oct 12, 2020
 *      Author: Lenovo
 */

#ifndef MAINS_INC_MAIN_OPERATION_H_
#define MAINS_INC_MAIN_OPERATION_H_

#include "stdbool.h"

typedef enum OPERATION_STATE
{
	STARTUP,
	IDLE,
	CHG,
	DSG,
	WARNING
} OPERATION_STATE_t;

typedef struct BUTTON_FLAG
{
	bool bat_flag;
	bool inv_flag;
} BUTTON_FLAG_t;

typedef struct STATE_FLAG
{
	bool flash_flag;
	bool batFull_flag;
	bool rInt_flag;
	bool startUp_flag;
	bool warning_flag;
} STATE_FLAG_t;

typedef enum ERROR_SIGN
{
	SIGN_NORMAL,	// normal is default
	SIGN_OCHG,		// over charge, battery is already at 100%
	SIGN_ODSG,		// over discharge, battery is already below 10%
	SIGN_OV,		// charging input voltage above the specified charging voltage
	SIGN_OT,		// battery over heat
	SIGN_OC,		// battery discharged/charged with higher current
	SIGN_NP			// charger is not plugged or charger power is not sufficient
} ERROR_SIGN_t;

BUTTON_FLAG_t buttonFlag;
STATE_FLAG_t  stateFlag;

#endif /* MAINS_INC_MAIN_OPERATION_H_ */
