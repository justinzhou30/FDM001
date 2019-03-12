#include "All_Function.h"


void Uart0_init(void)
{
	clr_ES;
	
	InitialUART0_Timer1(115200);
	
	set_ES;
}

void Uart0_init9600(void)
{
	clr_ES;
	
	InitialUART0_Timer1(9600);
	
	set_ES;
}

void Uart0_ISR(void) interrupt UART0_ISR
{
	if(TI)
	{
		face_isr_server_TI();
		TI = 0;
	}
	
	if(RI)
	{
		face_isr_server_RI();
		RI = 0;
	}
}
