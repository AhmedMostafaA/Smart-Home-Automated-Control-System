/*
 * ADC.c
 *
 * Created: 2/28/2020 8:10:09 PM
 *  Author: costa
 */ 


#include "ADC.h"
#include "Dio.h"

float const ADC_resolution = 5.0/1024;

void ADC_vidInt(void)
{
	/*config. ADC Multiplexer Selection Register*/
	
	/* Reference Selection Bits AVCC , 5v*/
	SETBIT(ADMUX,REFS0);
	CLEARBIT(ADMUX,REFS1);
	/* Reference Selection Bits internal AREF , 2.56*/
	//SETBIT(ADMUX,REFS0);
	//SETBIT(ADMUX,REFS1);
	
	/*Choose Right Adjust for ADC data register*/
	CLEARBIT(ADMUX,ADLAR);
	
	/*Interrupt Disable*/
	CLEARBIT(ADCSRA,ADIE);
	
	/*Auto Trigger Disable*/
	CLEARBIT(ADCSRA,ADATE);
	
	
	
	/*Select prescaler 128*/
	SETBIT(ADCSRA,ADPS0);
	SETBIT(ADCSRA,ADPS1);
	SETBIT(ADCSRA,ADPS2);
	
	/*enable ADC*/
	SETBIT(ADCSRA,ADEN);
	
	
	
}
uint16 ADC_U16Read(ADCChannel_t ch)
{
	/*Select Analog Channel*/
 	/*(READBIT(ch,0) == HIGH) ?  SETBIT(ADMUX,MUX0) : CLEARBIT(ADMUX,MUX0);
 	(READBIT(ch,1) == HIGH) ?  SETBIT(ADMUX,MUX1) : CLEARBIT(ADMUX,MUX1);
 	(READBIT(ch,2) == HIGH) ?  SETBIT(ADMUX,MUX2) : CLEARBIT(ADMUX,MUX2);
 	(READBIT(ch,3) == HIGH) ?  SETBIT(ADMUX,MUX3) : CLEARBIT(ADMUX,MUX3);
 	(READBIT(ch,4) == HIGH) ?  SETBIT(ADMUX,MUX4) : CLEARBIT(ADMUX,MUX4);*/
	ADMUX = (ADMUX & 0xE0) | (ch & 0x1F);


	
	/*Start Conversion*/
	SETBIT(ADCSRA,ADSC);
	
	/**/
	while(READBIT(ADCSRA,ADSC));
	
	return ADC;
}