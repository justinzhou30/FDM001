#include "All_Function.h"

void sys_init(void)
{
	Set_All_GPIO_Quasi_Mode;

//	MODIFY_HIRC_VLAUE();
//	CKDIV = 0x00;
//	P11_PushPull_Mode;
//	MODIFY_HIRC_VLAUE();
//	CKDIV = 0x50;					//HIRC devider 160
//	set_CLOEN;						//Check HIRC out wavefrom to confirm the HIRC modified
	
	P05 = 0;
	//P16 = 0;
	P14 = 0;
	P12 = 1;
	P11 = 0;

	Timer0_init();
	
	Uart0_init();
	
	Uart1_init();
	
	pwm_init();
	
	Spi_init();
	
	q_init();
	
	face_init();
	
	voice_init();
	
	key_init();
	
	gps_init();
	
	P0M2 |= 0x18;

	P1M2 |= 0x08;

	P03 = 1;
	
	set_EA;
}

