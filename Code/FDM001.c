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
	UINT8 spi_rec_data[16];
	UINT8	q_data;
	UINT8 q_flag;
	UINT8	spi_index;
	
	test_temp = 50;
	sys_init();

	spi_Read(0x00000000 , 16 , spi_rec_data);
	
//	spi_ReadInQ(0x00000000 , 32);
	while(get_spiReadState());
	
	for(test_temp = 0 ; test_temp < 16 ; test_temp++)
		printf("\n\n%01x , " , spi_rec_data[test_temp]);
	
	spi_index = 1;
	
	while(1)
	{
//		P12 = ~P12;
		if(!--test_temp)
		{
			Send_Data_To_UART0(0xaa);
//			Send_Data_To_UART1(0x55);
//			q_flag = q_pop(&q_data);
//			printf("\n\n%x , " , q_data);
//			
//			if(q_flag < 10)
//			{
//				spi_ReadInQ(0x00000000+spi_index*32 , 32);
//				spi_index++;
//			}
			
			test_temp = 5;
			
		}
		while(!flag_10ms);
		flag_10ms = FLASE;
	}
}

