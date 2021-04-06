/*
 * ssd1306_operation.c
 *
 *  Created on: Oct 15, 2020
 *      Author: Lenovo
 */

#include "ssd1306_operation.h"

static uint8_t ssdBuffer[(SSD1306_HEIGHT*SSD1306_WIDTH)>>3];

void SSD_Startup(void)
{
	/* Put a few times for delay */
	HAL_Delay(200);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0xAE);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x20);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x10);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0xB0);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0xC8);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x00);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x10);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x40);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x81);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0xFF);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0xA1);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0xA6);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0xA8);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x3F);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0xA4);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0xD3);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x00);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0xD5);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0xF0);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0xD9);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x22);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0xDA);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x12);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0xDB);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x20);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x8D);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x14);
	  I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0xAF);
}

void SSD_UpdateScreen(void)
{
	uint8_t m;

	for(m=0; m< 8; m++){
		I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0xB0+m);
		I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x00);
		I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x10);

		I2C_WriteMulti(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x40, &ssdBuffer[SSD1306_WIDTH*m], SSD1306_WIDTH);
	}
}

void SSD_Fill(SSD_Color_t color)
{
	memset(ssdBuffer, (color==SSD_COLOR_BLACK)?0x00:0xFF,sizeof(ssdBuffer));
}

void SSD_DrawPixel(uint16_t x, uint16_t y, SSD_Color_t color)
{
	/* if the input coordinates are extending the dimension */
	if((x >= SSD1306_WIDTH)||(y>=SSD1306_HEIGHT)){return;}

	/* Set Color */
	if(color == SSD_COLOR_WHITE)
	{
		ssdBuffer[x+(y/8) * SSD1306_WIDTH] |= 1 << (y%8);
	} else
	{
		ssdBuffer[x+(y/8) * SSD1306_WIDTH] &= ~(1 << (y%8));
	}
}

void SSD_GotoXY(SSD_Handler_t* handler, uint16_t x, uint16_t y)
{
	handler->currentX = x;
	handler->currentY = y;
}

char SSD_PutChar(char ch, FontDef_t* font, SSD_Handler_t* handler, SSD_Color_t color)
{
	uint32_t i, b, j;

	/* Check Available space in LCD */
	if((SSD1306_WIDTH <= (handler->currentX + font->font_width))||(SSD1306_HEIGHT <= (handler->currentY + font->font_height))){return 0;}

	/* Go through font */
	for(i=0; i < font->font_height; i++)
	{
		b = font->data[(ch-32) * font->font_height + i];
		for(j=0; j < font->font_width; j++)
		{
			if((b<<j) & 0x8000)
			{
				SSD_DrawPixel(handler->currentX + j, (handler->currentY+i), (SSD_Color_t) color);
			}
			else
			{
				SSD_DrawPixel(handler->currentX + j, (handler->currentY+i), (SSD_Color_t) !color);
			}
		}
	}

	handler->currentX += font->font_width;

	return ch;
}

char SSD_PutString(char* str, FontDef_t* font, SSD_Handler_t* handler, SSD_Color_t color)
{
	while(*str)
	{
		if(SSD_PutChar(*str, font, handler, color) != *str)
		{
			return *str;
		}

		str++;
	}

	return *str;
}

void SSD_TurnedOff(void)
{
	I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x0D);
	I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x14);
	I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0xAE);
}

void SSD_TurnedOn(void)
{
	I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x0D);
	I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0x14);
	I2C_Write(SSD1306_CHANNEL, SSD1306_I2C_ADDR, 0x00, 0xAF);
}

void SSD_DrawCircle(int16_t x0, int16_t y0, int16_t r, SSD_Color_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	SSD_DrawPixel(x0, y0 + r, color);
	SSD_DrawPixel(x0, y0 - r, color);
	SSD_DrawPixel(x0 + r, y0, color);
	SSD_DrawPixel(x0 - r, y0, color);

	while(x<y)
	{
		if(f >= 0)
		{
			y--; ddF_x += 2; f+= ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		SSD_DrawPixel(x0 + x, y0 + y, color);
		SSD_DrawPixel(x0 - x, y0 + y, color);
		SSD_DrawPixel(x0 + x, y0 - y, color);
		SSD_DrawPixel(x0 - x, y0 - y, color);

		SSD_DrawPixel(x0 + y, y0 + x, color);
		SSD_DrawPixel(x0 - y, y0 + x, color);
		SSD_DrawPixel(x0 + y, y0 - x, color);
		SSD_DrawPixel(x0 - y, y0 - x, color);
	}
}

void SSD_DrawBitmap(int16_t x0, int16_t y0, unsigned char* bitmap, int16_t width, int16_t height, SSD_Color_t color)
{
	int16_t byteWidth = (width + 7) >> 3;
	uint8_t byte = 0;

	for(int16_t j=0; j<height; j++, y0++)
	{
		for(int16_t i=0; i<width; i++)
		{
			if(i&7)
			{
				byte <<=1;
			} else
			{
				byte = (*(const unsigned char *)(&bitmap[j * byteWidth + i / 8]));
			}
			if(byte & 0x80)
			{
				SSD_DrawPixel(x0+i, y0, color);
			}
		}
	}
}
