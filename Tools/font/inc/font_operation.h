/*
 * font_operation.h
 *
 *  Created on: Oct 15, 2020
 *      Author: Lenovo
 */

#ifndef FONT_INC_FONT_OPERATION_H_
#define FONT_INC_FONT_OPERATION_H_

#include "stm32f1xx_hal.h"

typedef struct FontDef
{
	uint8_t font_width;
	uint8_t font_height;
	const uint16_t *data;
} FontDef_t;

typedef struct FontSize
{
	uint16_t length;
	uint16_t height;
} FontSize_t;

typedef enum CURRENT_DIRECTION
{
	DIRECTION_CHARGE,
	DIRECTION_DISCHARGE,
	DIRECTION_IDLE
} CURRENT_DIRECTION_t;

extern unsigned char TalisLogo[];
extern unsigned char overcharge[];
extern unsigned char overdischarge[];
extern unsigned char overcurrent[];
extern unsigned char overtemperature[];
extern unsigned char overvoltage[];
extern unsigned char nopower[];
extern unsigned char logo [];
extern unsigned char myBitmap2 [];
extern unsigned char bat78_28[];
extern unsigned char low_bat78_34[];
extern unsigned char bar5_18[];
extern unsigned char stop15_15[];
extern unsigned char arrow_left14_8[];
extern unsigned char arrow_right14_8[];
extern unsigned char tarrow_right22_8[];
extern unsigned char tarrow_left22_8[];
extern unsigned char straight22_8[];
extern FontDef_t Font_7x10;
extern FontDef_t Font_11x18;
extern FontDef_t Font_16x26;

#endif /* FONT_INC_FONT_OPERATION_H_ */
