/*
 * ssd1306.h
 *
 *  Created on: Nov 26, 2023
 *      Author: DELL
 */

#ifndef SSD1306_H_
#define SSD1306_H_


#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hard.h"
#include "ssd1306_font.h"

#ifdef __cplusplus
extern "C" {
#endif

//FONTE
extern FontDef Font_7x10;
extern FontDef Font_11x18;

//TAMANHO DO DISPLAY
#define SSD1306_HEIGHT          64
#define SSD1306_WIDTH           128
#define SSD1306_X_OFFSET_LOWER  0
#define SSD1306_X_OFFSET_UPPER	0
#define SSD1306_BUFFER_SIZE   	SSD1306_WIDTH * SSD1306_HEIGHT / 8

#define ON 	1
#define OFF	0

//REGISTRADORES
#define SSD1306_RegSetContrastControl 			0x81
#define SSD1306_RegSetMemoryAddressingMode  	0x20
#define SSD1306_RegSetColumnAddress 			0x21
#define SSD1306_RegSetPageAddress 				0x22
#define SSD1306_RegSetMultiplexRatio  			0xA8
#define SSD1306_RegSetDisplayOffset				0xD3
#define SSD1306_RegSetComPinsHwConfig			0xDA
#define SSD1306_RegSetDisplayClockDivide		0xD5
#define SSD1306_RegSetPreChargePeriod			0xD9
#define SSD1306_RegSetDeselectLevel				0xDB

//COMANDOS
//FUNDAMENTAL COMMAND
#define SSD1306_CONTRAST_CONTROL		0x7F
#define	SSD1306_ENT_DSP_ON				0xA4
#define	SSD1306_SET_NRML_DSP			0xA6
#define SSD1306_DISPLAYON				0xAF
#define SSD1306_DISPLAYOFF				0xAE


//ADDRESSING SETTING COMMAND
#define SSD1306_ADDR_MODE_HOR 	0x00
#define SSD1306_COL_ADDR_LW		0x00
#define SSD1306_COL_ADDR_HI		0x7F
#define SSD1306_PAGE_ADDR_LW	0x00
#define SSD1306_PAGE_ADDR_HI	0x07
#define SSD1306_PAGE_0			0xB0
#define SSD1306_LW_COL_START	0x00
#define SSD1306_HI_COL_START	0x10


//HARDWARE CONFIGURATION COMMAND
#define	SSD1306_SET_START_LINE			0x40
#define	SSD1306_SET_RE_MAP				0xA1
#define SSD1306_MUX_64					0x3F
#define	SSD1306_SET_COM_OUT_SCAN_DIRECT 0xC8
#define SSD1306_DIS_OFFSET 				0x00
#define SSD1306_COM_PIN					0x12

//TIMING & DRIVING SETTING COMMAND
#define SSD1306_DIS_CLK_DIVIDE			0x80
#define	SSD1306_PRE_CHARGE_PERIOD		0x22
#define SSD1306_DESLECT_LEVEL			0x20

typedef enum {
	Black = 0x00, // Pixel resetado
	White = 0x01  // Pixel setado
} SSD1306_COLOR;

typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Initialized;
	uint8_t DisplayOnOff;
} SSD1306_t;

//--------Funcoes
void ssd1306_Init();
void ssd1306_WriteCommand(uint8_t byte);
void ssd1306_WriteData(uint8_t* buffer, size_t buff_size);
void ssd1306_Fill(SSD1306_COLOR color);
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color);
char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color);
void ssd1306_FillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_COLOR color);

void ssd1306_SetDisplayOnOff(const uint8_t on);
void ssd1306_SetCursor(uint8_t x, uint8_t y);
void ssd1306_UpdateScreen(void);
void ssd1306_SetContrast(const uint8_t value);
void ssd1306_SetMemAdress(const uint8_t value);
void ssd1306_SetColumnAdress(const uint8_t value);
void ssd1306_SetPageAdress(const uint8_t value);
void ssd1306_SetMultiplexRatio(const uint8_t value);
void ssd1306_SetDisplayOffset(const uint8_t value);
void ssd1306_SetComPinsHwConfig(const uint8_t value);
void ssd1306_SetDisClockDivide(const uint8_t value);
void ssd1306_SetPreChargePeriod(const uint8_t value);
void ssd1306_SetDeselectLevel(const uint8_t value);
uint8_t ssd1306_GetDisplayOnOff();

#ifdef __cplusplus
}
#endif


#endif /* SSD1306_H_ */
