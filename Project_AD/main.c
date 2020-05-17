/*
 * Project_AD.c
 *
 * Created: 4/7/2020 1:23:16 PM
 * Author : Costa
 */ 

#include "Dio.h"
#include "LCD_Driver.h"
#include "STD_Types.h"
#include "Timer0.h"
#include "EEPROM.h"
#include "Motor.h"
#include "LED.h"
#include "I2C.h"
#include "SPI.h"

#define AD_ECU 0x7A
#define MGU_ECU 0x79
#define SR_ECU 0x7B

extern volatile uint8 Motor1_counter;
extern volatile uint8 Motor2_counter;
extern volatile uint8 Motor3_counter;
extern uint8 Motor1_cycle;
extern uint8 Motor1_change;
extern uint8 Motor2_cycle;
extern uint8 Motor2_change;
extern uint8 Motor3_cycle ;
extern uint8 Motor3_change;
extern uint8 Motor1_stop_f;
extern uint8 Motor2_stop_f;
extern uint8 Motor3_stop_f;


volatile uint8 TWI_flag = 0;
uint8 TWIRxData = 0, data, add, x=0 , d=6;
uint8 SPIRxData, TWITxData;
/* Arrays for states of devices*/
uint8 device_state[16];

void ACTU_VidSet_States(uint8 SenAdd , uint8 val);

int main(void)
{
	uint8 TWIRxData = 0, data, add, x=0 , d=6;
	uint8 SPIRxData, TWITxData;
    Dio_VidInit();
    LCD_VidInit();
	
	
	
	for (uint8 i=0 ; i<16 ; i++)
	{
		if (i<3)
		{
			device_state[i]= MEDIUM_SPEED;
		}
		else if (i<6)
		{
			device_state[i]= LOW;
		}
		else
		{
			device_state[i]= LOW;
		}
	}
	
	/*SPI */
	SPI_vidInit(SLAVE);
	
	/************************************************************************/
	/*  Store devices States in Internal EEPROM
	*  where initial states :
	*	Motors MEDIUM_SPEED
	*	Doors are closed (motion-leds off and RGB-leds light red )
	*	light-leds are off
	************************************************************************/
	
	/*Motors (Temperature Sensors) */
	EEPROM_VidWrite(0x0010 , device_state[0]);
	EEPROM_VidWrite(0x0011 , device_state[1]);
	EEPROM_VidWrite(0x0012 , device_state[2]);
	/*Light-leds (LDR Sensors) */
	EEPROM_VidWrite(0x0020 , device_state[3]);
	EEPROM_VidWrite(0x0021 , device_state[4]);
	EEPROM_VidWrite(0x0022 , device_state[5]);
	/*Motion-leds (Motion Sensors) */
	EEPROM_VidWrite(0x0030 , device_state[6]);
	EEPROM_VidWrite(0x0031 , device_state[7]);
	EEPROM_VidWrite(0x0032 , device_state[8]);
	EEPROM_VidWrite(0x0033 , device_state[9]);
	EEPROM_VidWrite(0x0034 , device_state[10]);
	/*RGB-leds (Motion Sensors) */
	EEPROM_VidWrite(0x0040 , device_state[11]);
	EEPROM_VidWrite(0x0041 , device_state[12]);
	EEPROM_VidWrite(0x0042 , device_state[13]);
	EEPROM_VidWrite(0x0043 , device_state[14]);
	EEPROM_VidWrite(0x0044 , device_state[15]);
	
	/* start LEDS and Motors*/
	Motor_VidInit(device_state);
	LED_VidInti(device_state);
	
	/************************************************************************/
	/*	Use Timer0 
	*	FCPU = 8 MZ , prescalar = 64 , time_of_each_count = 8 uS, OCR = 250
	*	      
	************************************************************************/
	cli();
	TIMER0_SetOCR0(250);
	TIMER0_CTC_Mode_Enable_Interrupt();
	TIMER0_VidInit(CTC,DISCONNECTED);
	TIMER0_VidStart(prescaler_64);
	sei();
	
	/* two Wire Interface Declaration */
	TWI_vidSlaveInit(AD_ECU);
	sei();
	char buffer[10];
    while (1) 
    {
		if (TWI_flag == 1)
		{
// 			TWIRxData = TWI_U8Read_Interrupt();
// 			TWI_flag = 0;
// 			
// 			if (TWIRxData != 0xff) /* check for not data received*/
// 			{
// 				
// 				add = (TWIRxData>>4);
// 				data = (TWIRxData&0x0f);
// 				//LCD_VidDisplayNum_2(data);
// 				/************************************************************************/
// 				/*  we have three types of addresses :
// 				 *	1) 0 to 2 these are Motor Decision  (0 is stop speed)(1 is medium speed)(2 is high speed)
// 				 *	2) 3 to 5 these are Light_Leds Decision (0 is turn off)(1 is turn on)
// 				 *	3) 6 to 10 these are Motion_Leds and RGB_Leds Decision 
// 				 *		(0 is turn off/light RED)(1 is turn on/light_Green) 
// 				/************************************************************************/
// 				if (add <3) 
// 				{
// 					if (data == 0)
// 					{
// 						device_state[add] = NO_SPEED;
// 						ACTU_VidSet_States(add,NO_SPEED);
// 					}
// 					else if (data == 1)
// 					{
// 						device_state[add] = MEDIUM_SPEED;
// 						ACTU_VidSet_States(add,MEDIUM_SPEED);
// 					}
// 					else
// 					{
// 						device_state[add] = HIGH_SPEED;
// 						ACTU_VidSet_States(add,HIGH_SPEED);
// 					}
// 				}
// 				else if (add >5)
// 				{
// 					device_state[add] = data;
// 					device_state[add+5] = data;
// 					ACTU_VidSet_States(add,data);
// 				}
// 				else
// 				{
// 					device_state[add] = data;
// 					ACTU_VidSet_States(add,data);
// 				}
// 			
// 				LED_VidInti(device_state);
// 				Motor_VidInit(device_state);
// 			}
// 			else
// 			{}
			
		}
		
		/* check Spi Data from MUGW-ECU*/
		if (READBIT(PINB,4) == 0)
		{
			SPIRxData = SPI_u8Receive();
			
			if (SPIRxData == 0xfb)
			{
				LCD_VidCommand(Clear_display_screen);
				LCD_VidDisplayChar('S');
				for (uint8 i=6 ;i<11 ;i++)
				{
					SPI_vidTransmit(device_state[i]);
					
				}
				for (uint8 i=6 ;i<11 ;i++)
				{
					LCD_VidDisplayChar(device_state[i]+48);
					LCD_VidDisplayChar(' ');
				}
				_delay_ms(10);
			}
			else if (SPIRxData == 0xfc)
			{
				LCD_VidDisplayChar('R');
				for (uint8 i=6 ;i<11 ;i++)
				{
					SPIRxData = SPI_u8Receive();
					device_state[i] = SPIRxData;
					device_state[i+5] = SPIRxData;
					//ACTU_VidSet_States(i,SPIRxData);
				}
				for (uint8 i=6 ;i<11 ;i++)
				{
					LCD_VidDisplayChar(device_state[i]+48);
					LCD_VidDisplayChar(' ');
				}
				LED_VidInti(device_state);
			}

		}
    }
}

void ACTU_VidReadStates(uint8 senVal[])
{
	/*Motors (Temperature Sensors) */
	senVal[0] = EEPROM_U8Read(0x0010);
	senVal[1] = EEPROM_U8Read(0x0011);
	senVal[2] = EEPROM_U8Read(0x0012);
	/*Light-leds (LDR Sensors) */
	senVal[3] = EEPROM_U8Read(0x0020);
	senVal[4] = EEPROM_U8Read(0x0021);
	senVal[5] = EEPROM_U8Read(0x0022);
	/*Motion-leds (Motion Sensors) */
	senVal[6]  = EEPROM_U8Read(0x0030);
	senVal[7]  = EEPROM_U8Read(0x0031);
	senVal[8]  = EEPROM_U8Read(0x0032);
	senVal[9]  = EEPROM_U8Read(0x0033);
	senVal[10] = EEPROM_U8Read(0x0034);
	/*RGB-leds (Motion Sensors) */
	senVal[11] = EEPROM_U8Read(0x0040);
	senVal[12] = EEPROM_U8Read(0x0041);
	senVal[13] = EEPROM_U8Read(0x0042);
	senVal[14] = EEPROM_U8Read(0x0043);
	senVal[15] = EEPROM_U8Read(0x0044);
}

void ACTU_VidSet_States(uint8 SenAdd , uint8 val)
{
	uint8 buffer[10];
	uint16 add = 0x0010;
	if (SenAdd < 6)
	{
		if (SenAdd > 2)
		{
			SenAdd -=3;
			add = 0x0020;
		}
		add = add + SenAdd;
		EEPROM_VidWrite(add , val);
	}
	else
	{
		SenAdd -=6;
		add = 0x0030 + SenAdd;
		EEPROM_VidWrite(add , val);
		add += 0x0010;
		EEPROM_VidWrite(add , val);
	}
}

ISR(TIMER0_COMP_vect)
{
	//conter_test+=1;
	(Motor1_stop_f==1) ? (Motor1_counter = Motor1_change) : (Motor1_counter+=1);
	(Motor2_stop_f==1) ? (Motor2_counter = Motor2_change) : (Motor2_counter+=1);
	(Motor3_stop_f==1) ? (Motor3_counter = Motor3_change) : (Motor3_counter+=1);
	if (Motor1_counter == Motor1_cycle)
	{
		Motor_VidOpen(MOTOR_1);
		Motor1_counter = 0;
	}
	else if (Motor1_counter == Motor1_change)
	{
		Motor_VidStop(MOTOR_1);
	}
	
	if (Motor2_counter == Motor2_cycle)
	{
		Motor_VidOpen(MOTOR_2);
		Motor2_counter = 0;
	}
	else if (Motor2_counter == Motor2_change)
	{
		Motor_VidStop(MOTOR_2);
	}
	
	
	if (Motor3_counter == Motor3_cycle)
	{
		Motor_VidOpen(MOTOR_3);
		Motor3_counter = 0;
	}
	else if (Motor3_counter == Motor3_change)
	{
		Motor_VidStop(MOTOR_3);
	}
	
	
}

ISR(TWI_vect)
{
	//TWI_flag = 1;
		TWIRxData = TWI_U8Read_Interrupt();
			TWI_flag = 0;
			
			if (TWIRxData != 0xff) /* check for not data received*/
			{
				
				add = (TWIRxData>>4);
				data = (TWIRxData&0x0f);
				//LCD_VidDisplayNum_2(data);
				/************************************************************************/
				/*  we have three types of addresses :
				 *	1) 0 to 2 these are Motor Decision  (0 is stop speed)(1 is medium speed)(2 is high speed)
				 *	2) 3 to 5 these are Light_Leds Decision (0 is turn off)(1 is turn on)
				 *	3) 6 to 10 these are Motion_Leds and RGB_Leds Decision 
				 *		(0 is turn off/light RED)(1 is turn on/light_Green) 
				/************************************************************************/
				if (add <3) 
				{
					if (data == 0)
					{
						device_state[add] = NO_SPEED;
						ACTU_VidSet_States(add,NO_SPEED);
					}
					else if (data == 1)
					{
						device_state[add] = MEDIUM_SPEED;
						ACTU_VidSet_States(add,MEDIUM_SPEED);
					}
					else
					{
						device_state[add] = HIGH_SPEED;
						ACTU_VidSet_States(add,HIGH_SPEED);
					}
				}
				else if (add >5)
				{
					device_state[add] = data;
					device_state[add+5] = data;
					ACTU_VidSet_States(add,data);
				}
				else
				{
					device_state[add] = data;
					ACTU_VidSet_States(add,data);
				}
			
				LED_VidInti(device_state);
				Motor_VidInit(device_state);
			}
			else
			{}
}