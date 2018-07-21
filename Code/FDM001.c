//***********************************************************************************************************
//  File Function: N76E003 Timer0/1 Mode1 demo code
//***********************************************************************************************************

#include "All_Function.h"

extern UINT8 fatiFacePosition;
/************************************************************************************************************
*    Main function 
************************************************************************************************************/
UINT8 flag_10ms;


void main (void)
{
	static UINT16 timeCount;
	static UINT8 timeCount30s;
//	static alarmFlag;
	
	UINT8 temp;
	
	sys_init();

	Timer2_Delay500us(1000);		//dealy 0.5s


	play_voice(VOICE_INDEX_WELCOM);
	//play_voice(VOICE_INDEX_FACIALREAD);

	while(1)
	{
		if(++timeCount > 600)			//delay  等face模块上电初始化完成
		{
			timeCount = 0;
			break;
		}

		while(!flag_10ms)
		{
			pwm_server();
			spi_server();
		}
		flag_10ms = FLASE;
	}
	
	face_txCommand(FACE_COMMAND_BOUNDRATE);	//修改视频模块波特率
	while(1)
		{
			if(++timeCount > 100) 		//delay  等face模块上电初始化完成
			{
				timeCount = 0;
				break;
			}
	
			while(!flag_10ms);
			flag_10ms = FLASE;
		}
	
	Uart0_init9600();
	face_txCommand(FACE_COMMAND_OPEN);

		
	while(1)
	{
		if((timeCount++ > 200) && (fatiFacePosition == 0))		//2秒一次 //长按按键
		{
			timeCount = 0;
			
			temp = get_gpsSpeed();
			
			if(temp == 0xff)		//GPS没有信号
			{
				face_closeAlarm();
			}
			else
			{
				if(temp > 4)		//speed > 5km
				{
					timeCount30s = 0;
					face_openAlarm();
				}
				else
				{
					if(++timeCount30s > 15)
					{
						timeCount30s = 16;
						face_closeAlarm();
					}
				}
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

