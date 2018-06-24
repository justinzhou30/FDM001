#include "All_Function.h"

#define SS_PIN          P15

#define FLASH_CMD_READ		03
#define FLASH_CMD_WRITE

#define SPI_READ_FLAG		2
#define SPI_READ_Q_FLAG		1
#define SPI_WRITE_FLAG		4
UINT8 spiFlag;


UINT8 addr_flashH[3];

UINT8 count_flash;

UINT8 *Paddr;


#define SPI_READSTATE_FINISH	0
#define SPI_READSTATE_BUSY		0xff
UINT8 spiReadState;


#define SPI_WRITE_ADDRH		1
#define SPI_WRITE_ADDRM		2
#define SPI_WRITE_ADDRL		3
#define	SPI_READ_DATA_PRE	4
#define	SPI_READ_DATA			5
#define SPI_DEFAULT				0xff
UINT8 spi_status;

void Spi_init(void)
{		
	set_DISMODF;															// SS General purpose I/O ( No Mode Fault ) 
	clr_SSOE;
 
	clr_LSBFE;																// MSB first         

	clr_CPOL;																	// The SPI clock is low in idle mode
	clr_CPHA;																	// The data is sample on the second edge of SPI clock 
	
	set_MSTR;																	// SPI in Master mode 
	 
	SPICLK_DIV2;															// Select SPI clock
	set_ESPI;																	// Enable SPI interrupt
	SS_PIN = 1;
	clr_SPIEN;																// Enable SPI function
	spiReadState = SPI_READSTATE_FINISH;
}



void spi_WriteByte(UINT8 data0)
{
	SPDR = data0;
}



UINT8 spi_ReadByte(void)
{
	return SPDR;
}



void Spi_ISR(void) interrupt SPI_ISR                  // Vecotr @  0x4B
{
	static UINT8 count_flash_bak;
	
	if(SPSR & 0x80)
  {
		clr_SPIF;
		
		if(spiFlag & SPI_READ_FLAG)										//read
		{
			
			switch(spi_status)
			{
				case SPI_WRITE_ADDRH:
					
					spi_WriteByte(addr_flashH[2]);
					spi_status = SPI_WRITE_ADDRM;
				
					break;
				
				case SPI_WRITE_ADDRM:
					
					spi_WriteByte(addr_flashH[1]);
					spi_status = SPI_WRITE_ADDRL;
					break;
				
				case SPI_WRITE_ADDRL:
					spi_WriteByte(addr_flashH[0]);
					spi_status = SPI_READ_DATA_PRE;
					break;
				
				case SPI_READ_DATA_PRE:
					spi_WriteByte(0);
					count_flash_bak = 1;
					spi_status = SPI_READ_DATA;
				
				
					break;
				
				case SPI_READ_DATA:
					if(spiFlag & SPI_READ_Q_FLAG)
					{
						q_push(spi_ReadByte());
					}
					else
						*(Paddr + count_flash_bak - 1) = spi_ReadByte();
				
					if(count_flash_bak == count_flash)
					{	
						SS_PIN = 1;
						clr_SPIEN;
						spi_status = SPI_DEFAULT;
						spiReadState = SPI_READSTATE_FINISH;
					}
					else
					{							
						spi_WriteByte(0);
						++count_flash_bak;
					}
					break;
				
				default:
					break;
				
			}
		}
		
		if(spiFlag == SPI_WRITE_FLAG)										//write
		{
		}
	}
}



void spi_Read(UINT32 addr_flash , UINT8 count , UINT8 *Paddr_mcu)
{
	SS_PIN = 1;
	spiFlag |= SPI_READ_FLAG;
	spiFlag &= ~SPI_READ_Q_FLAG;
	
	spiReadState = SPI_READSTATE_BUSY;
	
	addr_flashH[2] = (UINT8)(addr_flash >> 16);
	addr_flashH[1] = (UINT8)(addr_flash >> 8);
	addr_flashH[0] = (UINT8)(addr_flash);
	
	count_flash = count;
	
	Paddr = Paddr_mcu;
	
	SS_PIN = 0;
	set_SPIEN;
	
	spi_status = SPI_WRITE_ADDRH;
	
	spi_WriteByte(FLASH_CMD_READ);
}


void spi_ReadInQ(UINT32 addr_flash , UINT8 count)
{
	SS_PIN = 1;
	spiFlag |= SPI_READ_FLAG;
	spiFlag |= SPI_READ_Q_FLAG;
	
	spiReadState = SPI_READSTATE_BUSY;
	
	addr_flashH[2] = (UINT8)(addr_flash >> 16);
	addr_flashH[1] = (UINT8)(addr_flash >> 8);
	addr_flashH[0] = (UINT8)(addr_flash);
	
	count_flash = count;
	
	SS_PIN = 0;
	set_SPIEN;
	
	spi_status = SPI_WRITE_ADDRH;
	
	spi_WriteByte(FLASH_CMD_READ);
}


void spi_Write(UINT32 addr_flash , UINT8 count , UINT8 *Paddr_mcu)
{
	spiFlag = SPI_WRITE_FLAG;
	
	addr_flashH[2] = (UINT8)(addr_flash >> 16);
	addr_flashH[1] = (UINT8)(addr_flash >> 8);
	addr_flashH[0] = (UINT8)(addr_flash);
	
	count_flash = count;
	
	Paddr = Paddr_mcu;
	
	SS_PIN = 0;
	set_SPIEN;
	
//	spi_WriteByte();
}

UINT8 get_spiReadState(void)
{
	return spiReadState;
}
