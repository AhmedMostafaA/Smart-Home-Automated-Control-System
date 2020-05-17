/*
 * project_MUGW.c
 *
 * Created: 4/8/2020 4:12:06 PM
 * Author : costa
 */ 


#include "STD_Types.h"
#include "Dio.h"
#include "UART.h"
#include "I2C.h"
#include "EEPROM.h"
#include "LCD_Driver.h"
#include "Timer0.h"
#include "SPI.h"

#define AD_ECU 0x7A
#define MGU_ECU 0x79
#define SR_ECU 0x7B

volatile uint8 uart_RXC_f = 0;
volatile uint16 Timer0_counter = 0;

void Pass_VidCheck(uint8 senVal[]);
void Pass_VidChange(uint8 senVal[]);
void Pass_VidReadVal(uint8 senVal[]);
void calendar_display(uint8 calendar[]);
void calendar_Calcu(uint8 calendar[]);
void calendar_Send(uint8 calendar[]);
void calendar_receive(uint8 calendar[], uint8 RecivCale[]);
void Temp_VidSendVlu(uint8 PassTemp[]);
void RGB_VidSendVlu(uint8 RGBVlu[]);

void Recieve_SR_data_SPI(uint8 data[]);
void Send_SR_data_SPI(uint8 data[]);
void Recieve_AD_data_SPI(uint8 data[]);
void Send_AD_data_SPI(uint8 data[]);

int main(void)
{
	uint8 Uart_RXC_data , RXC_pass_digit=0,RXC_pass_flag=0 , RXC_GNR_flag=0 ,RXC_CAL_falg=0;
	uint8 RXC_TEMP_falg=0 , Rxc_RGB_flag=0;
	uint8 pass_chgchk =0;
	uint8 TWITxData;
	enuRet_t twi_ret = NOK;
	Dio_VidInit();
	LCD_VidInit();
	/*USART Initialization*/
	USART_VidInit(ASYNCHRONUS,BITS_8,TRANSMIT_AND_RECEIVE,DISABLE);
	sei();
    uint8 calendar[14]={5,4,9,5,3,2,2,9,1,2,2,0,2,0}; // SS:MM:HH dd:MM:YYYY (11:00:00)(12:01:2020)
    uint8 PassTemp[6]={'0','0','0','0','0','0'};
	//uint8 PassTemp[6]={0x00,0x15,0x01,0x016,0x02,0x17};
	uint8 Recivpass[6],RecivCale[14],RGBVlu[10]={11,0,12,0,13,1,14,1,15,0};
	
	/************************************************************************/
	/*  Store Password and Time in Internal EEPROM
	/*  where :
	/*	contain 6 digits
	/************************************************************************/
	/**/
	Pass_VidChange(PassTemp);
	
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
	
	/*88888888888888888888888888888888888888888*/
	SPI_vidInit(MASTER);
	
// 	_delay_ms(500);
// 	
// 	Send_AD_data_SPI(RGBVlu);
// 	Send_SR_data_SPI(PassTemp);
// 	_delay_ms(500);
// 	Recieve_SR_data_SPI(PassTemp);
// 	Recieve_AD_data_SPI(RGBVlu);
	/*88888888888888888888888888888888888888888888*/
	while (1) 
    {
	
		if (Timer0_counter >= 500) /* every 1 sec*/
		{
			calendar_Calcu(calendar);
			calendar_display(calendar);
			Timer0_counter=0;
			
			///*i2c to send temperature values to SD-ECU*/
			//LCD_VidCommand(Clear_display_screen);
			//twi_ret = TWI_MasterStart(SR_ECU,WRITE);
			//TWI_Write('A');
			//LCD_VidDisplayChar('A');
			//for (uint8 i=0 ; i<6 ; i++)
			//{
				//LCD_VidDisplayChar(PassTemp[i]);
				//twi_ret = TWI_Write(PassTemp[i]);
				//_delay_ms(300);
			//}
			//TWI_vidMasterStop();
			///*i2c to get temperature values from SD-ECU*/
			//twi_ret = TWI_MasterStart(SR_ECU,WRITE);
			//TWI_Write('D');
			//LCD_VidDisplayChar('D');
			//TWI_vidMasterStop();
			//TIMER0_VidStop();
		}
		
		if (uart_RXC_f != 0) //interrupt flag
		{
			Uart_RXC_data = USART_U8Reciver();
			//LCD_VidDisplayNum_2(Uart_RXC_data);
			if (RXC_GNR_flag == 0)
			{
				if (Uart_RXC_data == 0x01 || Uart_RXC_data == 0x03) //if check or change password
				{
					RXC_pass_flag = 1 ;
					RXC_GNR_flag  = 1 ;
				}
				
				if ( Uart_RXC_data == 0x03) // check or change password flag
				{
					pass_chgchk = 1;
				}
				else if(Uart_RXC_data == 0x01)
				{
					pass_chgchk = 0;
				}
				else if (Uart_RXC_data == 0x06)  // get(Transmit) calendar
				{
					RXC_GNR_flag  = 2 ;
					RXC_CAL_falg  = 1 ;
				}
				else if (Uart_RXC_data == 0x05)  // set(Receive) calendar
				{
					RXC_GNR_flag  = 1 ;
					RXC_CAL_falg  = 1 ;
				}
				else if (Uart_RXC_data == 0x08) // transmit temperature
				{
					RXC_GNR_flag =2;
					RXC_TEMP_falg=1;
				}
				else if (Uart_RXC_data == 0x07) // receive temperature
				{
					RXC_GNR_flag =1;
					RXC_TEMP_falg=1;
				}
				else if (Uart_RXC_data == 0x0c) // transmit RGB
				{
					RXC_GNR_flag =2;
					Rxc_RGB_flag =1;
				}
				else if (Uart_RXC_data == 0x0b) // receive RGB
				{
					RXC_GNR_flag =1;
					Rxc_RGB_flag =1;
				}
			}
			else if (RXC_GNR_flag == 1)
			{
				if (RXC_pass_flag == 1)
				{
					Recivpass[RXC_pass_digit] = Uart_RXC_data;
					/* all digits are received*/
					if (RXC_pass_digit == 5) 
					{
						RXC_GNR_flag   = 0;
						RXC_pass_flag  = 0;
						RXC_pass_digit = 0;
						if (pass_chgchk == 1)
						{
							Pass_VidChange(Recivpass);
						}
						else
						{
							Pass_VidCheck(Recivpass);
						}
						RXC_pass_digit-=1;
					}
					RXC_pass_digit+=1;
				}
				else if (RXC_CAL_falg == 1)
				{
					RecivCale[RXC_pass_digit] = Uart_RXC_data;
					/* all digits are received*/
					if (RXC_pass_digit == 13)
					{
						RXC_GNR_flag   = 0;
						RXC_CAL_falg  = 0;
						RXC_pass_digit = 0;
						calendar_receive(calendar,RecivCale);
						RXC_pass_digit-=1;
					}
					RXC_pass_digit+=1;
					
				}
				else if (RXC_TEMP_falg == 1)
				{
					PassTemp[RXC_pass_digit] = Uart_RXC_data;
					/* all digits are received*/
					if (RXC_pass_digit == 5)
					{
						RXC_GNR_flag   = 0;
						RXC_TEMP_falg  = 0;
						RXC_pass_digit = 0;
						/*i2c to send temperature values to SR-ECU*/
						/*888888888888888888888888888888888888888888888888888*/
						Send_SR_data_SPI(PassTemp);
						RXC_pass_digit-=1;
					}
					RXC_pass_digit+=1;
				}
				if (Rxc_RGB_flag == 1)
				{
					RGBVlu[RXC_pass_digit] = Uart_RXC_data;
					/* all digits are received*/
					if (RXC_pass_digit == 9)
					{
						RXC_GNR_flag   = 0;
						Rxc_RGB_flag  = 0;
						RXC_pass_digit = 0;
						/*00000000000000000000000000000000000000000000000000000*/
						/*i2c to send RGB values to AD-ECU*/
						Send_AD_data_SPI(RGBVlu);
						RXC_pass_digit-=1;
					}
					RXC_pass_digit+=1;
				}
			}
			
			if (RXC_GNR_flag  == 2)
			{
				if (RXC_CAL_falg == 1)
				{
					calendar_Send(calendar);
					RXC_GNR_flag = 0;
					RXC_CAL_falg = 0;
				}
				if (RXC_TEMP_falg == 1)
				{
					/*i2c to get temperature values from SR-ECU*/
					/*88888888888888888888888888888888888888888*/
					Recieve_SR_data_SPI(PassTemp);
					/*uart to send temperature values to RC-ECU (Done)*/
					Temp_VidSendVlu(PassTemp);
					RXC_GNR_flag = 0;
					RXC_CAL_falg = 0;	
				}
				if (Rxc_RGB_flag == 1)
				{
					/*0000000000000000000000000000000000000000000000000*/
					/*i2c to get RGB values from AD-ECU*/
					Recieve_AD_data_SPI(RGBVlu);
					/*UART to send RGB values to RC-ECU (Done)*/
					RGB_VidSendVlu(RGBVlu);
					RXC_GNR_flag = 0;
					Rxc_RGB_flag = 0;
				}
			}
			uart_RXC_f = 0;
		}
		
    }
}

void Pass_VidReadVal(uint8 senVal[])
{
	/*Read and Store password from EEPROM */
	senVal[0]  = EEPROM_U8Read(0x0010);
	senVal[1]  = EEPROM_U8Read(0x0011);
	senVal[2]  = EEPROM_U8Read(0x0012);
	senVal[3]  = EEPROM_U8Read(0x0013);
	senVal[4]  = EEPROM_U8Read(0x0014);
	senVal[5]  = EEPROM_U8Read(0x0015);
}

void Pass_VidChange(uint8 senVal[])
{
	EEPROM_VidWrite(0x0010 , senVal[0]);
	EEPROM_VidWrite(0x0011 , senVal[1]);
	EEPROM_VidWrite(0x0012 , senVal[2]);
	EEPROM_VidWrite(0x0013 , senVal[3]);
	EEPROM_VidWrite(0x0014 , senVal[4]);
	EEPROM_VidWrite(0x0015 , senVal[5]);
}

void Pass_VidCheck(uint8 senVal[])
{
	uint8 password[6] , flag=0;
	
	Pass_VidReadVal(password);
	
	for (uint8 i=0 ; i<6 ; i++)
	{
		if (senVal[i] != password[i])
		{
			flag =1;
			break;
		}
	}
	if (flag == 1)
	{
		LCD_VidDisplayString("Not correct");
		USART_VidTransmit(0x11);
	}
	else
	{
		LCD_VidDisplayString("correct");
		USART_VidTransmit(0x00);
	}
	
}

void calendar_display(uint8 calendar[])
{
	//SS:MM:HH
	LCD_VidCommand(Clear_display_screen);
	for (uint8 i=0 ;i<6 ;i++)
	{
		LCD_VidDisplayChar(calendar[5-i]+48);
		if (i==1 || i==3)
		{
			LCD_VidDisplayChar(':');
		}
	}
	LCD_VidCommand(cursor_beginning_2_line);
	for (uint8 i=6 ; i<14 ; i++)
	{
		LCD_VidDisplayChar(calendar[i]+48);
		if (i==7 || i==9)
		{
			LCD_VidDisplayChar(':');
		}
	}
}
void calendar_Calcu(uint8 calendar[])
{
	uint8 new_day = 0 ;
	// hh:mm:ss  24:00:01  // 20:15:41    41%10 = 1+48 
	if (calendar[0] != 9)
	{
		calendar[0]+=1;
	}
	else if (calendar[1] != 5)
	{
		calendar[0]=0;
		calendar[1]+=1;
	}
	else if (calendar[2] != 9)
	{
		calendar[0]=0;
		calendar[1]=0;
		calendar[2]+=1;
	}
	else if (calendar[3] != 5)
	{
		calendar[0]=0;
		calendar[1]=0;
		calendar[2]=0;
		calendar[3]+=1;
	}
	else if (calendar[4] != 9)
	{
		calendar[0]=0;
		calendar[1]=0;
		calendar[2]=0;
		calendar[3]=0;
		calendar[4]+=1;
		if (calendar[5] == 2 && calendar[4] == 4)  // esa3a 24
		{
			calendar[5]=0;
			calendar[4]=0;
			new_day = 1;
		}
	}
	else if (calendar[5] != 3)
	{
		calendar[0]=0;
		calendar[1]=0;
		calendar[2]=0;
		calendar[3]=0;
		calendar[4]=0;
		calendar[5]+=1;
		
	}
	
	if (new_day)
	{
		if (calendar[7] != 9)
		{
			calendar[7]+=1;
		}
		else if (calendar[6] != 2)
		{
			calendar[7]=1;
			calendar[6]+=1;
		}
		else if (calendar[9] != 9)
		{
			calendar[7]=1;
			calendar[6]=0;
			calendar[9]+=1;
			if (calendar[8] == 1 && calendar[9] == 3)  // shahr 12
			{
				calendar[9]=1;
				calendar[8]=0;
			}
		}
		else
		{
			calendar[7]=1;
			calendar[6]=0;
			calendar[9]=1;
			calendar[8]+=1;
		}
		new_day=0;
	}
}
void calendar_Send(uint8 calendar[])
{
	uint8 i;
	for (i=0 ; i<6 ; i++)
	{
		USART_VidTransmit(calendar[5-i]);
	}
	for (i=6 ; i<14 ; i++)
	{
		USART_VidTransmit(calendar[i]);
	}
}
void calendar_receive(uint8 calendar[], uint8 RecivCale[])
{
	uint8 i;
	for (i=0 ; i<6 ; i++)
	{
		calendar[5-i] = RecivCale[i];
	}
	for (i=6 ; i<14 ; i++)
	{
		calendar[i] = RecivCale[i];
	}
}

void Temp_VidSendVlu(uint8 PassTemp[])
{
	uint8 i;
	for (i=0 ; i<6 ; i++)
	{
		USART_VidTransmit(PassTemp[i]);
	}
}

void RGB_VidSendVlu(uint8 RGBVlu[])
{
	uint8 i;
	for (i=0 ; i<10 ; i++)
	{
		USART_VidTransmit(RGBVlu[i]);
	}
}



void Send_SR_data_SPI(uint8 data[])
{
	
	LCD_VidCommand(Clear_display_screen);
	LCD_VidDisplayChar('s');
	SPI_vidSlavSelect(SR);
	_delay_ms(100);
	SPI_vidTransmit(0xf7);
	
	
	for (uint8 i=1 ; i<6 ; i+=2)
	{
		
		SPI_vidTransmit(data[i]);
	}
	SPI_vidSlav_NSelect();
}

void Recieve_SR_data_SPI(uint8 data[])
{
	LCD_VidCommand(Clear_display_screen);
	LCD_VidDisplayChar('R');
	uint8 SPIRXData, n=0;
	
	SPI_vidSlavSelect(SR);
	_delay_ms(100);
	SPI_vidTransmit(0xf8);
	_delay_ms(10);
	

	for (uint8 i=0 ; i<3 ;i++)
	{
		
		SPIRXData = SPI_u8Receive();

		data[n] = i;		/* sensor number */
		n++;
		data[n] = SPIRXData;	/* sensor Value*/
		n++;
		//LCD_VidDisplayNum_2(SPIRXData);
		//LCD_VidDisplayChar(' ');
	}
	SPI_vidSlav_NSelect();
	for (uint8 i=0 ; i<6 ;i++)
	{
		LCD_VidDisplayNum_2(data[i]);
		LCD_VidDisplayChar(' ');
	}

}


void Recieve_AD_data_SPI(uint8 data[])
{
	LCD_VidCommand(Clear_display_screen);
	LCD_VidDisplayString("AD R:");
	uint8 SPIRXData, n=0;
	
	SPI_vidSlavSelect(AD);
	_delay_ms(100);
	SPI_vidTransmit(0xfb);
	_delay_ms(10);
	

	for (uint8 i=11 ; i<16 ;i++)
	{
		
		SPIRXData = SPI_u8Receive();

		data[n] = i;		/* sensor number */
		n++;
		data[n] = SPIRXData;	/* sensor Value*/
		n++;
		
	}
	SPI_vidSlav_NSelect();
	for (uint8 i=1 ; i<10 ;i+=2)
	{
		LCD_VidDisplayChar(data[i]+48);
		LCD_VidDisplayChar(' ');
	}
}
void Send_AD_data_SPI(uint8 data[])
{
	uint8 SPITxData;
	LCD_VidCommand(Clear_display_screen);
	LCD_VidDisplayString("AD S:");
	
	SPI_vidSlavSelect(AD);
	_delay_ms(100);
	SPI_vidTransmit(0xfc);
	
	
	for (uint8 i=1 ; i<10 ; i+=2)
	{
// 		SPITxData = (data[i]<<4) - 5;
// 		i+=1;
// 		SPITxData |= (data[i]&0x0f);
		SPI_vidTransmit(data[i]);
		LCD_VidDisplayChar(data[i]+48);
		LCD_VidDisplayChar(' ');
	}
	SPI_vidSlav_NSelect();
}

ISR(USART_RXC_vect)
{
	uart_RXC_f += 1;
}

ISR(TIMER0_COMP_vect)
{
	Timer0_counter+=1;
}