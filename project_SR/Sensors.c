/*
 * Sensors.c
 *
 * Created: 4/6/2020 12:52:31 PM
 *  Author: Costa
 */ 

#include "Sensors.h"

extern float const ADC_resolution;
float LDR_Step = 12.1/1000;
float LM35_Step = 10.0/1000;

void Sensor_VidInit()
{
	ADC_vidInt();
}


uint8 LDR_U8Read(LDR_t ld)
{
	/*
			TSL251RD Sensor use 15.1mv for each one lighting degree
			and we use AREF (5v) and 10bit resolution (1024) 
			then 5v / 1024 = 4.88mv for each step
			15.1mv/4.88mv = 3.09 approximate (step size)
			read = ADC_U16Read(ADC1_CH)/3.09; 
			read = ADC_U16Read(ADC1_CH)/((15.1)/((5000.0/1024.0)));
		*/
	float vRead;
	vRead = ADC_U16Read(ld)*ADC_resolution; 
	return((uint8)(round(vRead/LDR_Step)));
}

uint8 LM35_U8Read(TS_t ts)
{
	/*
			LM35 Sensor use 10mv for each one temperature degree
			and we use AREF (5v) and 10bit resolution (1024) 
			then 5v / 1024 = 4.88mv for each step
			10mv/4.88mv = 2.5 approximate (step size)
			 return (ADC_U16Read(ADC0_CH)/((10)/(5000.0/1024.0)));
		*/
	float vRead;
	vRead = ADC_U16Read(ts);
	return ((uint8)(vRead/(LM35_Step/ADC_resolution)));
}

uint8 Motion_U8Read(MDS_t md)
{
	uint8 md_level = LOW;
	
	switch(md)
	{
		case MD_1:
			md_level = Dio_U8ReadPin(PD,0);
			break;
		case MD_2:
			md_level = Dio_U8ReadPin(PD,1);
			break;
		case MD_3:
			md_level = Dio_U8ReadPin(PD,2);
			break;
		case MD_4:
			md_level = Dio_U8ReadPin(PD,3);
			break;
		case MD_5:
			md_level = Dio_U8ReadPin(PD,4);
			break;
		default:		break;
	}
	return md_level;
}


