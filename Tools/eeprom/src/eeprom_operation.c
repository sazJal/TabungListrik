/*
 * eeprom_operation.c
 *
 *  Created on: Jan 12, 2021
 *      Author: Lenovo
 */

#include "eeprom_operation.h"

static uint32_t GetPage(uint32_t Address)
{
  for (int indx=0; indx<128; indx++)
  {
	  if((Address < (0x08000000 + (1024 *(indx+1))) ) && (Address >= (0x08000000 + 1024*indx)))
	  {
		  return (0x08000000 + 1024*indx);
	  }
  }

  return -1;
}

uint32_t Flash_Write_Data(uint32_t StartPageAddress, uint32_t * dataWords, uint16_t wordsLength)
{

	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError;
	uint16_t increment=0;

//	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_BSY |
//						   FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR);
	/* Unlock the Flash to enable the flash control register access *************/
	HAL_FLASH_Unlock();

	/* Erase the user Flash area*/

	uint32_t StartPage = GetPage(StartPageAddress);
	uint32_t EndPageAdress = StartPageAddress + wordsLength*4;
	uint32_t EndPage = GetPage(EndPageAdress);

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = StartPage;
	EraseInitStruct.NbPages     = ((EndPage - StartPage)/FLASH_PAGE_SIZE) +1;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	{
		/*Error occurred while page erase.*/
		return HAL_FLASH_GetError ();
	}

    /* Program the user Flash area word by word*/
	increment = 0;
	while (increment < wordsLength)
	   {
	     if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartPageAddress, dataWords[increment]) == HAL_OK)
	     {
	    	 StartPageAddress += 4;  // use StartPageAddress += 2 for half word and 8 for double word
	    	 increment++;
	     }
	     else
	     {
	       /* Error occurred while writing data in Flash memory*/
	    	 return HAL_FLASH_GetError ();
	     }
	   }

	 /* Lock the Flash to disable the flash control register access (recommended
	    to protect the FLASH memory against possible unwanted operation) *********/
	 HAL_FLASH_Lock();

	 return 0;
}


bool Flash_Read_Data(uint32_t StartPageAddress, __IO uint32_t * dataWords)
{
	bool status;

	status = true;
	*dataWords = *(__IO uint32_t *)StartPageAddress;
	if((*dataWords == 0xffffffff)){status = false;}

	return status;
//	while (1)
//	{
//		*dataWords = *(__IO uint32_t *)StartPageAddress;
//		if (*dataWords == 0xffffffff)
//		{
//			*dataWords = '\0';
//			break;
//		}
//		StartPageAddress += 4;
//		dataWords++;
//	}
}
