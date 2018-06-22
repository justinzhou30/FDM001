#include "All_Function.h"

void sys_init(void)
{
	Set_All_GPIO_Quasi_Mode;
	
	Timer0_init();
	
	Uart0_init();
	
	set_EA;
}

