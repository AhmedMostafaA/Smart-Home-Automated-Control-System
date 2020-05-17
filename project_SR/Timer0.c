/*
 * Timer0.c
 *
 * Created: 2/11/2020 12:14:32 AM
 *  Author: Costa
 */ 


#include "Timer0.h"


void TIMER0_VidInit(Timer0_mode_t mod , Output_Compare_PIN_t com)
{
	switch(mod)
	{
		case NORMAL:		
			CLEARBIT(TCCR0,WGM00);
			CLEARBIT(TCCR0,WGM01);
			break;
		case CTC:			
			CLEARBIT(TCCR0,WGM00);
			SETBIT(TCCR0,WGM01);
			break;
		case PWM:			
			SETBIT(TCCR0,WGM00);
			CLEARBIT(TCCR0,WGM01);
			break;
		case FAST_PWM:		
			SETBIT(TCCR0,WGM00);
			SETBIT(TCCR0,WGM01);
			break;
		default:			
			break;
		
	}
	
	switch(com)
	{
		case DISCONNECTED:			
			CLEARBIT(TCCR0,COM00);
			CLEARBIT(TCCR0,COM01);
			break;
		case TOGGEL:				
			SETBIT(TCCR0,COM00);
			CLEARBIT(TCCR0,COM01);
			break;
		case CLEAR_NON_INVERTED:	
			CLEARBIT(TCCR0,COM00);
			SETBIT(TCCR0,COM01);
			break;
		case SET_INVERTED:			
			SETBIT(TCCR0,COM00);
			SETBIT(TCCR0,COM01);
			break;
		default:					
			break;
	}
}

void TIMER0_VidStart(Prescaler_t prescal)
{
	switch(prescal)
	{
		case  No_clock :				
			CLEARBIT(TCCR0,CS00);
			CLEARBIT(TCCR0,CS01);
			CLEARBIT(TCCR0,CS02);
			break;
		case  No_prescaling :			
			SETBIT(TCCR0,CS00);
			CLEARBIT(TCCR0,CS01);
			CLEARBIT(TCCR0,CS02);
			break;
		case  prescaler_8 :				
			CLEARBIT(TCCR0,CS00);
			SETBIT(TCCR0,CS01);
			CLEARBIT(TCCR0,CS02);
			break;
		case  prescaler_64 :			
			SETBIT(TCCR0,CS00);
			SETBIT(TCCR0,CS01);
			CLEARBIT(TCCR0,CS02);
			break;
		case  prescaler_256 :			
			CLEARBIT(TCCR0,CS00);
			CLEARBIT(TCCR0,CS01);
			SETBIT(TCCR0,CS02);
			break;
		case  prescaler_1024 :			
			SETBIT(TCCR0,CS00);
			CLEARBIT(TCCR0,CS01);
			SETBIT(TCCR0,CS02);
			break;
		case  External_clock_falling :	
			SETBIT(TCCR0,CS00);
			SETBIT(TCCR0,CS01);
			CLEARBIT(TCCR0,CS02);
			break;
		case External_clock_rising :	
			SETBIT(TCCR0,CS00);
			SETBIT(TCCR0,CS01);
			SETBIT(TCCR0,CS02);
			break;
		default :						
			break;
		
	}
}

void TIMER0_VidStop(void)
{
	TIMER0_VidStart(No_clock);
}

uint8 TIMER0_U8GetCount(void)
{
	return TCNT0;
}


void TIMER0_SetDutyCycle(uint8 duty)
{
	uint8 oc = ((0.01*duty) * 256);
	TIMER0_SetOCR0(oc);
}



void TIMER0_SetOCR0(uint8 value)
{
	OCR0 = value;
}

void TIMER0_Normal_Mode_Enable_Interrupt(void)
{
	SETBIT(TIMSK,TOIE0);	
}

void TIMER0_CTC_Mode_Enable_Interrupt(void)
{
	SETBIT(TIMSK,OCIE0);
}

uint8 TIMER0_U8CTC_Flag(void)
{
	return READBIT(TIFR,OCF0);
}

uint8 TIMER0_U8Normal_Flag(void)
{
	return READBIT(TIFR,TOV0);
}

/*ISR(TIMER0_COMP_vect)
{
	ocf_count++;
	if (50 == ocf_count)
	{
		ocf_count =0 ;
		if ( 0 == ocf_flag)
		{
			ocf_flag = 1;
			LED_Write(LED_1,HIGH);
		}
		else{
			ocf_flag = 0 ;
			LED_Write(LED_1,LOW);
		}
	}
}

ISR(TIMER0_OVF_vect)
{
}*/