/*
 * SPI.c
 *
 * Created: 3/28/2020 6:59:00 PM
 *  Author: moham
 */ 
#include "SPI.h"
#include "Dio.h"
#include "LCD_Driver.h"

void SPI_vidInit(enuSPIMode_t mode)
{
	if(mode == MASTER)
	{
		SETBIT(SPCR,MSTR);
		SETBIT(SPCR,SPR0);
		
		
		SETBIT(DDRD,6); //SS out SR
		SETBIT(DDRD,7); //SS out AD
		SETBIT(DDRB,5);	//MOSI
		SETBIT(DDRB,7);	//SCK
		
		CLEARBIT(DDRB,6);	//MISO
		
		Dio_VidWritePin(PD,6,HIGH);//Initialize SS with 1	
		Dio_VidWritePin(PD,7,HIGH);//Initialize SS with 1
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
	SPDR = 0x00;
	while(!READBIT(SPSR,SPIF));
	return SPDR;
}

void SPI_vidSlavSelect(ECU_s s)
{
	Dio_VidWritePin(PD,s,LOW);//Select slave connected to SS PB4
}

void SPI_vidSlav_NSelect(void)
{
	Dio_VidWritePin(PD,6,HIGH);//Initialize SS with 1
	Dio_VidWritePin(PD,7,HIGH);//Initialize SS with 1
}