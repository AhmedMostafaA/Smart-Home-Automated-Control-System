/*
 * I2C.c
 *
 * Created: 3/27/2020 5:53:10 PM
 *  Author: Mohamed
 */ 

 #include "I2C.h"
 #include "math.h"
 #include "LCD_Driver.h"
 #include "Dio.h"
 
void TWI_vidMasterInit(uint32 bitrate, enuPrsc_t PS )
{
	/* TWBR value to achieve SCL freq as provided in bitrate considering TWPS = 0 (Prescalar 1)*/
	TWBR = ((F_CPU/bitrate)-16)/(2*pow(4,PS));
	TWSR = (TWSR & 0xF8) | PS;
}

enuRet_t TWI_MasterStart(uint8 slave_add, enuRW_t dir)
{
	uint8 status;
	TWCR = (1<<TWEN) | (1<<TWSTA) | (1<<TWINT);
	while(!READBIT(TWCR,TWINT));
	/* Read the status of the I2C device*/
	status = TWSR & 0xF8;
	//LCD_vidWriteNum(status);
	//LCD_vidWriteCharacter('-');
	/* Check that the status is Start or Restart conditions is transmitted */
	if (status != START_COND_TRANSMITTED && status != RESTART_COND_TRANSMITTED)
	{
		return NOK; 
	}
	/* Put the slave address and the RW bit to the data register to be sent on I2C bus */
	TWDR = (slave_add<<1)| dir; 
	/* Clear TWINT (by writing logic 1 to it ) to start the next TWI job (send address frame) */
	TWCR = (1<<TWEN) | (1<<TWINT); 
	/* Wait send job to finish */
	while(!READBIT(TWCR,TWINT));
	/* Read the status of the I2C device*/
	status = TWSR & 0xF8;
	//LCD_vidWriteNum(status);
	//LCD_vidWriteCharacter('-');
	/* Check that the status is SLA and RW is transmitted and ACK received */
	if (status != SLA_W_SENT_ACK_RECEIVED && status != SLA_R_SENT_ACK_RECEIVED)
	{
		return NOK;
	}
	return OK;
}


void TWI_vidMasterStop(void)
{
	SETBIT(TWCR,TWSTO); //TWCR |= (1<<TWSTO);
	TWCR = (1<<TWEN) | (1<<TWINT);
	while(READBIT(TWCR,TWSTO)); // Wait STOP bit to be 0 as indication of stop condition executed
}

enuRet_t TWI_Write(uint8 data)
{
	uint8 status;
	TWDR = data;
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWIE) | (1<<TWEA);
	while(!READBIT(TWCR,TWINT));
	/* Read the status of the I2C device*/
	status = TWSR & 0xF8;
	/* Check that the status is Data is transmitted and ACK received */
	if (status != MASTER_DATA_SENT_ACK_RECEIVED && status != SLAVE_DATA_SENT_ACK_RECEIVED)
	{
		return NOK;
	}
	return OK;
}

enuRet_t TWI_Read(uint8 * outdataptr)
{
	uint8 status;
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!READBIT(TWCR,TWINT));
	/* Put the data register value to the output parameter */
	*outdataptr = TWDR;
	/* Read the status of the I2C device*/
	status = TWSR & 0xF8;
	/* Check that the status is Data is received and ACK transmitted */
	if (status != MASTER_DATA_RECEIVED_ACK_SENT && status != SLAVE_DATA_RECEIVED_ACK_SENT)
	{
		return NOK;
	}
	return OK;
}

void TWI_vidSlaveInit(uint8 own_address)
{
	TWAR = (own_address<<1);
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWIE) | (1<<TWEA);
}

uint8 TWI_U8Read_Interrupt()
{
	uint8 status = TWSR & 0xF8;
	/* Check that the status is Data is received and ACK transmitted */
	if (status == MASTER_DATA_RECEIVED_ACK_SENT || status == SLAVE_DATA_RECEIVED_ACK_SENT)
	{
		TWCR |= (1 << TWINT);
		return TWDR;
	}
	else if (status == 0xA8)  /* Slave transmit */
	{
		return 0xA8;
	}
	TWCR |= (1 << TWINT);
	return 0xff;
	
}
