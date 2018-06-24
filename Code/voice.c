#include "All_Function.h"

#define VOICE_DATA_SIZE_OFFSET		0x2a
#define VOICE_DATA_OFFSET					0x2e

UINT32 voiceAddrFlash;			//声音在flash里面的地址
UINT32 voiceDataSize;			//声音数据的大小
UINT32 voiceDataIndex;			//flash里面取的当前声音数据指针


//UINT8 voiceBuffer0[32];

#define VOICE_BUFFER_INDEX_MAX		16

UINT8 voiceBufferItem;			//指示用的是哪一个Buffer 0和1

UINT32 get_addrFlash(UINT8 index)		//根据索引取得当前声音在flash里面的地址
{
	return 0;
}

void play_voice(UINT8 index)
{
	UINT8 temp[4];
	
	stop_pwm();

	voiceAddrFlash = get_addrFlash(index);
	while(get_spiReadState());
	spi_Read(voiceAddrFlash+VOICE_DATA_SIZE_OFFSET , 4 , &temp);	
	
	voiceDataIndex = 0;
	
	while(get_spiReadState());
	voiceDataSize = 0;
	voiceDataSize |= temp[3];
	voiceDataSize <<= 8;
	voiceDataSize |= temp[2];
	voiceDataSize <<= 8;
	voiceDataSize |= temp[1];	
	voiceDataSize <<= 8;
	voiceDataSize |= temp[0];
	
	spi_ReadInQ(voiceAddrFlash+VOICE_DATA_OFFSET , 32);
	while(get_spiReadState());
	voiceDataIndex += 32;
	
	q_pop(&temp[0]);
	q_pop(&temp[1]);
	
	set_pwmDuty(temp);
	
	start_pwm();
}


void getVoiceNextData(void)
{
	UINT8 pwmDutyData[2];
	
	if(q_get_dataSize() == 0)
	{
//		stop_pwm();
		return;
	}
	
	q_pop(&pwmDutyData[0]);
	q_pop(&pwmDutyData[1]);
	
	set_pwmDuty(pwmDutyData);
	
	if(q_get_dataSize() < VOICE_BUFFER_INDEX_MAX)
	{
		if(voiceDataSize == voiceDataIndex)
			return;
		
		if((voiceDataSize - voiceDataIndex) < 32)
		{
			spi_ReadInQ(voiceAddrFlash+VOICE_DATA_OFFSET+voiceDataIndex , voiceDataSize - voiceDataIndex);
			voiceDataIndex = voiceDataSize;
		}
		else
		{
			spi_ReadInQ(voiceAddrFlash+VOICE_DATA_OFFSET+voiceDataIndex , 32);
			voiceDataIndex += 32;
		}
	}
}
