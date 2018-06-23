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
#define PWM_ISR		13
#define SPI_ISR		9

void Uart0_init(void);
void Uart1_init(void);
void sys_init(void);
void pwm_init(void);
void Spi_init(void);
void spi_Read(UINT32 addr_flash , UINT8 count , UINT8 *Paddr_mcu);

#endif
