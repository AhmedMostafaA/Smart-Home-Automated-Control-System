/*
 * Sensors.h
 *
 * Created: 4/6/2020 12:52:55 PM
 *  Author: Costa
 */ 


#ifndef SENSORS_H_
#define SENSORS_H_

#include "Dio.h"
#include "ADC.h"

typedef enum
{
	LDR_1=4,
	LDR_2,
	LDR_3	
}LDR_t;

typedef enum
{
	LM35_1=0,
	LM35_2,
	LM35_3
}TS_t;

typedef enum
{
	MD_1=0,
	MD_2,
	MD_3,
	MD_4,
	MD_5
}MDS_t;

extern void Sensor_VidInit();
extern uint8 LDR_U8Read(LDR_t ld);
extern uint8 LM35_U8Read(TS_t ts);
extern uint8 Motion_U8Read(MDS_t md);


#endif /* SENSORS_H_ */