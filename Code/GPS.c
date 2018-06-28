#include "All_Function.h"

code *gpsDataGPVTG = "GPVTG";

void gps_init(void)
{
}

void gps_rxDataServer(void)
{
	static UINT8 rxDataStatus;
	static UINT8 temp0;
	static UINT8 temp1;
	static UINT8 Uart1RxData[10];
	
	UINT8 temp_RxData;
	
	if(RI_1)
	{
		
		temp_RxData = SBUF_1;
		RI_1 = 0;
		
		switch(rxDataStatus)
		{
			case 0:
				if(temp_RxData == '$')
				{
					rxDataStatus++;
					temp0 = 0;
				}
				break;
			
			
			case 1:
				Uart1RxData[temp0++] = temp_RxData;		//接收GPS特征字
				
				if(temp0 == 5)							//接收完GPS特征字
				{
					for(temp1 = 0 ; temp1 < temp0 ; temp1++)
					{
						if(Uart1RxData[temp1] == gpsDataGPVTG[temp1])		//比较是否需要的GPS特征字
							continue;
						else
						{
							rxDataStatus = 0;			//不是需要的特征字，退出
							break;
						}
					}
					
					if(temp1 == 5)				//符合GPS首部特征字
					{
						temp0 = 0;
						temp1 = 0;
						rxDataStatus++;
					}
				}
				
				break;
				
				
			case 2:							//筛选GPS速度所在字节
				if(temp_RxData == ',')
					temp0++;
				
				if(temp0 == 7)				//GPS速度所在位置
				{
					temp0 = 0;
					temp1 = 0;
					rxDataStatus++;
				}
				break;
				
			case 3:							//截取速度并转化存储
				if(temp_RxData == ',')
				{
					if(temp0 == 0)			//没有测到速度
					{
						temp0 = 0;
						temp1 = 0;
						rxDataStatus = 0;					
					}
					else
					{
					
					
					for(temp1 = 0 ; temp1 < temp0 ; temp1++);
//						????				//处理速度 justin
					}
				}
				else
				{
					Uart1RxData[temp0++] = temp_RxData;		//接收速度字节
				}
				break;
				
			default:
				break;
		}
		
	}
}

UINT8 get_gpsSpeed(void)
{
	return 80;
}

void gps_server(void)
{
	
}
