/*
 * EEPROM.h
 *
 * Created: 4/5/2020 10:06:53 PM
 *  Author: costa
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include "STD_Types.h"

extern void EEPROM_VidWrite(uint16 uiAddress, uint8 ucData);
extern uint8 EEPROM_U8Read(uint16 uiAddress);

#endif /* EEPROM_H_ */