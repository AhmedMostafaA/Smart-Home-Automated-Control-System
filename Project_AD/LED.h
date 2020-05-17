/*
 * LED.h
 *
 * Created: 4/7/2020 4:25:05 PM
 *  Author: costa
 */ 


#ifndef LED_H_
#define LED_H_

#include "STD_Types.h"

typedef enum
{
	LIGHTING_LED_1 = 0,
	LIGHTING_LED_2,
	LIGHTING_LED_3,
	Motion_LED_1,
	Motion_LED_2,
	Motion_LED_3,
	Motion_LED_4,
	Motion_LED_5,
	RGB_LED_1,
	RGB_LED_2,
	RGB_LED_3,
	RGB_LED_4,
	RGB_LED_5
}LED_t;

extern void LED_VidInti(uint8 device_state[]);
extern void LED_VidOpen(LED_t led);
extern void LED_VidClose(LED_t led);
#endif /* LED_H_ */