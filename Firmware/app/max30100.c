/*
 * max30100.c
 *
 *  Created on: Nov 26, 2023
 *      Author: DELL
 */

#include <max30100.h>

#ifdef __cplusplus
extern "C"
{
#endif


static max30100_t max30100;

//Inicializaçao do sensor
void max30100_init()
{
	max30100.interrupt_flag = 0;
	memset(max30100.ir_samples, 0, MAX30100_SAMPLE_LEN_MAX * sizeof(uint16_t));
	memset(max30100.red_samples, 0, MAX30100_SAMPLE_LEN_MAX * sizeof(uint16_t));

	max30100_reset();
	max30100_clear_fifo();

	//seta configuracoes
	max30100_set_led_pulse_width(max30100_pw_16_bit);
	max30100_set_led_current(max30100_ledCrt14_2);
	max30100_set_mode(max30100_heart_rate);
	max30100_set_a_full(ON);

}

//--------Funcoes de escrita/leitura

//Envia informações para um registrador
void max30100_write(uint8_t reg, uint8_t *buf, uint16_t buflen)
{
	uint8_t *payload = (uint8_t *) malloc((buflen + 1) * sizeof(uint8_t));
	*payload = reg;
	if (buf != NULL && buflen != 0)
		memcpy(payload + 1, buf, buflen);
	hard_I2C_Master_Transmit(&MAX30100_I2C_PORT, MAX30100_I2C_ADDR, payload, buflen + 1, MAX30100_I2C_TIMEOUT);
	free(payload);
}

//Coleta informacao do registrador e a copia em buf
void max30100_read(uint8_t reg, uint8_t *buf, uint16_t buflen)
{
	uint8_t reg_addr = reg;
	hard_I2C_Master_Transmit(&MAX30100_I2C_PORT, MAX30100_I2C_ADDR, &reg_addr, 1, MAX30100_I2C_TIMEOUT);
	hard_I2C_Master_Receive(&MAX30100_I2C_PORT, MAX30100_I2C_ADDR, buf, buflen, MAX30100_I2C_TIMEOUT);
}

//Reseta o sensor
void max30100_reset()
{
	uint8_t reg_addr = MAX30100_RESET;
	max30100_write(MAX30100_RegModeConfig, &reg_addr, 1);
}

//--------Funcoes de inicializacao

//Ativa full interrupt
void max30100_set_a_full(uint8_t enable)
{
	uint8_t reg = 0;
	max30100_read(MAX30100_RegInterruptEnable, &reg, 1);
	reg &= ~(1 << MAX30100_A_FULL);
	reg |= ((enable & 1) << MAX30100_A_FULL);
	max30100_write(MAX30100_RegInterruptEnable, &reg, 1);
}

//seta flag de interrupcao, é chamada pelo TIM
void max30100_on_interrupt()
{
	max30100.interrupt_flag = 1;
}

//Tratamento de interrupcoes
void max30100_interrupt_handler()
{
	uint8_t reg[2] = {0x00};
	// As flgas de interrupção são limpas na leitura
	max30100_read(MAX30100_RegInterrupt, reg, 2);

	if ((reg[0] >> MAX30100_A_FULL) & 0x01)
	{
		// FIFO almost full
		max30100_read_fifo();
	}

	// Reseta interrupt flag
	max30100.interrupt_flag = 0;
}

//shutdown system
void max30100_shutdown(uint8_t shdn)
{
	uint8_t config;
	max30100_read(MAX30100_RegModeConfig, &config, 1);
	config = (config & 0x7f) | (shdn << MAX30100_SHDN);
	max30100_write(MAX30100_RegModeConfig, &config, 1);
}

//Seta modo de leitura para heart rate ou spo2
void max30100_set_mode(max30100_mode_t mode)
{
	uint8_t config;
	max30100_read(MAX30100_RegModeConfig, &config, 1);
	config = (config & 0xf8) | mode;
	max30100_write(MAX30100_RegModeConfig, &config, 1);
	max30100_clear_fifo();
}

//seta largura de pulso do led
void max30100_set_led_pulse_width(max30100_led_pw_t pw)
{
	uint8_t config;
	max30100_read(MAX30100_RegSPO2Config, &config, 1);
	config = (config & 0x7c) | (pw << MAX30100_LED_PW);
	max30100_write(MAX30100_RegSPO2Config, &config, 1);
}

//seta corrente tipica do led
void max30100_set_led_current(max30100_ledCurrent_t current)
{
	max30100_write(MAX30100_RegLedConfig, &current, 1);
}

//limpa todos os ponteiros da fifo
void max30100_clear_fifo()
{
	uint8_t val = 0x00;
	max30100_write(MAX30100_RegFifoWritePnt, &val, 3);
	max30100_write(MAX30100_RegFidoReadPnt, &val, 3);
	max30100_write(MAX30100_RegOverFlowCont, &val, 3);
}

//leitura da FIFO e armazena no max30100_t
void max30100_read_fifo()
{
	// pega Valor FifoWritePnt e FifoReadPnt
	uint8_t wr_ptr = 0, rd_ptr = 0;
	max30100_read(MAX30100_RegFifoWritePnt, &wr_ptr, 1);
	max30100_read(MAX30100_RegFidoReadPnt, &rd_ptr, 1);

	int8_t num_samples;

	num_samples = (int8_t)wr_ptr - (int8_t)rd_ptr;
	if (num_samples < 1)
	{
		num_samples += 16;
	}

	// le e imprime red_sample e ir_semple
	//Red_sample nao sera utilizado, logo nao foi colocada no plot
	for (int8_t i = 0; i < num_samples; i++)
	{
		uint8_t sample[4];
		max30100_read(MAX30100_RegFifoDataReg, sample, 4);
		uint16_t ir_sample = ((uint16_t)(sample[0] << 15) | (uint16_t)(sample[1] << 7)) & 0xFFFF;
		uint16_t red_sample = ((uint16_t)(sample[2] << 15) | (uint16_t)(sample[3] << 7)) & 0xFFFF;
		max30100.ir_samples[i] = ir_sample;
		max30100.red_samples[i] = red_sample;
		hard_get_sample_max30100(ir_sample);
	}
}

uint16_t max30100_convert_value(uint16_t ir_sample)
{
	// funçao de conversão do valor digital para BPM, o max tem como saída um valor de 16 bits
	//portanto é necessário fazer a conversão do valor digital para BPM, para isso é necessário
	//o procedimento de calibração do instrumento
	return ir_sample;
}

//Verifica se a flag de interrupcao esta ativa
uint8_t max30100_has_interrupt()
{
	return max30100.interrupt_flag;
}
