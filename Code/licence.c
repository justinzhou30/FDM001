#include "All_Function.h"

#define LICENCE_SPI_ADDR	0x0300
#define READ_96BIT_UID		0x04
#define BYTE_PROGRAM_AP		0x21
#define PAGE_ERASE_AP			0x22

unsigned char code fd001RunTimes[128] _at_ 0x1980; 
int code checkdata _at_ 0x1984;

void checksum(void)
{
	code int checksum_len=0x1234;
	code int checksum_data = 0x0000;
	static unsigned char code *p;
	
	int temp;
	long sum;
	
	sum = 0;
	p = 0;
	
	for(temp = 0 ; temp < checksum_len ; temp++)
		sum += p[temp];

	sum -= checksum_data;
	
	if(sum != checksum_data)
		while(1);
}

void run_times(void)
{ 
 // unsigned char code fd001RunTimes[128];
	UINT8 temp_data;
	
	if(AUXR1 & 0x80)			// Èí¼þ¸´Î»£¬²»¼ì²â
	{
		AUXR1 &= 0x7f;
		return;
	}
	
	
	temp_data = fd001RunTimes[0];
//	putchar(temp_data);
	if(temp_data == 0xff)
		temp_data = 20;
	
	if(temp_data == 0)
		while(1);
	
	temp_data--;
	TA = 0xAA; //CHPCON is TA protected
	TA = 0x55;
	CHPCON |= 0x01; //IAPEN = 1, enable IAP mode
	
	TA = 0xAA; //IAPUEN is TA protected
	TA = 0x55;
	IAPUEN |= 0x01; //APUEN = 1, enable APROM update
	
	IAPCN = PAGE_ERASE_AP; //Erase page 200h~27Fh
	IAPAH = 0x19;
	IAPAL = 0x80;
	IAPFD = 0xFF;
	TA = 0xAA; //IAPTRG is TA protected
	TA = 0x55;
	IAPTRG |= 0x01; //write ¡®1¡¯ to IAPGO to trigger IAP process
	
	IAPCN = BYTE_PROGRAM_AP; // Program 201h with 55h
	IAPAH = ((int)(fd001RunTimes) >> 8);	
	IAPAL = (unsigned char)(fd001RunTimes);
	IAPFD = temp_data;
	TA = 0xAA;
	TA = 0x55;
	IAPTRG |= 0x01; //write ¡®1¡¯ to IAPGO to trigger IAP process	
	
	TA = 0xAA; //IAPUEN is TA protected
	TA = 0x55;
	IAPUEN &= ~0x01; //APUEN = 0, disable APROM update
	TA = 0xAA; //CHPCON is TA protected
	TA = 0x55;
	CHPCON &= ~0x01; //IAPEN = 0, disable IAP mode
}

void read_uid(UINT8 *p)
{
	UINT8 n;
	
	TA = 0xAA; //CHPCON is TA protected
	TA = 0x55;
	CHPCON |= 0x01; //IAPEN = 1, enable IAP mode
	
	TA = 0xAA; //IAPUEN is TA protected
	TA = 0x55;
	IAPUEN |= 0x01; //APUEN = 1, enable APROM update
	
	IAPCN = READ_96BIT_UID; // Program 201h with 55h
	IAPAH = 0x00;
	for(n = 0 ; n < 0x0c ; n++)
	{
		IAPAL = n;
		TA = 0xAA;
		TA = 0x55;
		IAPTRG |= 0x01; //write ¡®1¡¯ to IAPGO to trigger IAP process
		*(p+n) = IAPFD;
	}

	TA = 0xAA; //IAPUEN is TA protected
	TA = 0x55;
	IAPUEN &= ~0x01; //APUEN = 0, disable APROM update
	TA = 0xAA; //CHPCON is TA protected
	TA = 0x55;
	CHPCON &= ~0x01; //IAPEN = 0, disable IAP mode
	
}

void licence_check(void)
{
	code char p[] = "This firmware write by JustinZhou <justin.zhou30@qq.com>";
	
	unsigned char n;
	
	spi_ReadInit(LICENCE_SPI_ADDR);
	
	for(n = 0 ; n < sizeof(p) ; n++)
	{
		if(spi_ReadNextByte() == p[n])
			continue;
		else
			break;
	}
	
	if(n < sizeof(p))
		while(1);
	
	
	spi_ReadInit(LICENCE_SPI_ADDR+0x80);
	
	for(n = 0 ; n < 20 ; n++)
		spi_ReadNextByte();
	
	if(spi_ReadNextByte() != 'O')
		while(1);
	
	for(n = 0 ; n < 7 ; n++)
		spi_ReadNextByte();
	
	if(spi_ReadNextByte() != 'T')
		while(1);
}
