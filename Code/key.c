#include "All_Function.h"

#define KEY_INPUT	P17
#define KEY_PRESSTIME	220
#define KEY_DEBANCE		5


#define SHORT_PRESS 1
#define LONG_PRESS	2
UINT8 key_pressStatus;

#define SYS_OPEN	1
#define SYS_CLOSE	0
#define SYS_CHANGE	0x80
UINT8 sys_status;

UINT8 sys_close_flag;
extern UINT8 fatiFacePosition;

void key_init(void)
{
	KEY_INPUT = 1;
	sys_status |= SYS_OPEN;
}

void key_scan_10ms(void)
{
	static UINT8 key_scanTimer;
	static UINT8 key_runStatus;
	
	if(KEY_INPUT == 1)
	{
		if((key_scanTimer > KEY_DEBANCE) && (key_scanTimer < (KEY_PRESSTIME+1)))		//短按以及按键去抖
		{
//			putchar(0);
			key_pressStatus = SHORT_PRESS;
		}
//		putchar(0);
		key_runStatus = 0;
		key_scanTimer = 0;
		return;
	}
	else
	{
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
}

void key_server(void)
{
	if(key_pressStatus)
	{
		if(key_pressStatus == SHORT_PRESS)
		{
			if(sys_status == SYS_OPEN)
			{
//				putchar(0);
				sys_status = SYS_CLOSE + SYS_CHANGE;
//				play_voice(VOICE_INDEX_BYE);
				
			}
			
			if(sys_status == SYS_CLOSE)
			{
//				putchar(1);
				sys_status = SYS_OPEN + SYS_CHANGE;
				play_voice(VOICE_INDEX_WELCOM);
			}
		}
		
		if((key_pressStatus == LONG_PRESS) && (sys_status == SYS_OPEN))
		{
			fatiFacePosition = 0xff;
			P13 = 1;
//			face_txCommand(FACE_COMMAND_POSITION);
			play_voice(VOICE_INDEX_FACIALREAD);
		}
		
		key_pressStatus = 0;
	}
}

void openCloseServer_10ms(void)
{
	static UINT8 runState;
	static UINT8 temp;
	
	if(sys_status & SYS_CHANGE)
	{
		sys_status &= ~SYS_CHANGE;
		runState = 1;
	}
		
	
	switch(runState)
	{
		case 0:
			break;
			
		case 1:
			if(sys_status & SYS_OPEN)
				runState = 2;
			else
				runState = 3;
		
			break;
			
		case 2:					//开机
//			putchar(6);
			SW_Reset0();			//软件复位
			break;
			
		case 3:					//关机
//			putchar(8);
		  sys_close_flag = 0xaa;
			voice_init();
			play_voice(VOICE_INDEX_BYE);
			face_txCommand(FACE_COMMAND_CLOSE);
			runState = 4;
			break;
			
		case 4:				//延时
			if(temp++ > 251)
			{
				temp = 0;
//				putchar(10);
				P03 = 0;
				P12 = 0;
				P13 = 0;
				P14 = 0;
				P11 = 0;
				runState = 0;
			}
			break;
			
		default:
			runState = 0;
			break;
	}
}


