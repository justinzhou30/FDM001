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
//  UINT8 test_temp;
//	UINT8 spi_rec_data[16];
//	UINT8	q_data;
//	UINT8 q_flag;
//	UINT8	spi_index;
	
//	test_temp = 50;
	sys_init();

//	spi_Read(0x00000000 , 16 , spi_rec_data);

//	spi_ReadInQ(0x00000000 , 32);

//	while(get_spiReadState());

////	
////	for(test_temp = 0 ; test_temp < 16 ; test_temp++)
////		printf("\n\n%01x , " , spi_rec_data[test_temp]);
//	

//	spi_index = 1;
play_voice(0);
	while(1)
	{
//		P12 = ~P12;
//		if(!--test_temp)
//		{
//			static UINT8 temp00;
//			face_txCommand(temp00);
//			if(++temp00 > 3)
//				temp00 = 0;
//			Send_Data_To_UART0(0xaa);
//			Send_Data_To_UART1(0x55);
//			q_flag = q_pop(&q_data);
//			Send_Data_To_UART0(q_data);
////			printf("\n\n%c , " , q_data);
//			
//			if(q_flag < 20)
//			{
//				spi_ReadInQ(0x00000000+spi_index*32 , 32);
//				spi_index++;
//			}
//			
			
//			test_temp =200;
			
//		}
		
		key_scan_10ms();
		
		while(!flag_10ms)
		{
			pwm_server();
			spi_server();
			q_server();
			face_server();
			key_server();
			voice_server();
			gps_server();
		}
		flag_10ms = FLASE;
	}
}

