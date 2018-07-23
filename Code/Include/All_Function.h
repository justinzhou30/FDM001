#ifndef __ALL_FUNCTION_H__
#define __ALL_FUNCTION_H__

#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "stdio.h"

#define UART0_ISR	4
#define TIMER0_ISR	1
#define UART1_ISR	15
#define PWM_ISR		13
#define SPI_ISR		9


#define VOICE_INDEX_WELCOM	1
#define VOICE_INDEX_FACIALREAD	2
#define VOICE_INDEX_UNSUCCESSFUL	3
#define VOICE_INDEX_SUCCESSFUL	4
#define VOICE_INDEX_WATCHROAD	5
#define VOICE_INDEX_CAREFULLY	6
#define VOICE_INDEX_DANGER		7
#define VOICE_INDEX_REST		8
#define VOICE_INDEX_STOP		9
#define VOICE_INDEX_BYE			10
#define VOICE_INDEX_DI			11

#define FACE_COMMAND_SETTING	3
#define FACE_COMMAND_OPEN		0
#define FACE_COMMAND_CLOSE		1
#define FACE_COMMAND_POSITION	2
#define FACE_COMMAND_BOUNDRATE	4

#define FATI_WARRING_CLOSE	0		//¾¯¸æ¿ª¹Ø
#define FATI_WARRING_OPEN		1

void sys_init(void);
void Timer0_init(void);
void Uart0_init(void);
void Uart0_init9600(void);
void Uart1_init(void);


void Spi_init(void);
void spi_ReadInit(UINT32 addr_flash);
UINT8 spi_ReadNextByte(void);
void spi_ReadStop(void);
//void spi_Read(UINT32 addr_flash , UINT8 count , UINT8 *Paddr_mcu);
//void spi_ReadInQ(UINT32 addr_flash , UINT8 count);
//UINT8 get_spiReadState(void);
void spi_server(void);


void q_init(void);
void q_push(UINT8 qData);
UINT8 q_pop(UINT8 *qData);
UINT8 q_get_dataSize(void);
void q_server(void);


void pwm_init(void);
void set_pwmDuty(UINT8 *pBuffer);
void start_pwm(void);
void stop_pwm(void);
void pwmToMiddle(void);
void pwm_server(void);


void voice_init(void);
void getVoiceNextData(void);
void play_voice(UINT8 index);
void voice_server(void);


void face_init(void);
void face_isr_server_RI(void);
void face_isr_server_TI(void);
void face_server_10ms(void);
void face_txCommand(UINT8 face_command);
void face_txCommandSpeed(UINT8 speed);
void face_openAlarm(void);
void face_closeAlarm(void);
UINT8 fati_getWarringState(void);
void fati_setWarringState(UINT8 state);
void face_server(void);


void key_init(void);
void key_scan_10ms(void);
void key_server(void);
void openCloseServer_10ms(void);

void gps_init(void);
void gps_rxDataServer(void);
UINT8 get_gpsSpeed(void);
void gps_Server_10ms(void);
void gps_server(void);

void SW_Reset0(void);
#endif
