/*
 * LED.c
 *
 * Created: 4/7/2020 4:25:17 PM
 *  Author: costa
 */ 


#include "Dio.h"
#include "LED.h"

void LED_VidInti(uint8 device_state[])
{
	for (uint8 i=3 ;i<16 ;i++)
	{
		if (device_state[i] == LOW)
		{
			LED_VidClose(i-3);
		}
		else if (device_state[i] == HIGH)
		{
			LED_VidOpen(i-3);
		}
		else{}
	}
}
void LED_VidOpen(LED_t led)
{
	switch(led)
	{
		case LIGHTING_LED_1:
				Dio_VidWritePin(PA,4,HIGH);
				break;
		case LIGHTING_LED_2:
				Dio_VidWritePin(PA,5,HIGH);
				break;
		case LIGHTING_LED_3:
				Dio_VidWritePin(PA,6,HIGH);
				break;
		case Motion_LED_1:
				Dio_VidWritePin(PD,0,HIGH);
				break;
		case Motion_LED_2:
				Dio_VidWritePin(PD,1,HIGH);
				break;
		case Motion_LED_3:
				Dio_VidWritePin(PD,2,HIGH);
				break;
		case Motion_LED_4:
				Dio_VidWritePin(PD,3,HIGH);
				break;
		case Motion_LED_5:
				Dio_VidWritePin(PD,4,HIGH);
				break;
		case RGB_LED_1:
				Dio_VidWritePin(PD,0,HIGH);
				Dio_VidWritePin(PA,3,LOW);
				break;
		case RGB_LED_2:
				Dio_VidWritePin(PD,1,HIGH);
				Dio_VidWritePin(PA,7,LOW);
				break;
		case RGB_LED_3:
				Dio_VidWritePin(PD,2,HIGH);
				Dio_VidWritePin(PD,5,LOW);
				break;
		case RGB_LED_4:
				Dio_VidWritePin(PD,3,HIGH);
				Dio_VidWritePin(PD,6,LOW);
				break;
		case RGB_LED_5:
				Dio_VidWritePin(PD,4,HIGH);
				Dio_VidWritePin(PD,7,LOW);
				break;
		default:	break;
	}
}

void LED_VidClose(LED_t led)
{
	switch(led)
	{
		case LIGHTING_LED_1:
				Dio_VidWritePin(PA,4,LOW);
				break;
		case LIGHTING_LED_2:
				Dio_VidWritePin(PA,5,LOW);
				break;
		case LIGHTING_LED_3:
				Dio_VidWritePin(PA,6,LOW);
				break;
		case Motion_LED_1:
				Dio_VidWritePin(PD,0,LOW);
				break;
		case Motion_LED_2:
				Dio_VidWritePin(PD,1,LOW);
				break;
		case Motion_LED_3:
				Dio_VidWritePin(PD,2,LOW);
				break;
		case Motion_LED_4:
				Dio_VidWritePin(PD,3,LOW);
				break;
		case Motion_LED_5:
				Dio_VidWritePin(PD,4,LOW);
				break;
		case RGB_LED_1:
				Dio_VidWritePin(PD,0,LOW);
				Dio_VidWritePin(PA,3,HIGH);
				break;
				case RGB_LED_2:
				Dio_VidWritePin(PD,1,LOW);
				Dio_VidWritePin(PA,7,HIGH);
				break;
				case RGB_LED_3:
				Dio_VidWritePin(PD,2,LOW);
				Dio_VidWritePin(PD,5,HIGH);
				break;
				case RGB_LED_4:
				Dio_VidWritePin(PD,3,LOW);
				Dio_VidWritePin(PD,6,HIGH);
				break;
				case RGB_LED_5:
				Dio_VidWritePin(PD,4,LOW);
				Dio_VidWritePin(PD,7,HIGH);
				break;
		default:	break;
	}
}