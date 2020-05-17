/*
 * I2C.h
 *
 * Created: 3/27/2020 5:53:23 PM
 *  Author: Mohamed
 */ 


#ifndef I2C_H_
#define I2C_H_

#include "STD_Types.h"

typedef enum
{
  TWI_PRESC_1,
  TWI_PRESC_4,
  TWI_PRESC_16,
  TWI_PRESC_64
}enuPrsc_t;
typedef enum
{
  WRITE,
  READ
}enuRW_t;
typedef enum
{
  NOK,
  OK
}enuRet_t;

#define START_COND_TRANSMITTED          0x08
#define RESTART_COND_TRANSMITTED        0x10
#define SLA_W_SENT_ACK_RECEIVED         0x18
#define SLA_R_SENT_ACK_RECEIVED         0x40
#define MASTER_DATA_SENT_ACK_RECEIVED   0x28
#define SLAVE_DATA_SENT_ACK_RECEIVED    0xB8
#define MASTER_DATA_RECEIVED_ACK_SENT   0x50
#define MASTER_DATA_RECEIVED_N_ACK_SENT 0x58
#define SLAVE_DATA_RECEIVED_ACK_SENT    0x80
#define SLAVE_DATA_RECEIVED_N_ACK_SENT  0x88

extern void TWI_vidMasterInit(uint32 bitrate, enuPrsc_t PS );
extern enuRet_t TWI_MasterStart(uint8 slave_add, enuRW_t dir);
extern void TWI_vidMasterStop(void);
extern enuRet_t TWI_Write(uint8 data);
extern enuRet_t TWI_Read(uint8 * outdataptr);

extern void TWI_vidSlaveInit(uint8 own_address);
extern uint8 TWI_U8Read_Interrupt();



#endif /* I2C_H_ */