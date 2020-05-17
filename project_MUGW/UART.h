/*
 * UART_Driver.h
 *
 * Created: 2/11/2020 8:38:51 PM
 *  Author: Costa
 */ 


#ifndef UART_H_
#define UART_H_

#include "STD_Types.h"


#define BAUD 9600

#define UBR (((F_CPU / (BAUD * 16UL))) - 1)  // Asynchronous mode 

typedef enum
{
	SYNCHRONUS = 0,
	ASYNCHRONUS
}Mode_t;

typedef enum
{
	BITS_5 = 0,
	BITS_6,
	BITS_7,
	BITS_8,
	BITS_9
}Number_Bits_t;

typedef enum
{
	TRANSMIT = 0,
	RECEIVE,
	TRANSMIT_AND_RECEIVE
}Send_Receive_t;

typedef enum
{
	DISABLE = 0,
	EVEN_PARITY,
	ODD_PARITY
}Parity_Mode_t;


extern void USART_VidInit(Mode_t mod, Number_Bits_t nbit, Send_Receive_t tr, Parity_Mode_t pmod);
extern void USART_VidTransmit(uint8 value);
extern uint8 USART_U8Reciver(void);
extern void USART_VidTransmit_String(char * str);
extern uint8 USART_U8Read(void);

/* Interrupt flag set 1 when receive  data */
//extern uint8 uart_RXC_f;

#endif /* UART_DRIVER_H_ */