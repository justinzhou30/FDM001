//***********************************************************************************************************
//  File Function: N76E003 Timer0/1 Mode1 demo code
//***********************************************************************************************************

#include "All_Function.h"

/************************************************************************************************************
*    Main function 
************************************************************************************************************/
UINT8 flag_10ms;


void main (void)
{
  UINT8 test_temp;
	test_temp = 100;
	sys_init();


	while(1)
	{
//		P12 = ~P12;
		if(!--test_temp)
		{
//			Send_Data_To_UART0(0xaa);
//			Send_Data_To_UART1(0x55);
			test_temp = 100;
		}
		while(!flag_10ms);
		flag_10ms = FLASE;
	}
}

