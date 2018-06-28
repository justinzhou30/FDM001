#include "All_Function.h"

#define KEY_INPUT	P17
#define KEY_PRESSTIME	100
#define KEY_DEBANCE		2


#define SHORT_PRESS 1
#define LONG_PRESS	2
UINT8 key_pressStatus;

#define SYS_OPEN	1
#define SYS_CLOSE	0
UINT8 sys_status;

void key_init(void)
{
	KEY_INPUT = 1;
}

void key_scan_10ms(void)
{
	static UINT8 key_scanTimer;
	static UINT8 key_runStatus;
	
	if(KEY_INPUT == 1)
	{
		if((key_scanTimer > KEY_DEBANCE) && (key_scanTimer < (KEY_PRESSTIME+1)))		//短按以及按键去抖
			key_pressStatus = SHORT_PRESS;
		
		key_runStatus = 0;
		key_scanTimer = 0;
		return;
	}
	
	switch(key_runStatus)
	{
		case 0:
			if(++key_scanTimer > KEY_DEBANCE)			//按键去抖
				key_runStatus = 1;
			break;
			
		case 1:
			++key_scanTimer;
			
			if(key_scanTimer == KEY_PRESSTIME)
				key_pressStatus = LONG_PRESS;
			
			if(key_scanTimer > KEY_PRESSTIME)
				key_scanTimer = KEY_PRESSTIME+5;
			break;
			
		default:
			break;	
	}
		
}

void key_server(void)
{
	if(key_pressStatus)
	{
		if(key_pressStatus == SHORT_PRESS)
		{
			if(sys_status == SYS_OPEN)
			{
				play_voice(VOICE_INDEX_BYE);
				
			}
			
			if(sys_status == SYS_CLOSE)
			{
				play_voice(VOICE_INDEX_WELCOM);
			}
		}
		
		if(key_pressStatus == LONG_PRESS)
		{
			play_voice(VOICE_INDEX_FACIALREAD);
		}
		
		key_pressStatus = 0;
	}
}

