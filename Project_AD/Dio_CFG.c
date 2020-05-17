/*
 * Dio_CFG.c
 *
 * Created: 1/20/2020 7:12:44 PM
 *  Author: Costa
 */ 

#include <avr/io.h>
#include "Dio_CFG.h"

/*const Dio_Pin_State_t Pin_state[LASTPIN] ={OUTPUT,INFREE,OUTPUT,OUTPUT,OUTPUT,OUTPUT,OUTPUT,OUTPUT,INPULLUP,OUTPUT,OUTPUT,OUTPUT,INPULLUP,OUTPUT,OUTPUT,OUTPUT,OUTPUT,OUTPUT,OUTPUT,OUTPUT,OUTPUT,OUTPUT,OUTPUT,OUTPUT,OUTPUT,OUTPUT,INPULLUP,OUTPUT,OUTPUT,OUTPUT,OUTPUT,OUTPUT};*/

const Dio_Pin_State_t Pin_state[LASTPIN] =
{
	//portA
	OUTPUT,		 // 0 Motor 1
	OUTPUT,		 // 1 Motor 2
	OUTPUT,		 // 2 Motor 3
	OUTPUT,		 // 3 RGB_LED G 1
	OUTPUT,		 // 4 Light_LED 1
	OUTPUT,		 // 5 Light_LED 2
	OUTPUT,		 // 6 Light_LED 3
	OUTPUT,		 // 7 RGB_LED R 1

	//portB
	OUTPUT,				//0 RGB_LED G 5
	OUTPUT,				 // 1 LCD_RS  / 7SEG_EN1
	OUTPUT,				 // 2 LCD_RW  / 7SEG_EN2
	OUTPUT,				 // 3 LCD_E  / 7SEG_DP
	INFREE,			// 4 RGB_LED G 2
	INFREE,				 // 5 RGB_LED R 2
	OUTPUT,				 // 6 RGB_LED G 3
	INFREE,				 // 7 RGB_LED R 3
	
	//portC
	OUTPUT,		 // 0 SCL  I2C EEPROM Module
	OUTPUT,		 // 1 SDA  I2C EEPROM Module
	OUTPUT,		 // 2 LED 0
	OUTPUT,		 // 3 /A1 H-Bridge
	OUTPUT,		 // 4 /A2 H-Bridge
	OUTPUT,		 // 5 /A3 H-Bridge
	OUTPUT,		 // 6 /A4 H-Bridge
	OUTPUT,		 // 7 LED 1
	
	//portD
	OUTPUT,				 // 0 Motion_LEDS 1
	OUTPUT,				 // 1 Motion_LEDS 2
	OUTPUT,				// 2  Motion_LEDS 3
	OUTPUT,				// 3  Motion_LEDS 4
	OUTPUT,				 // 4 Motion_LEDS 5
	OUTPUT,				 // 5 RGB_LED G 4
	OUTPUT,				 // 6 RGB_LED R 4
	OUTPUT				 // 7 RGB_LED R 5
};
