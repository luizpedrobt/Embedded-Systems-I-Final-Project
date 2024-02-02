/*
 * ssd1306_font.h
 *
 *  Created on: Nov 26, 2023
 *      Author: DELL
 */

#ifndef SSD1306_FONT_H_
#define SSD1306_FONT_H_

#include <stdint.h>

typedef struct {
	const uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	const uint16_t *data; /*!< Pointer to data font data array */
} FontDef;

#define SSD1306_INCLUDE_FONT_7x10
#define SSD1306_INCLUDE_FONT_11x18

#endif // _SSD1306_FONTS_H_
