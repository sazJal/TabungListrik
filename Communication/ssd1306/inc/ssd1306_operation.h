/*
 * ssd1306_operation.h
 *
 *  Created on: Oct 15, 2020
 *      Author: Lenovo
 */

#ifndef SSD1306_INC_SSD1306_OPERATION_H_
#define SSD1306_INC_SSD1306_OPERATION_H_

#include "font_operation.h"
#include "i2c_operation.h"
#include "string.h"

#define SSD1306_CHANNEL		I2C2
#define SSD1306_I2C_ADDR	0x78
#define SSD1306_WIDTH		128
#define SSD1306_HEIGHT		64

typedef struct SSD_Handler
{
	uint16_t currentX;
	uint16_t currentY;
} SSD_Handler_t;

typedef enum SSD_Color
{
	SSD_COLOR_BLACK,
	SSD_COLOR_WHITE
} SSD_Color_t;

void SSD_Startup(void);
void SSD_UpdateScreen(void);
void SSD_Fill(SSD_Color_t color);
void SSD_DrawPixel(uint16_t x, uint16_t y, SSD_Color_t color);
void SSD_GotoXY(SSD_Handler_t* handler, uint16_t x, uint16_t y);
char SSD_PutChar(char ch, FontDef_t* font, SSD_Handler_t* handler, SSD_Color_t color);
char SSD_PutString(char* str, FontDef_t* font, SSD_Handler_t* handler, SSD_Color_t color);
void SSD_TurnedOff(void);
void SSD_TurnedOn(void);
void SSD_DrawCircle(int16_t x0, int16_t y0, int16_t r, SSD_Color_t color);
void SSD_DrawBitmap(int16_t x0, int16_t y0, unsigned char* bitmap, int16_t width, int16_t height, SSD_Color_t color);

#endif /* SSD1306_INC_SSD1306_OPERATION_H_ */
