/*
 * eeprom_operation.h
 *
 *  Created on: Jan 12, 2021
 *      Author: Lenovo
 */

#ifndef EEPROM_INC_EEPROM_OPERATION_H_
#define EEPROM_INC_EEPROM_OPERATION_H_

#include "stm32f1xx_hal.h"
#include "stdbool.h"

uint32_t Flash_Write_Data(uint32_t StartPageAddress, uint32_t * dataWords, uint16_t wordsLength);
//void Flash_Read_Data(uint32_t StartPageAddress, __IO uint32_t * DATA_32);
bool Flash_Read_Data(uint32_t StartPageAddress, __IO uint32_t * dataWords);
#endif /* EEPROM_INC_EEPROM_OPERATION_H_ */
