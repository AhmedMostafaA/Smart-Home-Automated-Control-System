/*
 * EEPROM.c
 *
 * Created: 4/5/2020 10:06:35 PM
 *  Author: costa
 */ 

#include "EEPROM.h"


void EEPROM_VidWrite(uint16 uiAddress, uint8 ucData)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE));
	/* Set up address and data registers */
	EEAR = uiAddress;
	EEDR = ucData;
	/* Write logical one to EEMWE */
	EECR |= (1<<EEMWE);
	/* Start eeprom write by setting EEWE */
	EECR |= (1<<EEWE);
}



uint8 EEPROM_U8Read(uint16 uiAddress)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE));
	/* Set up address register */
	EEAR = uiAddress;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from data register */
	return EEDR;
}