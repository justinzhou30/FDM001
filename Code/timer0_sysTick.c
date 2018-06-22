#include "All_Function.h"

#define TIMER0_TICK			1000

#define TH0_INIT        16000000/TIMER0_TICK
#define TL0_INIT        16000000/TIMER0_TICK

UINT8 u8TH0_Tmp,u8TL0_Tmp;
extern UINT8 flag_10ms;
void Timer0_init(void)
{
	TIMER0_MODE1_ENABLE;
	set_T0M;
	
  u8TH0_Tmp = (65536-TH0_INIT)/256;
  u8TL0_Tmp = (65536-TL0_INIT)%256;
	
  TH0 = u8TH0_Tmp;
  TL0 = u8TL0_Tmp;

	set_ET0;
	set_TR0;
}

void Timer0_ISR (void) interrupt TIMER0_ISR  //interrupt address is 0x000B
{
		static UINT8 count_10ms = TIMER0_TICK/100;
    TH0 = u8TH0_Tmp;
    TL0 = u8TL0_Tmp;

		if(--count_10ms)
			return;
		
		count_10ms = TIMER0_TICK/100;
		flag_10ms = TRUE;
//    P12 = ~P12;                     // GPIO1 toggle when interrupt
}

 