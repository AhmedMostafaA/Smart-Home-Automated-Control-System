/*
 * Timer0.h
 *
 * Created: 2/11/2020 12:14:55 AM
 *  Author: Costa
 */ 


#ifndef TIMER0_H_
#define TIMER0_H_

#include "STD_Types.h"

typedef enum
{
	DISCONNECTED = 0,
	TOGGEL,
	CLEAR_NON_INVERTED,
	SET_INVERTED
}Output_Compare_PIN_t;

typedef enum
{
	No_clock = 0,
	No_prescaling,
	prescaler_8,
	prescaler_64,
	prescaler_256,
	prescaler_1024,
	External_clock_falling,
	External_clock_rising
}Prescaler_t;

typedef enum
{
		NORMAL = 0,
		CTC,
		PWM,
		FAST_PWM
}Timer0_mode_t;

extern void TIMER0_VidInit(Timer0_mode_t mod , Output_Compare_PIN_t com);
extern void TIMER0_VidStart(Prescaler_t prescal);
extern void TIMER0_VidStop(void);
extern uint8 TIMER0_U8GetCount(void);
extern void TIMER0_SetDutyCycle(uint8 duty);
extern void TIMER0_SetOCR0(uint8 value);
extern void TIMER0_Normal_Mode_Enable_Interrupt(void);
extern void TIMER0_CTC_Mode_Enable_Interrupt(void);
extern uint8 TIMER0_U8CTC_Flag(void);
extern uint8 TIMER0_U8Normal_Flag(void);


#endif /* TIMER0_H_ */