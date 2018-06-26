#include "All_Function.h"

UINT8 code FACE_SETTING[]={0xFB,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x02,0x00,0x00,0x03,0x00,0x00,0x00,0x64,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x63};
UINT8 code FACE_OPEN[]={0xFB,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x01,0x02,0x00,0x00,0xF1};
UINT8 code FACE_CLOSE[]={0xFB,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x02,0x02,0x00,0x00,0xF0};
UINT8 code FACE_POSITION[]={0xFB,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x02,0x04,0x00,0x00,0xEE};



UINT8 face_recev0[32];
UINT8	face_recev0Index;
UINT8	face_recev1[32];
UINT8	face_recev1Index;


UINT8 *pFace_recevData;
UINT8 *pFace_dealData;

#define RECEV_INDEX 0x01
#define RECEV_COMPLE_MASK	0x80
UINT8	face_recev_stat;

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

void face_receData(void)
{
	
}

void face_server(void)
{
	UINT8 temp_index;
	UINT8 temp_checksum;
	UINT8 temp;
	
	if(face_recev_stat &= RECEV_COMPLE_MASK)
	{
		face_recev_stat &= ~RECEV_COMPLE_MASK;
		
		temp_index = *(pFace_dealData+1);
		
		 temp_checksum = 0;
		
		for(temp = 0 ; temp < temp_index ; temp_index++)
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
							break;
						case 0x02:
							break;
						case 0x03:
							break;
						case 0x04:
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

void face_isr_server(void)
{
	static UINT8 temp_len;
	UINT8 temp_data;
	
	temp_data = SBUF;
	
	if(temp_data == 0xfb)
		face_recev0Index = 0;
	
	if(face_recev0Index == 1)
		temp_len = temp_data;
	
	*(pFace_recevData+face_recev0Index) = temp_data;
	face_recev0Index++;
	
	if(temp_len == face_recev0Index)
	{
		if(face_recev_stat & RECEV_INDEX)
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
