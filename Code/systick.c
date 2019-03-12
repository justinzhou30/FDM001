//#include <N76E003.H>
#include "All_Function.h"

extern UINT8 systick_flag;

UINT8 systick_callbak;
UINT8 systick_step;

extern UINT8 sys_close_flag;

void check_systic_10ms(void)
{
	static unsigned int systick_time;
	static unsigned int systick_time0;
	
	if(++systick_time > 2500)
	{
		systick_time = 0;
		
//		if(sys_status & 1/*SYS_OPEN*/)
//			systick_flag = 1;
		if(sys_close_flag == 0xaa)			// 判断是否关机。
			systick_flag = 1;
		
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
				Uart0_init();
				//face_txCommand(FACE_COMMAND_BOUNDRATE);
				systick_step++;
				// 关电
				break;
			
			case 1:
				if(++systick_time0 > 100)
					systick_step++;
				//延时
				break;
			
			case 2:
				P03 = 1;
				systick_time0 = 0;
				systick_step++;
				// 上电
				break;
			
			case 3:
				if(++systick_time0 > 500)
					systick_step++;
				// 延时
				break;
				
			case 4:
				//Uart0_init();
				face_txCommand(FACE_COMMAND_BOUNDRATE);
				//Uart0_init9600();
				//face_txCommand(FACE_COMMAND_OPEN);
			
			  systick_time0 = 0;
				systick_step++;
				// 发送检测信息
				break;
			
			case 5:
				if(++systick_time0 > 1400)
				{
					Uart0_init9600();
					systick_step++;
				}
				//延时
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
