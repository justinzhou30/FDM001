#include "All_Function.h"

UINT8 pwm_intFlag;

#define VOICE_PLAY_STATE2		2
extern UINT8 voicePlayState;

void pwm_init(void)
{
	PWM3_P04_OUTPUT_ENABLE;
	PWM_CLOCK_FSYS;
	PWM_INT_PWM3;
	PWM_RISING_INT;
//	PWM_CLOCK_DIV_2;
	PWMPH = 16000000/15625/256;					//16000000 / 1024 = 15625
	PWMPL = (16000000/15625)%256;
	
	PWM3H = 0x01;
	PWM3L = 0xff;
	
	clr_EPWM;
	clr_LOAD;
	clr_PWMRUN;
}

void Pwm_ISR (void) interrupt PWM_ISR
{
//	static UINT16 temp;
  clr_PWMF;               // clear PWM interrupt flag

//	if(++PWM3L == 0xe7)
//	{
//		PWM3L = 1;
//		if(++PWM3H == 4)
//			PWM3H = 0;

//	}
//	set_LOAD;
//	if(++temp == 16000)
//	{
//		Send_Data_To_UART0(0x34);
//		temp = 0;
//	}
//	P12 = ~P12;
	pwm_intFlag = 0xff;
//	getVoiceNextData();
}

void start_pwm(void)
{
//	printf("\nPWM_start");
//	return;
	PWM3H = 0x00;
	PWM3L = 0x01;
	
	set_EPWM;
	set_LOAD;
	set_PWMRUN;
}

void stop_pwm(void)
{
	clr_EPWM;
	clr_PWMRUN;
	clr_PWMF;
//	printf("\nPWM_STOP");
}

void set_pwmDuty(UINT8 *pBuffer)
{
	UINT16 temp;
	UINT16 temp2;
	UINT8		temp_flag;
	
	temp = 0;
	temp |= *(pBuffer+1);
	temp <<= 8;
	temp |= *pBuffer;
	
	if(temp & 0x8000)
	{
		temp = ~temp;
		temp++;
		temp_flag = 0xff;
	}
	
	
	temp >>= 5;						//temp = temp/32			
	
	if(temp_flag)
		temp2 = 512 - temp;
	else
		temp2 = 512 + temp;
	
	
//	temp >>= 3;						//???????
	if(temp2 > 1023)
		temp2 = 1000;
	
	if(temp2 == 0)
		temp2 = 25;
	
	PWM3L = (UINT8)temp2;
	PWM3H = (UINT8)(temp2>>8);
	
	set_LOAD;
}

void pwmToMiddle(void)
{
	if((PWM3L == 0x00) && (PWM3H == 0x02))
		voicePlayState = VOICE_PLAY_STATE2;
	else
	{
		if( ++PWM3L == 0)
			++PWM3H;
	}
	set_LOAD;
}

void pwm_server(void)
{
	if(pwm_intFlag)
	{
		pwm_intFlag = 0;
		getVoiceNextData();
	}
}
