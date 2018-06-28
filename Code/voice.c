#include "All_Function.h"

#define VOICE_DATA_SIZE_OFFSET		0x2a
#define VOICE_DATA_OFFSET					0x2e

UINT32 voiceAddrFlash;			//声音在flash里面的地址
UINT32 voiceDataSize;			//声音数据的大小
UINT32 voiceDataIndex;			//flash里面取的当前声音数据指针

#define VOICE_PLAY_STATE1		1
#define VOICE_PLAY_STATE2		2
UINT8 voicePlayState;		
//UINT8 voiceBuffer0[32];

#define VOICE_BUFFER_INDEX_MAX		16

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

code UINT32 voice_flash_addr[] = {0,0,0,0,0,0,0,0,0,0};


void voice_init(void)
{
	
}

UINT32 get_addrFlash(UINT8 index)		//根据索引取得当前声音在flash里面的地址
{
	return voice_flash_addr[index];
}

void play_voice(UINT8 index)
{
	UINT8 temp[4];
	
	stop_pwm();

	voiceAddrFlash = get_addrFlash(index);
	
	spi_ReadInit(voiceAddrFlash+VOICE_DATA_SIZE_OFFSET);
	
	voiceDataSize = 0;
	
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
	
	temp[0] = 0x01;
	temp[1] = 0x00;
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
			pwmToMiddle();
			break;
		
		case VOICE_PLAY_STATE2:		
			if(voiceDataIndex == voiceDataSize)
			{
				stop_pwm();
			}
			else
			{
				pwmDutyData[0] = spi_ReadNextByte();
				pwmDutyData[1] = spi_ReadNextByte();
				voiceDataIndex += 2;
				set_pwmDuty(pwmDutyData);
			}
			break;
			
		default:
			break;
	}
}

void voice_server(void)
{
	
}

