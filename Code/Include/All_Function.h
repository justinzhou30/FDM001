#ifndef __ALL_FUNCTION_H__
#define __ALL_FUNCTION_H__

#include "Timer0_sysTick.h"
#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"

#define UART0_ISR	4
#define TIMER0_ISR	1
#define UART1_ISR	15

void Uart0_init(void);
void Uart1_init(void);
void sys_init(void);

#endif
