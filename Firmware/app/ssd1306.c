#include "ssd1306.h"

static SSD1306_t SSD1306;
static uint8_t SSD1306_Buffer[SSD1306_BUFFER_SIZE];

//--------Inicializa display
void ssd1306_Init() {
	ssd1306_SetDisplayOnOff(OFF); //display off

	//Fundamental Command
	ssd1306_SetContrast(SSD1306_CONTRAST_CONTROL);
	ssd1306_WriteCommand(SSD1306_ENT_DSP_ON);
	ssd1306_WriteCommand(SSD1306_SET_NRML_DSP);

	//Addressing setting
	ssd1306_SetMemAdress(SSD1306_ADDR_MODE_HOR);
	ssd1306_SetColumnAdress(SSD1306_COL_ADDR_HI);
	ssd1306_SetColumnAdress(SSD1306_COL_ADDR_LW);
	ssd1306_SetPageAdress(SSD1306_PAGE_ADDR_HI);
	ssd1306_SetPageAdress(SSD1306_PAGE_ADDR_LW);

	//Hardware Configuration
	ssd1306_WriteCommand(SSD1306_SET_START_LINE);
	ssd1306_WriteCommand(SSD1306_SET_RE_MAP);
	ssd1306_SetMultiplexRatio(SSD1306_MUX_64);
	ssd1306_WriteCommand(SSD1306_SET_COM_OUT_SCAN_DIRECT);
	ssd1306_SetDisplayOffset(SSD1306_DIS_OFFSET);
	ssd1306_SetComPinsHwConfig(SSD1306_COM_PIN);

	//Timing & driving setting
	ssd1306_SetDisClockDivide(SSD1306_DIS_CLK_DIVIDE);
	ssd1306_SetPreChargePeriod(SSD1306_PRE_CHARGE_PERIOD);
	ssd1306_SetDeselectLevel(SSD1306_DESLECT_LEVEL);

	//Liga display
	ssd1306_SetDisplayOnOff(ON);

	ssd1306_Fill(Black);

	ssd1306_UpdateScreen();

	//Valores inciais
	SSD1306.Initialized = ON;
	SSD1306.DisplayOnOff = ON;

	SSD1306.CurrentX = 73;
	SSD1306.CurrentY = 10;

	char bpm[10] = "BPM";

	ssd1306_WriteString(bpm, Font_11x18, White);

	SSD1306.CurrentX = 62;
	SSD1306.CurrentY = 10;

}

//--------Funcoes de escrita/desenho

//Envia informações para um registrador
void ssd1306_WriteCommand(uint8_t byte) {
	hard_I2C_Mem_Write(&SSD1306_I2C_PORT, SSD1306_I2C_ADDR, 0x00, 1, &byte, 1, SSD1306_I2C_TIMEOUT);
}

// Envia Data
void ssd1306_WriteData(uint8_t* buffer, size_t buff_size) {
	hard_I2C_Mem_Write(&SSD1306_I2C_PORT, SSD1306_I2C_ADDR, SSD1306_SET_START_LINE, 1, buffer, buff_size, SSD1306_I2C_TIMEOUT);
}

//Preenche a tela
void ssd1306_Fill(SSD1306_COLOR color) {
	uint32_t i;
	for(i = 0; i < sizeof(SSD1306_Buffer); i++) {
		SSD1306_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
	}
}

//Desenha um pixel
void ssd1306_DrawPixel(uint8_t cord_x, uint8_t cord_y, SSD1306_COLOR color) {
	if(cord_x >= SSD1306_WIDTH || cord_y >= SSD1306_HEIGHT) {
		return;
	}
	if(color == White) {
		SSD1306_Buffer[cord_x + (cord_y / 8) * SSD1306_WIDTH] |= 1 << (cord_y % 8);
	} else {
		SSD1306_Buffer[cord_x + (cord_y / 8) * SSD1306_WIDTH] &= ~(1 << (cord_y % 8));
	}
}

//Escreve um char
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color) {
	uint32_t i, b, j;
	//Verifica char válido tabela ascii
	if (ch < 32 || ch > 126)
		return 0;

	//Verifica espaço na linha
	if (SSD1306_WIDTH < (SSD1306.CurrentX + Font.FontWidth) || SSD1306_HEIGHT < (SSD1306.CurrentY + Font.FontHeight)){
		return 0;
	}

	for(i = 0; i < Font.FontHeight; i++) {
		b = Font.data[(ch - 32) * Font.FontHeight + i];
		for(j = 0; j < Font.FontWidth; j++) {
			if((b << j) & 0x8000)  {
				ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color);
			} else {
				ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR)!color);
			}
		}
	}

	SSD1306.CurrentX += Font.FontWidth;

	return ch;
}

//Escreve uma string
char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color) {
	while (*str) {
		if (ssd1306_WriteChar(*str, Font, color) != *str) {
			//Erro na escrita
			return *str;
		}
		str++;
	}
	return *str;
}

//Desenha retangulo preenchido
void ssd1306_FillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_COLOR color) {
	uint8_t x_start = ((x1<=x2) ? x1 : x2);
	uint8_t x_end   = ((x1<=x2) ? x2 : x1);
	uint8_t y_start = ((y1<=y2) ? y1 : y2);
	uint8_t y_end   = ((y1<=y2) ? y2 : y1);

	for (uint8_t y= y_start; (y<= y_end)&&(y<SSD1306_HEIGHT); y++) {
		for (uint8_t x= x_start; (x<= x_end)&&(x<SSD1306_WIDTH); x++) {
			ssd1306_DrawPixel(x, y, color);
		}
	}
	return;
}

//--------Funcoes de inicializacao
void ssd1306_SetDisplayOnOff(const uint8_t on) {
	uint8_t value;
	if (on) {
		value = SSD1306_DISPLAYON;
		SSD1306.DisplayOnOff = 1;
	} else {
		value = SSD1306_DISPLAYOFF;
		SSD1306.DisplayOnOff = 0;
	}
	ssd1306_WriteCommand(value);
}

//Seta posiçao do cursor
void ssd1306_SetCursor(uint8_t x, uint8_t y) {
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}

void ssd1306_UpdateScreen(void) {
	for(uint8_t i = 0; i < SSD1306_HEIGHT/8; i++) {
		ssd1306_WriteCommand(SSD1306_PAGE_0 + i);
		ssd1306_WriteCommand(SSD1306_LW_COL_START + SSD1306_X_OFFSET_LOWER);
		ssd1306_WriteCommand(SSD1306_HI_COL_START + SSD1306_X_OFFSET_UPPER);
		ssd1306_WriteData(&SSD1306_Buffer[SSD1306_WIDTH*i],SSD1306_WIDTH);
	}
}

void ssd1306_SetContrast(const uint8_t value) {
	ssd1306_WriteCommand(SSD1306_RegSetContrastControl);
	ssd1306_WriteCommand(value);
}

void ssd1306_SetMemAdress(const uint8_t value){
	ssd1306_WriteCommand(SSD1306_RegSetMemoryAddressingMode);
	ssd1306_WriteCommand(value);
}

void ssd1306_SetColumnAdress(const uint8_t value){
	ssd1306_WriteCommand(SSD1306_RegSetColumnAddress);
	ssd1306_WriteCommand(value);
}

void ssd1306_SetPageAdress(const uint8_t value){
	ssd1306_WriteCommand(SSD1306_RegSetPageAddress);
	ssd1306_WriteCommand(value);
}

void ssd1306_SetMultiplexRatio(const uint8_t value){
	ssd1306_WriteCommand(SSD1306_RegSetMultiplexRatio);
	ssd1306_WriteCommand(value);
}

void ssd1306_SetDisplayOffset(const uint8_t value){
	ssd1306_WriteCommand(SSD1306_RegSetDisplayOffset);
	ssd1306_WriteCommand(value);
}

void ssd1306_SetComPinsHwConfig(const uint8_t value){
	ssd1306_WriteCommand(SSD1306_RegSetComPinsHwConfig);
	ssd1306_WriteCommand(value);
}

void ssd1306_SetDisClockDivide(const uint8_t value){
	ssd1306_WriteCommand(SSD1306_RegSetDisplayClockDivide);
	ssd1306_WriteCommand(value);
}

void ssd1306_SetPreChargePeriod(const uint8_t value){
	ssd1306_WriteCommand(SSD1306_RegSetPreChargePeriod);
	ssd1306_WriteCommand(value);
}

void ssd1306_SetDeselectLevel(const uint8_t value){
	ssd1306_WriteCommand(SSD1306_RegSetDeselectLevel);
	ssd1306_WriteCommand(value);
}

uint8_t ssd1306_GetDisplayOnOff() {
	return SSD1306.DisplayOnOff;
}
