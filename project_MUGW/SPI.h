/*
 * IncFile1.h
 *
 * Created: 3/28/2020 5:56:59 PM
 *  Author: moham
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

typedef enum
{
	MASTER,
	SLAVE
}enuSPIMode_t;

typedef enum
{
	SR=6,
	AD
}ECU_s;


#include "STD_Types.h"

extern void SPI_vidInit(enuSPIMode_t mode);
extern void SPI_vidTransmit(uint8 data);
extern uint8 SPI_u8Receive(void);
extern void SPI_vidSlavSelect(ECU_s s);
extern void SPI_vidSlav_NSelect(void);

#endif /* INCFILE1_H_ */