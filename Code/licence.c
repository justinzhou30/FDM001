#include "All_Function.h"

#define LICENCE_SPI_ADDR	0x0300


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
