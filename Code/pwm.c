#include "All_Function.h"

void pwm_init(void)
{
	PWM3_P04_OUTPUT_ENABLE;
	PWM_CLOCK_FSYS;
	PWM_INT_PWM3;
	PWM_PERIOD_END_INT;
//	PWM_CLOCK_DIV_2;
	PWMPH = 16000000/16000/256;
	PWMPL = (16000000/16000)%256;
	
	PWM3H = 0x01;
	PWM3L = 0xf4;
	
	set_EPWM;
	set_LOAD;
	set_PWMRUN;
}

void Pwm_ISR (void) interrupt PWM_ISR
{
	static UINT16 temp;
  clr_PWMF;               // clear PWM interrupt flag

	if(++PWM3L == 0xe7)
	{
		PWM3L = 1;
		if(++PWM3H == 4)
			PWM3H = 0;

	}
	set_LOAD;
	if(++temp == 16000)
	{
		Send_Data_To_UART0(0x34);
		temp = 0;
	}
}