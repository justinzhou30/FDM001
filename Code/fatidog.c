#include "All_Function.h"

UINT8 code FACE_SETTING[]={0xFB,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x02,0x00,0x00,0x03,0x00,0x00,0x00,0x64,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x63};
UINT8 code FACE_OPEN[]={0xFB,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x01,0x02,0x00,0x00,0xF1};
UINT8 code FACE_CLOSE[]={0xFB,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x02,0x02,0x00,0x00,0xF0};
UINT8 code FACE_POSITION[]={0xFB,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x02,0x04,0x00,0x00,0xEE};


UINT8 face_recev0[32];
UINT8	face_recev0Index;
UINT8	face_recev1[32];
UINT8	face_recev1Index;

UINT8	face_TxCommandSpeed[64];	//暂存需要发送的命令，用来计算checksum

UINT8 *pFace_recevData;			//接收数据的缓冲区
UINT8 *pFace_dealData;			//处理数据的缓冲区

#define RECEV_INDEX 0x01
#define RECEV_COMPLE_MASK	0x80
UINT8	face_recev_stat;		//接收数据的状态


UINT8	face_TxIndex;			//发送数据的指针偏移
UINT8 *pFace_TxData;			//发送数据的缓冲区

#define TX_WORKING	0x01
#define TX_COMPLE		0x80
UINT8 face_tx_stat;				//发送数据的状态

#define FATI_STYLE_OFFSET	9


#define FATI_STYLE_FACE		0x01
#define FATI_STYLE_TIRED	0x02
#define FATI_STYLE_SIGN		0x03
#define FATI_STYLE_SYS		0x09
#define FATI_STYLE_ACK		0x12
void face_init(void)
{
	pFace_recevData = face_recev0;
	pFace_dealData = face_recev1;
	face_recev_stat = 0;
}

void face_txCommandSpeed(UINT8 speed)
{
	UINT8 temp;
	UINT8 temp2;
	
	for(temp = 0 ; temp < FACE_SETTING[1] ; temp++)
		face_TxCommandSpeed[temp] = FACE_SETTING[temp];
	
	face_TxCommandSpeed[14] = 0x01;		//设置速度标志
	face_TxCommandSpeed[18] = speed;	//速度
	
	temp2 = 0;
	
	for(temp = 0 ; temp < FACE_SETTING[1]-1 ; temp++)
		temp2 += face_TxCommandSpeed[temp];
	
	temp2 = ~temp2;						//checksum 必须为0
	++temp2;
	
	face_TxCommandSpeed[temp] = temp2;
	
	pFace_TxData = face_TxCommandSpeed;
	face_TxIndex = 0;
	
	SBUF = *(pFace_TxData+face_TxIndex);
}

void face_txCommand(UINT8 face_command)
{
	UINT8 code *pFace_TxDataTemp[] = {FACE_OPEN,FACE_CLOSE,FACE_POSITION,FACE_SETTING};
	
	if(face_command > 3)
		return;
	
	pFace_TxData = pFace_TxDataTemp[face_command];
	
	face_tx_stat = TX_WORKING;
	face_TxIndex = 0;
	
	SBUF = *(pFace_TxData+face_TxIndex);
}

void face_server(void)
{
	UINT8 temp_index;
	UINT8 temp_checksum;
	UINT8 temp;
	
	if(face_recev_stat & RECEV_COMPLE_MASK)				//处理从fati接收的数据
	{
		face_recev_stat &= ~RECEV_COMPLE_MASK;
		
		temp_index = *(pFace_dealData+1);
		
		 temp_checksum = 0;
		
		for(temp = 0 ; temp < temp_index ; temp++)
			temp_checksum += *(pFace_dealData+temp);					//checksum
		
		if(temp_checksum)				//data error
			return;
		
		switch(*(pFace_dealData+FATI_STYLE_OFFSET))
		{
			case FATI_STYLE_FACE:
				break;
			
			case FATI_STYLE_TIRED:
				if(*(pFace_dealData+10) == 0x06)
				{
					switch(*(pFace_dealData+11))
					{
						case 0x01:
							play_voice(VOICE_INDEX_CAREFULLY);
							break;
						case 0x02:
							play_voice(VOICE_INDEX_WATCHROAD);
							break;
						case 0x03:
							play_voice(VOICE_INDEX_DANGER);
							break;
						case 0x04:
							play_voice(VOICE_INDEX_REST);
							break;
						
						default:
							break;
					}
				}
				break;
			
			case FATI_STYLE_SIGN:
				break;
			
			case FATI_STYLE_SYS:
				break;
			
			case FATI_STYLE_ACK:
				break;
			
			default:
				break;
		}
	}
}

void face_isr_server_TI(void)
{
	static UINT8 temp_len;
	
	if(face_tx_stat == TX_WORKING)
	{
		if(++face_TxIndex == 1)
			temp_len = *(pFace_TxData+face_TxIndex);		//发送数据包长度
		
		if(face_TxIndex > (temp_len-1))
		{
			face_tx_stat = TX_COMPLE;
			return;
		}
		
		SBUF = *(pFace_TxData + face_TxIndex);
	}
}

void face_isr_server_RI(void)
{
	static UINT8 temp_len;
	UINT8 temp_data;
	
	temp_data = SBUF;
	
	if(temp_data == 0xfb)
		face_recev0Index = 0;
	
	if(face_recev0Index == 1)
		temp_len = temp_data;			//fati 的数据长度
	
	*(pFace_recevData+face_recev0Index) = temp_data;
	face_recev0Index++;
	
	if(temp_len == face_recev0Index)
	{
		if(face_recev_stat & RECEV_INDEX)			//接收缓冲区有两个，交替接收工作
		{
			face_recev_stat &= ~RECEV_INDEX;		
			pFace_recevData = face_recev0;
			pFace_dealData = face_recev1;
		}
		else
		{
			face_recev_stat |= RECEV_INDEX;
			pFace_recevData = face_recev1;
			pFace_dealData = face_recev0;
		}
		
		face_recev_stat |= RECEV_COMPLE_MASK;
	}
	
//	RI = 0;
}
