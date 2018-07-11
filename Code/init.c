#include "All_Function.h"

void sys_init(void)
{
	Set_All_GPIO_Quasi_Mode;
	
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

<<<<<<< Updated upstream
=======
	P03 = 1;
	
>>>>>>> Stashed changes
	set_EA;
}

