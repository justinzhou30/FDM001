//#include <N76E003.H>
#include "All_Function.h"

extern UINT8 systick_flag;

UINT8 systick_callbak;
UINT8 systick_step;

void check_systic_10ms(void)
{
	static unsigned int systick_time;
	static unsigned int systick_time0;
	
	if(++systick_time > 1000)
	{
		systick_time = 0;
		
		if(systick_flag)
		{
			systick_flag = 0;
			systick_callbak = 0;
		}
		else
		{
			systick_callbak = 1;
			systick_step = 0;
		}
	}
	
	if(systick_callbak)
	{
		switch(systick_step)
		{
			case 0:
				P03 = 0;
				systick_time0 = 0;
				systick_step++;
				// �ص�
				break;
			
			case 1:
				if(++systick_time0 > 100)
					systick_step++;
				//��ʱ
				break;
			
			case 2:
				P03 = 1;
				systick_time0 = 0;
				systick_step++;
				// �ϵ�
				break;
			
			case 3:
				if(++systick_time0 > 600)
					systick_step++;
				// ��ʱ
				break;
				
			case 4:
				Uart0_init();
				face_txCommand(FACE_COMMAND_BOUNDRATE);
				Uart0_init9600();
				//face_txCommand(FACE_COMMAND_OPEN);
			
			  systick_time0 = 0;
				systick_step++;
				// ���ͼ����Ϣ
				break;
			
			case 5:
				if(++systick_time0 > 50)
					systick_step++;
				//��ʱ
				break;
			
			case 6:
				face_txCommand(FACE_COMMAND_OPEN);
				//fati_setWarringState(FATI_WARRING_OPEN);
				systick_step++;
				break;
			
			default:
				break;
		}
	}
}
