/*
 * hard.h
 *
 *  Created on: Nov 24, 2023
 *      Author: DELL
 */

#ifndef HARD_H_
#define HARD_H_


#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stm32l0xx_hal.h>
#include "main.h"
#include "max30100.h"
#include "ssd1306.h"
#include "ssd1306_font.h"

//CONFIGURA MAX30100
extern TIM_HandleTypeDef 		htim3;
extern I2C_HandleTypeDef 		hi2c2;
#define MAX30100_TIM_PORT 		htim3
#define MAX30100_I2C_PORT 		hi2c2
#define MAX30100_I2C_ADDR		(0x57 << 1) //slave address
#define MAX30100_I2C_TIMEOUT 	100

//CONFIGURA DISPLAY
extern I2C_HandleTypeDef 		hi2c3;
#define SSD1306_I2C_PORT        hi2c3
#define SSD1306_I2C_ADDR        (0x3C << 1) //slave address
#define SSD1306_I2C_TIMEOUT 	100

//CONFIGURA BUZZER
extern TIM_HandleTypeDef 		htim2;
#define BUZZER_TIM_PORT 		htim2


#define hard_statusTypeDef HAL_StatusTypeDef


#define TAM_MAX 50

void hard_init();
void hard_get_sample_max30100(uint16_t ir_sample);
void hard_max30100_start_interrupt(void);
void hard_buzzer_pwm_start(void);
void hard_buzzer_tim_set_counter(uint16_t cnt);
void hard_buzzer_tim_set_arr(uint16_t cnt);
void hard_buzzer_tim_set_ccr(uint16_t cnt);
void hard_cpu_sleep(void);

void hard_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
void hard_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
void hard_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);


#endif /* HARD_H_ */
