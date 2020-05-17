/*
 * project_SR.c
 *
 * Created: 4/5/2020 7:42:33 PM
 * Author : Costa
 */ 


#include "Dio.h"
#include "LCD_Driver.h"
#include "STD_Types.h"
#include "EEPROM.h"
#include "Sensors.h"
#include "Timer0.h"
#include "I2C.h"
#include "SPI.h"

#define AD_ECU 0x7A
#define MGU_ECU 0x79
#define SR_ECU 0x7B

volatile uint16 Timer0_counter = 0;
volatile uint8 SPI_flag = 0;
volatile uint8 SPIRxData,SPIRxStatus;
volatile char buffer[10];
volatile uint8 sensor_thrsh[11];
volatile d=0;

void Sensor_VidReadVal(uint8 senVal[]);
void Sensor_VidCmpVal(uint8 sensor_Val[] , uint8 sensor_thrsh[], uint8 decision[]);
void Sensor_VidReadThreshold(uint8 senVal[]);

int main(void)
{
	uint8 TWITxData ;
	enuRet_t twi_ret = NOK;
	Dio_VidInit();
	LCD_VidInit();
	Sensor_VidInit();
	
	/* Arrays for values, thresholds and Decision  of sensors*/
	uint8 sensor_Val[11];
	uint8 decision[11];
	
	/************************************************************************/
	/*  Store Sensors thresholds in Internal EEPROM
	/*  where :
	/*	LDR is 150 for each Sensor  and started address at 0x10 Number of sensors 3
	/*	TS  is 22  for each Sensor  and started address at 0x20 Number of sensors 3
	/*	MD  is 0   for each Sensor  and started address at 0x30 Number of sensors 5                                                                    */
	/************************************************************************/
	/*TS (Temperature) Sensors  0x48 equivalent to 22*/
	EEPROM_VidWrite(0x0010 , 0x16);
	EEPROM_VidWrite(0x0011 , 0x16);
	EEPROM_VidWrite(0x0012 , 0x16);
	/*LDR (Lighting) Sensors 0x96 equivalent to 150*/
	EEPROM_VidWrite(0x0020 , 0x96);
	EEPROM_VidWrite(0x0021 , 0x96);
	EEPROM_VidWrite(0x0022 , 0x96);
	/*MD (Motion Detection) Sensors*/
	EEPROM_VidWrite(0x0030 , 0x00);
	EEPROM_VidWrite(0x0031 , 0x00);
	EEPROM_VidWrite(0x0032 , 0x00);
	EEPROM_VidWrite(0x0033 , 0x00);
	EEPROM_VidWrite(0x0034 , 0x00);
	
	/*store all thresholds of sensors*/
	for (uint8 i=0;i<11;i++)
	{
		if (i<3)
		{
			sensor_thrsh[i] = 0x16;
		}
		else if (i<6)
		{
			sensor_thrsh[i] = 0x96;
		}
		else
		{
			sensor_thrsh[i] = 0x00;
		}
	}
	/************************************************************************/
	/*	Use Timer0 
	/*	FCPU = 8 MZ , prescalar = 64 , time_of_each_count = 8 uS, OCR = 250
	/*	then need 50 count for each 100 ms so Timer0_counter => 100 to achive 200ms      */
	/************************************************************************/
	cli();
	TIMER0_SetOCR0(250);
	TIMER0_CTC_Mode_Enable_Interrupt();
	TIMER0_VidInit(CTC,DISCONNECTED);
	TIMER0_VidStart(prescaler_64);
	sei();
	
	/* two Wire Interface declaration  */
	TWI_vidSlaveInit(SR_ECU);
	sei();
	
	/*SPI */
	SPI_vidInit(SLAVE);
	
	while(1)
	{
		if (Timer0_counter >= 500)
		{
			LCD_VidDisplayChar('T');
			//TIMER0_VidStop();
			//Timer0_counter=0;
			
			/* Read and Store all values of sensors to Array */
			Sensor_VidReadVal(sensor_Val);
			/* Read and Store all Threshold values of sensors to Array */
			Sensor_VidReadThreshold(sensor_thrsh);
		
			/* Compare thresholds with actual values to Make decision to send it to AD ECU */
			Sensor_VidCmpVal(sensor_Val, sensor_thrsh, decision);
			
			/* just for test */	
			//for (uint8 i=0 ; i<11 ; i++)
			//{
				//LCD_VidCommand(Clear_display_screen);
				//
				//itoa(sensor_Val[i],buffer,10);
				//LCD_VidDisplayString(buffer);
				//
				//LCD_VidDisplayString("  ");
				//
				//itoa(sensor_thrsh[i],buffer,10);
				//LCD_VidDisplayString(buffer);
				//
				//LCD_VidDisplayString("  ");
				//
				//LCD_VidDisplayChar(decision[i]);
				//
				//_delay_ms(3000);
			//}
			
			
			/*
			 All  above statements take 24 MicroSeconds so we can neglect it
			*/
			
			/* Send Decision to AD ECU Via I2C  */
			TWI_vidMasterInit(10000,TWI_PRESC_1);
			
			//while (twi_ret != OK)
			//{
				//twi_ret = TWI_MasterStart(0x7A,WRITE);
			//}
			
			twi_ret = NOK;
			
			/* Transmit all data receiver cannot catch all of them Receiver always received f4 as initial  */
			LCD_VidCommand(Clear_display_screen);
			for (uint8 i=0 ; i<11 ; i++)
			{
				twi_ret = TWI_MasterStart(AD_ECU,WRITE);
				LCD_VidDisplayChar(decision[i]+48);
				TWITxData = (i<<4);
				TWITxData |= (decision[i]&0x0f);
				twi_ret = TWI_Write(TWITxData);
				TWI_vidMasterStop();
				_delay_ms(10);
			}
			//TWI_vidMasterStop();
			LCD_VidDisplayChar('d');
			/* Return to Slave Mode (Not sure for this step) */
			//TWI_vidSlaveInit(SR_ECU);
			Timer0_counter = 0;
		}
		
		/* check Spi Data from MUGW-ECU*/
		if (READBIT(PINB,4) == 0)
		{
			SPIRxData = SPI_u8Receive();
			
			if (SPIRxData == 0xf8)
			{
				for (uint8 i=0 ;i<3 ;i++)
				{	
					SPI_vidTransmit(sensor_thrsh[i]);
				}
				_delay_ms(10);
			}
			else if (SPIRxData == 0xf7)
			{
				for (uint8 i=0 ;i<3 ;i++)
				{
					sensor_thrsh[i] = SPI_u8Receive();
					
				}
// 				for (uint8 i=0 ;i<3 ;i++)
// 				{					
// 					LCD_VidDisplayNum_2(sensor_thrsh[i]);
// 					LCD_VidDisplayChar(' ');
// 				}
//				SPI_flag = 0;	
			}

		}
		
	}
}


void Sensor_VidReadVal(uint8 senVal[])
{
	/*Read and Store all values of sensors to Array */
	senVal[0]  = LM35_U8Read(LM35_1);
	senVal[1]  = LM35_U8Read(LM35_2);
	senVal[2]  = LM35_U8Read(LM35_3);
	senVal[3]  = LDR_U8Read(LDR_1);
	senVal[4]  = LDR_U8Read(LDR_2);
	senVal[5]  = LDR_U8Read(LDR_3);
	senVal[6]  = Motion_U8Read(MD_1);
	senVal[7]  = Motion_U8Read(MD_2);
	senVal[8]  = Motion_U8Read(MD_3);
	senVal[9]  = Motion_U8Read(MD_4);
	senVal[10] = Motion_U8Read(MD_5);
}

void Sensor_VidReadThreshold(uint8 senVal[])
{
	/*TS (Temperature) Sensors  0x48 equivalent to 22*/
	senVal[0] = EEPROM_U8Read(0x0010);
	senVal[1] = EEPROM_U8Read(0x0011);
	senVal[2] = EEPROM_U8Read(0x0012);
	/*LDR (Lighting) Sensors 0x96 equivalent to 150*/
	senVal[3] = EEPROM_U8Read(0x0020);
	senVal[4] = EEPROM_U8Read(0x0021);
	senVal[5] = EEPROM_U8Read(0x0022);
	/*MD (Motion Detection) Sensors*/
	senVal[6]  = EEPROM_U8Read(0x0030);
	senVal[7]  = EEPROM_U8Read(0x0031);
	senVal[8]  = EEPROM_U8Read(0x0032);
	senVal[9]  = EEPROM_U8Read(0x0033);
	senVal[10] = EEPROM_U8Read(0x0034);
}

void Sensor_VidCmpVal(uint8 sensor_Val[] , uint8 sensor_thrsh[], uint8 decision[])
{
	/* Compare thresholds with actual values to Make decision to send it to AD ECU */
	for (uint8 i=0 ; i<11 ; i++)
	{
		if (i<3)
		{
			if (sensor_Val[i] == sensor_thrsh[i])
			{
				decision[i] = 1;
			}
			else if (sensor_Val[i] < sensor_thrsh[i])
			{
				decision[i] = 0;
			}
			else
			{
				decision[i] = 2;
			}
		}
		else if (i<6)
		{
			if (sensor_Val[i] >= sensor_thrsh[i])
			{
				decision[i] = 1;
			}
			else 
			{
				decision[i] = 0;
			}
		}
		else
		{
			if (sensor_Val[i] == sensor_thrsh[i])
			{
				decision[i] = 0;
			}
			else
			{
				decision[i] = 1;
			}
		}
	}
}

/*send threshold to AD-ECU */
void Set_VidSenThreshold(uint8 SenAdd , uint8 val)
{
	uint16 add = (uint16)SenAdd;
	EEPROM_VidWrite(add , val);
}

ISR(TIMER0_COMP_vect)
{
	Timer0_counter+=1;
}

