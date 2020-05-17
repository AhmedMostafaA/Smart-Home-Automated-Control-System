/*
 * ADC.h
 *
 * Created: 2/28/2020 8:10:26 PM
 *  Author: costa
 */ 


#ifndef ADC_H_
#define ADC_H_

#include "STD_Types.h"

typedef enum
{
	ADC0_CH = 0,
	ADC1_CH,
	ADC2_CH,
	ADC3_CH,
	ADC4_CH,
	ADC5_CH,
	ADC6_CH,
	ADC7_CH
}ADCChannel_t;


extern void ADC_vidInt(void);
extern uint16 ADC_U16Read(ADCChannel_t ch);
#endif /* ADC_H_ */