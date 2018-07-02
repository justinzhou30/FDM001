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
	static UINT8 timeCount;
	
	UINT8 temp;
	
	sys_init();

	//play_voice(0);
	while(1)
	{
		if(timeCount++ > 200)		//2秒一次
		{
			timeCount = 0;
			temp = get_gpsSpeed();
			
			if(temp == 0xff)		//GPS没有信号
			{
				face_txCommandSpeed(65);
			}
			else
			{
				face_txCommandSpeed(temp);
			}
		}
		
		key_scan_10ms();
		gps_Server_10ms();
		
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

