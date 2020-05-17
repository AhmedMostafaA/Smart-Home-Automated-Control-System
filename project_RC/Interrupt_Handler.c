/*
 * Interrupt_Handler.c
 *
 * Created: 2/24/2020 3:26:16 AM
 *  Author: Costa
 */ 

#include "STD_Types.h"

uint8 count_int0 = 0 ;
uint8 uart_RXC_f = 0;

ISR(INT0_vect)
{
	count_int0 = (count_int0 + 1)%8;
}

ISR(USART_RXC_vect)
{
	uart_RXC_f += 1;
}