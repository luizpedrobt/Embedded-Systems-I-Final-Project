#include <hard.h>

uint16_t arr = 0;

void hard_init()
{
	ssd1306_Init();
	max30100_init();
	hard_max30100_start_interrupt();
	hard_buzzer_pwm_start();

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &MAX30100_TIM_PORT)
	{
		max30100_on_interrupt();
		max30100_interrupt_handler();
	}
}

/*Faz a média de 40 leituras do max30100 e envia para o display, display muda a leitura a cada ~1s*/
void hard_get_sample_max30100(uint16_t ir_sample)
{
	static int sample = 1;
	static uint32_t soma = 0;

	if (sample == 40){
		/*Converte o valor digital para bpm, função generica*/
		uint16_t valor_convertido;
		valor_convertido = max30100_convert_value(ir_sample);
		valor_convertido = floor(valor_convertido);

		char batimento[10];
		/*Converte uint16_t para string*/
		sprintf(batimento, "%u", valor_convertido);

		/*Limpa display, seta cursor e escreve*/
		ssd1306_FillRectangle(0, 0, 63, 63, Black);
		ssd1306_SetCursor(10, 10);
		ssd1306_WriteString(batimento, Font_11x18, White);

		/*Conversão levando em consideração batimentos entre 50bpm e 220bpm (condição normal)
		  Frequência pós pré-escalar = 2.5kHz (PSC = 12799, definido em Projeto_Final_SEMB.ioc -> Timers -> TIM2)
		  Pulso do buzzzer constante em 0.1s (CCR = 249) indenpendente da frequência de batimentos*/
		arr = floor((60/(valor_convertido*0.0004))-1);

		/*Seta novo valor de arr e zera o contador*/
		hard_buzzer_tim_set_arr(arr);
		hard_buzzer_tim_set_counter(0);

		sample = 0;
		soma = 0;
	}

	/*Média de 40 leituras do max30100*/
	soma += (ir_sample / 40 );
	if(sample > 40)
	{
		sample = 0;
		soma = 0;
	}
	sample++;
}

void hard_max30100_start_interrupt(void)
{
	/*Inicia timer para amostragem constante em 27ms -> Fs (36.7Hz) > 2Fmáx (2*3.67Hz) (Respeitando teorema de Nyquist) usou-se 10 vezes maior
	  PSC = 15999 e ARR = 53, definido em Projeto_Final_SEMB.ioc -> Timers -> TIM3*/
	HAL_TIM_Base_Start_IT(&MAX30100_TIM_PORT);
}

void hard_buzzer_pwm_start(void)
{
	hard_buzzer_tim_set_arr(12799); /*Frequência de 2.5kHz pós pre-escalar (PSC) = 12799*/
	hard_buzzer_tim_set_ccr(249); /*Pulso de som do buzzer com duração de 0.1s*/
	hard_buzzer_tim_set_counter(0);/*Garante contador iniciar em 0*/
	HAL_TIM_PWM_Init(&BUZZER_TIM_PORT);
	HAL_TIM_PWM_Start(&BUZZER_TIM_PORT, TIM_CHANNEL_1);
}

void hard_buzzer_tim_set_counter(uint16_t cnt)
{
	__HAL_TIM_SET_COUNTER(&BUZZER_TIM_PORT, cnt);
}

void hard_buzzer_tim_set_arr(uint16_t cnt)
{
	__HAL_TIM_SET_AUTORELOAD(&BUZZER_TIM_PORT, cnt);
}

void hard_buzzer_tim_set_ccr(uint16_t cnt)
{
	__HAL_TIM_SET_COMPARE(&BUZZER_TIM_PORT, TIM_CHANNEL_1, cnt);
}

/*Ativa modo de espera até uma interrupção acontecer*/
void hard_cpu_sleep(void)
{
	__WFI();
}

void hard_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout){
	HAL_I2C_Master_Transmit(hi2c, DevAddress, pData, Size, Timeout);
}

void hard_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout){
	HAL_I2C_Master_Receive(hi2c, DevAddress, pData, Size, Timeout);
}

void hard_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout){
	HAL_I2C_Mem_Write(hi2c, DevAddress, MemAddress, MemAddSize, pData, Size, Timeout);
}

