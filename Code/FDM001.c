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

	Timer2_Delay500us(3000);
	
	
	face_txCommand(FACE_COMMAND_OPEN);
	play_voice(VOICE_INDEX_WELCOM);
	
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
		face_server_10ms();
		openCloseServer_10ms();
		
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

