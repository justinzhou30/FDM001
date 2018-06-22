#include "All_Function.h"

void Uart1_init(void)
{
	InitialUART1_Timer3(9600);
	
	set_ES_1;
}

void Uart1_ISR(void) interrupt UART1_ISR
{
	if(TI_1)
	{
		TI_1 = 0;
	}
	
	if(RI_1)
	{
		RI_1 = 0;
	}
}