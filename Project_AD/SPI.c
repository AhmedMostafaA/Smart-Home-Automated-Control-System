/*
 * SPI.c
 *
 * Created: 3/28/2020 6:59:00 PM
 *  Author: moham
 */ 
#include "SPI.h"
#include "Dio.h"


void SPI_vidInit(enuSPIMode_t mode)
{
	if(mode == MASTER)
	{
		SETBIT(SPCR,MSTR);
		
		SETBIT(DDRB,4); //SS out
		SETBIT(DDRB,5);	//MOSI
		SETBIT(DDRB,7);	//SCK
		
		CLEARBIT(DDRB,6);	//MISO
		
		Dio_VidWritePin(PB,4,HIGH);//Initialize SS with 1	
		
	}
	else
	{
		CLEARBIT(SPCR,MSTR);
		//CLEARBIT(SPCR,SPIE);
		
		CLEARBIT(DDRB,4);	//SS
		CLEARBIT(DDRB,5);	//MOSI
		CLEARBIT(DDRB,7);	//SCK
		SETBIT(DDRB,6);		//MISO
		
	}
	SETBIT(SPCR,SPE);
}
void SPI_vidTransmit(uint8 data)
{
	SPDR = data;
	while(!READBIT(SPSR,SPIF));
}
uint8 SPI_u8Receive(void)
{
	while(!READBIT(SPSR,SPIF));
	return SPDR;
}

void SPI_vidSlavSelect(void)
{
	Dio_VidWritePin(PB,4,LOW);//Select slave connected to SS PB4
}
