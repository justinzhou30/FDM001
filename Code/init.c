#include "All_Function.h"

void sys_init(void)
{
	Set_All_GPIO_Quasi_Mode;
	
	Timer0_init();
	
	Uart0_init();
	
	Uart1_init();
	
	pwm_init();
	
	Spi_init();
	
	set_EA;
}
