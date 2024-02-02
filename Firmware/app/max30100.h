/*
 * max30100.h
 *
 *  Created on: Nov 26, 2023
 *      Author: DELL
 */

#ifndef MAX30100_H_
#define MAX30100_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <hard.h>
#include "main.h"

#define ON 	1
#define OFF 0

//MAX30100 CONFIG
#define MAX30100_BYTES_PER_SAMPLE 4
#define MAX30100_SAMPLE_LEN_MAX 16

//REGISTRADORES

//STATUS
#define MAX30100_RegInterrupt 			0x00
#define MAX30100_RegInterruptEnable 	0x01

#define MAX30100_A_FULL		7

//FIFO
#define MAX30100_RegFifoWritePnt 		0x02
#define MAX30100_RegOverFlowCont 		0x03
#define MAX30100_RegFidoReadPnt 		0x04
#define MAX30100_RegFifoDataReg			0x05

//CONFIGURATION
#define MAX30100_RegModeConfig		 	0x06
#define MAX30100_RegSPO2Config 			0x07
#define MAX30100_RegLedConfig			0x09

#define MAX30100_RESET				0x40
#define MAX30100_SHDN				7

#define MAX30100_LED_PW				0

typedef enum max30100_mode_t
{
	max30100_heart_rate = 0x02
} max30100_mode_t;

typedef enum max30100_ledCurrent_t
{
	max30100_ledCrt14_2 = 0x44
} max30100_ledCurrent_t;

typedef enum max30100_led_pw_t
{
	max30100_pw_16_bit = 3
} max30100_led_pw_t;

typedef struct max30100_t
{
	uint16_t ir_samples[16];
	uint16_t red_samples[16];
	uint8_t interrupt_flag;
} max30100_t;

//--------Funcoes
void max30100_init();
void max30100_write(uint8_t reg, uint8_t *buf, uint16_t buflen);
void max30100_read(uint8_t reg, uint8_t *buf, uint16_t buflen);

void max30100_reset();

void max30100_set_a_full(uint8_t enable);
void max30100_on_interrupt();
void max30100_interrupt_handler();
void max30100_shutdown(uint8_t shdn);
void max30100_set_mode(max30100_mode_t mode);
void max30100_set_led_pulse_width(max30100_led_pw_t pw);
void max30100_set_led_current(max30100_ledCurrent_t current);

void max30100_clear_fifo();
void max30100_read_fifo();

uint16_t max30100_convert_value(uint16_t ir_sample);
uint8_t max30100_has_interrupt();

#ifdef __cplusplus
}
#endif



#endif /* MAX30100_H_ */
