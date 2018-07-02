#include "All_Function.h"

//UINT8 uart1RiFlag;

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
		q_push(SBUF_1);
//		uart1RiFlag = 0xff;
	}
}

char putchar (char c)  {
    TI_1 = 0;
    SBUF_1 = c;
    while(TI_1==0);
		return c;
}
