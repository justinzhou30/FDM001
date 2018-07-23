#include "All_Function.h"

#define VOICE_DATA_SIZE_OFFSET		0x2a
#define VOICE_DATA_OFFSET					0x2e

UINT32 voiceAddrFlash;			//声音在flash里面的地址
UINT32 voiceDataSize;			//声音数据的大小
UINT32 voiceDataIndex;			//flash里面取的当前声音数据指针

#define VOICE_PLAY_STATE1		1
#define VOICE_PLAY_STATE2		2
#define VOICE_PLAY_STATE3		3
#define VOICE_PLAY_STATE4		4
UINT8 voicePlayState;		
//UINT8 voiceBuffer0[32];

#define VOICE_BUFFER_INDEX_MAX		16
UINT8 voiceIndexBuffer[16];			//语音播放缓冲
UINT8 voiceIndexF;								//指向语音播放缓冲
UINT8 voiceIndexB;							

#define VOICE_STATE_PLAY		1
#define VOICE_STATE_STOP		2
UINT8 voiceState;

UINT8 voiceBufferItem;			//指示用的是哪一个Buffer 0和1


// #define VOICE_INDEX_WELCOM	1
// #define VOICE_INDEX_FACIALREAD	2
// #define VOICE_INDEX_UNSUCCESSFUL	3
// #define VOICE_INDEX_SUCCESSFUL	4
// #define VOICE_INDEX_WATCHROAD	5
// #define VOICE_INDEX_CAREFULLY	6
// #define VOICE_INDEX_DANGER		7
// #define VOICE_INDEX_REST		8
// #define VOICE_INDEX_STOP		9
// #define VOICE_INDEX_BYE			10

//code UINT32 voice_flash_addr[] = {0,
//																	0x00000400,
//																	0x0000b0ce,
//																	0x0001928a,
//																	0x00024636,
//																	0x00029be2,
//																	0x0002f9d0,
//																	0x0003331e,
//																	0x00034c4c,
//																	0x0003d91a,
//																	0x00041a48};

static void voice_setPlayState(UINT8 stateData)
{
	voiceState = stateData;
}

static UINT8 voice_getPlayState(void)
{
	return voiceState;
}

void voice_init(void)
{
	for(voiceIndexF = 0 ; voiceIndexF < 8 ; voiceIndexF++)
		voiceIndexBuffer[voiceIndexF] = 0;
	
	voiceIndexB = 0;
	voiceIndexF = 0;
	voice_setPlayState(VOICE_STATE_STOP);
}


void voice_IC_close(void)
{
	// P03 = 0;
}

void voice_IC_open(void)
{
	// P03 = 1;
}

UINT32 get_addrFlash(UINT8 index)		//根据索引取得当前声音在flash里面的地址
{
	UINT8 temp[4];
	UINT32 temp32;
	
	if((index > 0) && (index < 12))
	{
		temp32 = 0;
		temp32 |= index;
		temp32 <<= 4;
		
		spi_ReadInit(temp32);
	
		temp[0] = spi_ReadNextByte();
		temp[1] = spi_ReadNextByte();
		temp[2] = spi_ReadNextByte();
		temp[3] = spi_ReadNextByte();
		spi_ReadStop();
		
		temp32 = 0;
		
		temp32 |= temp[3];
		temp32 <<= 8;
		temp32 |= temp[2];
		temp32 <<= 8;
		temp32 |= temp[1];
		temp32 <<= 8;
		temp32 |= temp[0];
		
//		putchar(temp[3]);
//		putchar(temp[2]);
//		putchar(temp[1]);
//		putchar(temp[0]);
		return temp32;
	}
	else
	{
		return 0x00;
	}
}

void play_voice(UINT8 index)
{
	if(((voiceIndexF+1)&0x0f) == voiceIndexB)
		return;													//播放列表满
	
	
	voiceIndexBuffer[voiceIndexF++] = index;
	voiceIndexF &= 0x0f;
}

void play_voiceBak(UINT8 index)
{
	UINT8 temp[4];
	
	voice_setPlayState(VOICE_STATE_PLAY);
	
	stop_pwm();

	voiceAddrFlash = get_addrFlash(index);
	
	spi_ReadInit(voiceAddrFlash+VOICE_DATA_SIZE_OFFSET);
	
	temp[0] = spi_ReadNextByte();
	temp[1] = spi_ReadNextByte();
	temp[2] = spi_ReadNextByte();
	temp[3] = spi_ReadNextByte();
	
	voiceDataSize = 0;
	
	voiceDataSize |= temp[3];
	voiceDataSize <<= 8;
	voiceDataSize |= temp[2];
	voiceDataSize <<= 8;
	voiceDataSize |= temp[1];
	voiceDataSize <<= 8;
	voiceDataSize |= temp[0];
	
	temp[0] = 0x00;
	temp[1] = 0x02;
	voiceDataIndex = 0;
//	
//	set_pwmDuty(temp);
	
	start_pwm();
		
		voicePlayState = VOICE_PLAY_STATE1;
}


void getVoiceNextData(void)
{
	UINT8 pwmDutyData[2];
	
	switch(voicePlayState)
	{
		case VOICE_PLAY_STATE1:
			pwmToMiddle();					//去爆破音
			break;
		
		case VOICE_PLAY_STATE2:		
			if(voiceDataIndex == voiceDataSize)
			{
				voicePlayState = VOICE_PLAY_STATE3;
//				voice_setPlayState(VOICE_STATE_STOP);
//				voice_IC_close();
//				spi_ReadStop();
//				stop_pwm();
			}
			else
			{
				voice_IC_open();
				pwmDutyData[0] = spi_ReadNextByte();
//				pwmDutyData[1] = spi_ReadNextByte();
				voiceDataIndex++;
				set_pwmDuty(pwmDutyData);
			}
			break;
			
		case VOICE_PLAY_STATE3:
			if((PWM3L == 0x01) && (PWM3H == 0x00))
				voicePlayState = VOICE_PLAY_STATE4;
			else
			{
				if( --PWM3L == 0xff)
					--PWM3H;
					
			}
			set_LOAD;			
			break;
		
		case VOICE_PLAY_STATE4:
			voice_setPlayState(VOICE_STATE_STOP);
			voice_IC_close();
			spi_ReadStop();
			stop_pwm();			
			break;
		
		default:
			break;
	}
}



void voice_server(void)
{
	if(voiceIndexF == voiceIndexB)
		return;									//播放列表空
	
	if(voice_getPlayState() == VOICE_STATE_STOP)
	{
		play_voiceBak(voiceIndexBuffer[voiceIndexB++]);
		voiceIndexB &= 0x0f;
	}
}

