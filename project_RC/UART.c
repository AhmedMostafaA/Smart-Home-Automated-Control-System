/*
 * UART_Driver.c
 *
 * Created: 2/11/2020 8:38:33 PM
 *  Author: Costa
 */ 

#include <avr/io.h>
#include "UART.h"

void USART_VidInit(Mode_t mod, Number_Bits_t nbit, Send_Receive_t tr, Parity_Mode_t pmod)
{	
	/* Set U2X to 0 to use normal speed */
	CLEARBIT(UCSRA,U2X);
	
	/* Select UCSRC register through writing 1 to URSEL bit */
	SETBIT(UCSRC,URSEL);
	
	/* Configure no. of Stop bits */
	CLEARBIT(UCSRC,USBS);
	
	/* Configure UART Mode */
	switch(mod)
	{
		case ASYNCHRONUS :	CLEARBIT(UCSRC,UMSEL);
							break;
		case SYNCHRONUS :	SETBIT(UCSRC,UMSEL);
							break;
		default:			break;
	}
	
	switch(nbit)
	{
		case BITS_5 :	CLEARBIT(UCSRC,UCSZ0);
						CLEARBIT(UCSRC,UCSZ1);
						CLEARBIT(UCSRB,UCSZ2);
						break;
		case BITS_6:	SETBIT(UCSRC,UCSZ0);
						CLEARBIT(UCSRC,UCSZ1);
						CLEARBIT(UCSRB,UCSZ2);
						break;
		case BITS_7:	CLEARBIT(UCSRC,UCSZ0);
						SETBIT(UCSRC,UCSZ1);
						CLEARBIT(UCSRB,UCSZ2);
						break;
		case BITS_8:	SETBIT(UCSRC,UCSZ0);
						SETBIT(UCSRC,UCSZ1);
						CLEARBIT(UCSRB,UCSZ2);
						break;
		case BITS_9:	SETBIT(UCSRC,UCSZ0);
						SETBIT(UCSRC,UCSZ1);
						SETBIT(UCSRB,UCSZ2);
						break;
		default:		break;
	}
	
	switch(tr)
	{
		case TRANSMIT:				SETBIT(UCSRB,TXEN);
									break;
		case RECEIVE:				SETBIT(UCSRB,RXEN);
									break;
		case TRANSMIT_AND_RECEIVE:	SETBIT(UCSRB,TXEN);
									SETBIT(UCSRB,RXEN);
									break;
		default:					break;
	}
	
	/* Configure Parity check */
	switch(pmod)
	{
		case DISABLE:			CLEARBIT(UCSRC,UPM0);
								CLEARBIT(UCSRC,UPM1);
								break;
		case EVEN_PARITY:		CLEARBIT(UCSRC,UPM0);
								SETBIT(UCSRC,UPM1);
								break;
		case ODD_PARITY:		SETBIT(UCSRC,UPM0);
								SETBIT(UCSRC,UPM1);
								break;
		default:				break;
	}
	//UCSRB |= (1 << RXEN) | (1 << TXEN);/* Turn on transmission and reception */
	//UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);/* Use 8-bit character sizes */
	
	
	/* Configure the TXCIE, RXCIE, UDRIE interrupts */
	CLEARBIT(UCSRB,TXCIE);
	SETBIT(UCSRB,RXCIE);
	CLEARBIT(UCSRB,UDRIE);
	
	/* Select UBRRH register through writing 0 to URSEL bit */
	CLEARBIT(UBRRH,URSEL);
	
	/* Set the value of UBRRH and UBRRL to configure the needed Baud UBR #define */
	UBRRL = (uint8) UBR;
	UBRRH = (uint8)	(UBR>>8);	
}

void USART_VidTransmit(uint8 value)
{
	while(!(READBIT(UCSRA,UDRE)));  // wait (data register flag) get 1
	
	UDR = value;
}

uint8 USART_U8Reciver(void)
{
	while (!(READBIT(UCSRA,RXC)));/* Wait till data is received */
	return(UDR);
}

void USART_VidTransmit_String(char * str)
{
	uint8 i =0 ;
	while(str[i] != 0)
	{
		USART_VidTransmit(str[i]);
		i++;
	}
}

uint8 USART_U8Read(void)
{
	//uart_RXC_f = 0 ; /* reset flag for next data receiving */
	return UDR;
}