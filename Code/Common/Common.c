/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2016 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Nuvoton Technoledge Corp. 
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Apr/21/2016
//***********************************************************************************************************

#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
UINT8 hircmap0,hircmap1;
UINT16 trimvalue16bit;
//----------------------------------------------------------------------------------
// UART0 baud rate initial setting 
//----------------------------------------------------------------------------------
void InitialUART0_Timer1(UINT32 u32Baudrate)    //T1M = 1, SMOD = 1
{
		P06_Quasi_Mode;		//Setting UART pin as Quasi mode for transmit
		P07_Quasi_Mode;		//Setting UART pin as Quasi mode for transmit
	
    SCON = 0x50;     	//UART0 Mode1,REN=1,TI=1
    TMOD |= 0x20;    	//Timer1 Mode1
    
    set_SMOD;        	//UART0 Double Rate Enable
    set_T1M;
    clr_BRCK;        	//Serial port 0 baud rate clock source = Timer1
 
#ifdef FOSC_160000
    TH1 = 256 - (1000000/u32Baudrate+1);               /*16 MHz */
#endif    	
#ifdef FOSC_166000
    TH1 = 256 - (1037500/u32Baudrate);         		     /*16.6 MHz */
#endif
    set_TR1;
		set_TI;						//For printf function must setting TI = 1
}
//---------------------------------------------------------------
void InitialUART0_Timer3(UINT32 u32Baudrate) //use timer3 as Baudrate generator
{
		P06_Quasi_Mode;		//Setting UART pin as Quasi mode for transmit
		P07_Quasi_Mode;		//Setting UART pin as Quasi mode for transmit	
	
    SCON = 0x50;     //UART0 Mode1,REN=1,TI=1
    set_SMOD;        //UART0 Double Rate Enable
    T3CON &= 0xF8;   //T3PS2=0,T3PS1=0,T3PS0=0(Prescale=1)
    set_BRCK;        //UART0 baud rate clock source = Timer3

#ifdef FOSC_160000
		RH3    = HIBYTE(65536 - (1000000/u32Baudrate)-1);  		/*16 MHz */
		RL3    = LOBYTE(65536 - (1000000/u32Baudrate)-1);			/*16 MHz */
#endif
#ifdef FOSC_166000
    RH3    = HIBYTE(65536 - (1037500/u32Baudrate)); 			/*16.6 MHz */
    RL3    = LOBYTE(65536 - (1037500/u32Baudrate)); 			/*16.6 MHz */
#endif
    set_TR3;         //Trigger Timer3
		set_TI;					 //For printf function must setting TI = 1
}

UINT8 Receive_Data_From_UART0(void)
{
    UINT8 c;
    while (!RI);
    c = SBUF;
    RI = 0;
    return (c);
}

void Send_Data_To_UART0 (UINT8 c)
{
    TI = 0;
    SBUF = c;
    while(TI==0);
}


//----------------------------------------------------------------------------------
// UART1 baud rate initial setting 
//----------------------------------------------------------------------------------
void InitialUART1_Timer3(UINT32 u32Baudrate) //use timer3 as Baudrate generator
{
		P02_Quasi_Mode;		//Setting UART pin as Quasi mode for transmit
		P16_Quasi_Mode;		//Setting UART pin as Quasi mode for transmit
	
	  SCON_1 = 0x50;   	//UART1 Mode1,REN_1=1,TI_1=1
    T3CON = 0x08;   	//T3PS2=0,T3PS1=0,T3PS0=0(Prescale=1), UART1 in MODE 1
		clr_BRCK;
	
#ifdef FOSC_160000
		RH3    = HIBYTE(65536 - (1000000/u32Baudrate)-1);  		/*16 MHz */
		RL3    = LOBYTE(65536 - (1000000/u32Baudrate)-1);			/*16 MHz */
#endif
#ifdef FOSC_166000
		RH3    = HIBYTE(65536 - (1037500/u32Baudrate));  			/*16.6 MHz */
		RL3    = LOBYTE(65536 - (1037500/u32Baudrate));				/*16.6 MHz */
#endif
    set_TR3;         //Trigger Timer3
}

UINT8 Receive_Data_From_UART1(void)
{
    UINT8 c;
    
    while (!RI_1);
    c = SBUF_1;
    RI_1 = 0;
    return (c);
}

void Send_Data_To_UART1 (UINT8 c)
{
    TI_1 = 0;
    SBUF_1 = c;
    while(TI_1==0);
}


/*==========================================================================*/
//#ifdef SW_Reset
void SW_Reset0(void)
{
    TA = 0xAA;
    TA = 0x55;
    set_SWRST;
}
//#endif
/*==========================================================================*/

void MODIFY_HIRC_VLAUE(void)				// Modify HIRC to 16.6MHz, more detail please see datasheet V1.02
{
		set_IAPEN;
		IAPAL = 0x30;
    IAPAH = 0x00;
    IAPCN = READ_UID;
    set_IAPGO;
		hircmap0 = IAPFD;
		IAPAL = 0x31;
    IAPAH = 0x00;
    set_IAPGO;
		hircmap1 = IAPFD;
		clr_IAPEN;
		trimvalue16bit = ((hircmap0<<1)+(hircmap1&0x01));
		trimvalue16bit = trimvalue16bit - 15;
		hircmap1 = trimvalue16bit&0x01;
		hircmap0 = trimvalue16bit>>1;
		TA=0XAA;
		TA=0X55;
		RCTRIM0 = hircmap0;
		TA=0XAA;
		TA=0X55;
		RCTRIM1 = hircmap1;
}
