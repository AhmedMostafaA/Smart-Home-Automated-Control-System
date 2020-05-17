/*
 * project_RC.c
 *
 * Created: 4/8/2020 12:59:25 AM
 * Author : Costa
 */ 

#include <math.h>
#include "STD_Types.h"
#include "Dio.h"
#include "keypad_Driver.h"
#include "LCD_Driver.h"
#include "UART.h"

void check_pass(uint8 password[]);
uint8 main_menu(void);
void calendar_view_update(uint8 calendar[]);
void temp_view_update(uint8 temp_vlu[]);
void RGB_view_update(uint8 RGB_vlu[]);

int main(void)
{
    uint8 key , keyReleased = 0;
	uint8 password[6], main_key , i ,pass_NC=1;
	uint8 calendar[14]={1,1,0,0,0,0,1,2,0,1,2,0,2,0}; // HH:MM:SS DD:MM:YYYY (11:00:00)(12:01:2020)
	uint8 temp_vlu[6]={0x00,0x16,0x01,0x017,0x02,0x18}; // temperature add then value
	uint8 RGB_vlu[10] = {11,0,12,0,13,1,14,1,15,0};
	Dio_VidInit();
	Keypad_vidInt();
	LCD_VidInit();
	
	USART_VidInit(ASYNCHRONUS,BITS_8,TRANSMIT_AND_RECEIVE,DISABLE);
	sei();
	
	/* check for password key  */
	do 
	{
		LCD_VidCommand(Clear_display_screen);
		check_pass(password);
		USART_VidTransmit(0x01);
		for (i=0 ; i<6 ; i++)
		{
			USART_VidTransmit(password[5-i]);
			//LCD_VidDisplayChar(password[5-i]);
		}
		if (USART_U8Reciver() == 0x00) // when received the password is ok
		{
			pass_NC =0;
		}
		else
		{
			LCD_VidCommand(Clear_display_screen);
			LCD_VidDisplayString("Not Correct");
			_delay_ms(500);
		}
	} while (pass_NC);
	
	while(1)
	{
		LCD_VidCommand(Clear_display_screen);
		LCD_VidDisplayString("WELCOME");
		_delay_ms(300);
	
		/* Show main menu to user*/
		main_key =  main_menu();
		LCD_VidCommand(Clear_display_screen);
		/* invalid press topic in menu*/
		if (main_key == 0 || main_key == 6)
		{
			main_key =  main_menu();
		}
		else if (main_key == 1) /*change password key*/
		{
			/* take new password from user*/
			check_pass(password);
			LCD_VidCommand(Clear_display_screen);
			/* Send password to MUGW-ECU */
			USART_VidTransmit(0x03);
			for (i=0 ; i<6 ; i++)
			{
				USART_VidTransmit(password[5-i]);
				//LCD_VidDisplayChar(password[5-i]);
			}
		
		}
		else if (main_key == 2) /* Calendar update and diplay*/
		{
			/* send to MUGW-ECU to get calendar */
			USART_VidTransmit(0x06);
			for (uint8 i=0 ; i<14 ; i++)
			{
				calendar[i] = USART_U8Reciver();
			}
		
			calendar_view_update(calendar);
		
			/* Send NEW Calender to MUGW_ECU*/
			USART_VidTransmit(0x05);
			for (uint8 i=0 ; i<14 ; i++)
			{
				LCD_VidCommand(Clear_display_screen);
				USART_VidTransmit(calendar[i]);
				_delay_ms(5);
			}
		
		}
		else if (main_key == 3)  /* A/C temperature control*/
		{
			/*send to MUGW-ECU to get temperature values */
			USART_VidTransmit(0x08);
			for (uint8 i=0 ; i<6 ; i++)
			{
				temp_vlu[i] = USART_U8Reciver();
			}
			temp_view_update(temp_vlu);
			/* Send NEW temperature values to MUGW_ECU*/
			USART_VidTransmit(0x07);
			for (uint8 i=0 ; i<6 ; i++)
			{
				LCD_VidCommand(Clear_display_screen);
				USART_VidTransmit(temp_vlu[i]);
				_delay_ms(5);
			}
		}
		else if (main_key == 4)/*Self test req*/
		{
		}
		else if (main_key == 5)/*RGB light ctrl*/
		{
			/*send to MUGW-ECU to get temperature values */
			USART_VidTransmit(0x0c);
			for (uint8 i=0 ; i<10 ; i++)
			{
				RGB_vlu[i] = USART_U8Reciver();
			}
		
			RGB_view_update(RGB_vlu);
		
			/* Send NEW temperature values to MUGW_ECU*/
			USART_VidTransmit(0x0b);
			for (uint8 i=0 ; i<10 ; i++)
			{
				LCD_VidCommand(Clear_display_screen);
				USART_VidTransmit(RGB_vlu[i]);
				_delay_ms(5);
			}
			LCD_VidDisplayString("Done");
		}
	
		
	}
}

void check_pass(uint8 password[])
{
	 uint8 key , keyReleased = 0, pass_digit=6;
	 //LCD_VidCommand(Clear_display_screen);
	 LCD_VidDisplayString(" PASSWORD..");
	 LCD_VidCommand(cursor_beginning_2_line);
	 while (1)
	 {
		 key = Keypad_U8Read();
		 Keypad_vidInt();
		 
		 /* just for check that released key to not add number more than one in one pressed */
		 if (key != 'f' && keyReleased == 1)
		 {
			 
			 //LCD_VidDisplayNum_Binary(key-48);
			 keyReleased = 0;
			 if (key>='0' && key <= '9')
			 {
				 LCD_VidDisplayChar('*');
				 //LCD_VidDisplayChar(key);
				 password[pass_digit-1] = key;
				 pass_digit -=1 ;
			 }
			 else if (key == '=')
			 {
				 /* check password contain 6 digits */
				 if (pass_digit != 0)
				 {
					 LCD_VidCommand(Clear_display_screen);
					 LCD_VidCommand(cursor_beginning_2_line);
					 LCD_VidDisplayString("ERROR Again");
					 _delay_ms(500);
					 LCD_VidCommand(Clear_display_screen);
					 LCD_VidDisplayString("Enter PASSWORD..");
					 LCD_VidCommand(cursor_beginning_2_line);
					 pass_digit = 6 ;
				 }
				 else
				 {
					 break;
				 }
			 }
		 }
		 else if (key == 'f')
		 {
			 keyReleased =1;
		 }
	 }
}

uint8 main_menu(void)
{
	uint8 key , keyReleased = 0;
	uint8 main_key = 1 , key_change=1;
	
	while (1)
	{
		key = Keypad_U8Read();
		Keypad_vidInt();
		
		/* just for check that released key to not add number more than one in one pressed */
		if (key != 'f' && keyReleased == 1)
		{
			
			//LCD_VidDisplayNum_Binary(key-48);
			keyReleased = 0;
			if (key =='8' )
			{
				if (main_key < 6)
				{
					main_key +=1;
					key_change =1;
				}
				
			}
			else if (key =='2' )
			{
				if (main_key > 0)
				{
					main_key -=1;
					key_change=1;
				}
			}
			else if (key =='=' )
			{
				break;
			}
		}
		else if (key == 'f')
		{
			keyReleased =1;
		}
		
		if (key_change == 1)
		{
			LCD_VidCommand(Clear_display_screen);
			switch(main_key)
			{
				case 1:
					LCD_VidDisplayString("Change passkey");
					break;
				case 2:
					for(uint8 i=0 ; i<8 ; i++)
					{
						LCD_VidCommand(Shift_cursor_right);
					}
					LCD_VidDisplayString("Digital calendar view and update");
					for(uint8 i=0 ; i<32 ; i++)
					{
						LCD_VidCommand(Shift_display_pos_left);
						_delay_ms(50);
					}
					LCD_VidCommand(Clear_display_screen);
					LCD_VidDisplayString("Digital calendar view and update");
					break;
				case 3:
					for(uint8 i=0 ; i<8 ; i++)
					{
						LCD_VidCommand(Shift_cursor_right);
					}
					LCD_VidDisplayString("A/C temperature control");
					for(uint8 i=0 ; i<15 ; i++)
					{
						LCD_VidCommand(Shift_display_pos_left);
						_delay_ms(50);
					}
					LCD_VidCommand(Clear_display_screen);
					LCD_VidDisplayString("A/C temperature control");
					break;
				case 4:
					LCD_VidDisplayString("Self test req");
					break;
				case 5:
					LCD_VidDisplayString("RGB light ctrl");
					break;
				default:
					LCD_VidDisplayString("No More");
			}
			
			key_change=0;
		}		
	}
	return (main_key);
}

void calendar_view_update(uint8 calendar[])
{
	uint8 key , keyReleased = 0;
	uint8 main_key = 0 ;
	// hh:MM:SS 
	// DD:MM:YYYY
	
	LCD_VidCommand(Clear_display_screen);
	for (uint8 i=0 ;i<6 ;i++)
	{
		LCD_VidDisplayChar(calendar[i]+48);
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
	LCD_VidCommand(cursor_beginning_1_line);
	
	while (1)
	{
		key = Keypad_U8Read();
		Keypad_vidInt();
		
		/* just for check that released key to not add number more than one in one pressed */
		if (key != 'f' && keyReleased == 1)
		{
			keyReleased = 0;
			if (key =='6' )
			{
				if (main_key == 5)
				{
					main_key +=1;
					LCD_VidCommand(cursor_beginning_2_line);
				}
				else if (main_key < 13)
				{
					main_key +=1;
					LCD_VidCommand(Shift_cursor_right);
					if (main_key==2 || main_key==4 || main_key==8 || main_key==10)
					{
						LCD_VidCommand(Shift_cursor_right);
					}
				}
				
			}
			else if (key =='4' )
			{
				if (main_key == 6)
				{
					main_key =0;
					LCD_VidCommand(cursor_beginning_1_line);
				}
				else if (main_key > 0)
				{
					main_key -=1;
					LCD_VidCommand(Shift_cursor_left);
					if (main_key==1 || main_key==3 || main_key==7 || main_key==9)
					{
						LCD_VidCommand(Shift_cursor_left);
					}
				}
			}
			else if (key =='8' )
			{
				if (calendar[main_key] != 10)
				{
					if (calendar[main_key] == 9)
					{
						calendar[main_key] = 0;
						LCD_VidDisplayChar(calendar[main_key]+48);
						LCD_VidCommand(Shift_cursor_left);
					}
					else
					{
						calendar[main_key] = calendar[main_key]+1;
						LCD_VidDisplayChar(calendar[main_key]+48);
						LCD_VidCommand(Shift_cursor_left);	
					}
					
				}
				
			}
			else if (key =='2' )
			{
				if (calendar[main_key] != 0)
				{
					calendar[main_key] = calendar[main_key]-1;
					LCD_VidDisplayChar(calendar[main_key]+48);
					LCD_VidCommand(Shift_cursor_left);
				}
			}
			else if (key =='=' )
			{
				break;
			}
		}
		else if (key == 'f')
		{
			keyReleased =1;
		}
		
		
	}
}

void temp_view_update(uint8 temp_vlu[])
{
	uint8 key , keyReleased = 0;
	uint8 main_key = 0 ;
	// add vlu add vlu add vlu
	// s1:20  s2:21 s3:22
	
	LCD_VidCommand(Clear_display_screen);
	LCD_VidDisplayChar('S');
	LCD_VidDisplayNum_2(temp_vlu[0]+1);
	LCD_VidDisplayChar(':');
	LCD_VidDisplayNum_2(temp_vlu[1]);
	
	LCD_VidCommand(Shift_cursor_right);
	LCD_VidCommand(Shift_cursor_right);
	
	LCD_VidDisplayChar('S');
	LCD_VidDisplayNum_2(temp_vlu[2]+1);
	LCD_VidDisplayChar(':');
	LCD_VidDisplayNum_2(temp_vlu[3]);
	
	LCD_VidCommand(cursor_beginning_2_line);
	
	LCD_VidDisplayChar('S');
	LCD_VidDisplayNum_2(temp_vlu[4]+1);
	LCD_VidDisplayChar(':');
	LCD_VidDisplayNum_2(temp_vlu[5]);

	LCD_VidCommand(cursor_beginning_1_line);
	
	while (1)
	{
		key = Keypad_U8Read();
		Keypad_vidInt();
		
		/* just for check that released key to not add number more than one in one pressed */
		if (key != 'f' && keyReleased == 1)
		{
			keyReleased = 0;
			if (key =='6' )
			{
				if (main_key == 0)
				{
					LCD_VidCommand(0x87);
					main_key+=1;
				}
				else if (main_key == 1)
				{
					LCD_VidCommand(cursor_beginning_2_line);
					main_key +=1;
				}
				else if (main_key == 2)
				{
					LCD_VidCommand(cursor_beginning_1_line);
					main_key = 0 ;
				}
			}
			else if (key =='4' )
			{
				if (main_key == 0)
				{
					LCD_VidCommand(cursor_beginning_2_line);
					main_key = 2 ;
				}
				else if (main_key == 1)
				{
					LCD_VidCommand(cursor_beginning_1_line);
					main_key -=1;
				}
				else if (main_key == 2)
				{
					LCD_VidCommand(0x87);
					main_key -=1;
				}
			}
			else if (key =='8' )
			{
				if (main_key == 0)
				{   //0 22 1 23 2 24
					temp_vlu[1] +=1;
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidDisplayNum_2(temp_vlu[1]);
					LCD_VidCommand(cursor_beginning_1_line);
				}
				else if (main_key == 1)
				{
					temp_vlu[3] +=1;
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidDisplayNum_2(temp_vlu[3]);
					LCD_VidCommand(0x87);
				}
				else if (main_key == 2)
				{
					temp_vlu[5] +=1;
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidDisplayNum_2(temp_vlu[5]);
					LCD_VidCommand(cursor_beginning_2_line);
				}
				
			}
			else if (key =='2' )
			{
				if (main_key == 0)
				{
					temp_vlu[1] -=1;
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidDisplayNum_2(temp_vlu[1]);
					LCD_VidCommand(cursor_beginning_1_line);
				}
				else if (main_key == 1)
				{
					temp_vlu[3] -=1;
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidDisplayNum_2(temp_vlu[3]);
					LCD_VidCommand(0x87);
				}
				else if (main_key == 2)
				{
					temp_vlu[5] -=1;
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidDisplayNum_2(temp_vlu[5]);
					LCD_VidCommand(cursor_beginning_2_line);
				}
			}
			else if (key =='=' )
			{
				break;
			}
		}
		else if (key == 'f')
		{
			keyReleased =1;
		}
		
		
	}
}

void RGB_view_update(uint8 RGB_vlu[])
{
	uint8 key , keyReleased = 0;
	uint8 main_key = 0 ;
	
	LCD_VidCommand(Clear_display_screen);
	for (uint8 i=0 ; i<10 ; i++)
	{
		if (i==6)
		{
			LCD_VidCommand(cursor_beginning_2_line);
		}
		LCD_VidDisplayChar('D');
		LCD_VidDisplayNum_2(RGB_vlu[i]-10);
		i+=1;
		LCD_VidDisplayChar(':');
		LCD_VidDisplayChar(RGB_vlu[i]+48);
		
		LCD_VidCommand(Shift_cursor_right);
	}
	
	
	LCD_VidCommand(cursor_beginning_1_line);
	
	while (1)
	{
		key = Keypad_U8Read();
		Keypad_vidInt();
		
		/* just for check that released key to not add number more than one in one pressed */
		if (key != 'f' && keyReleased == 1)
		{
			keyReleased = 0;
			if (key =='6' )
			{
				if (main_key == 0)
				{
					LCD_VidCommand(0x85);
					main_key+=1;
				}
				else if (main_key == 1)
				{
					LCD_VidCommand(0x8a);
					main_key +=1;
				}
				else if (main_key == 2)
				{
					LCD_VidCommand(0xC0);
					main_key +=1  ;
				}
				else if (main_key == 3)
				{
					LCD_VidCommand(0xC5);
					main_key += 1 ;
				}
				else if (main_key == 4)
				{
					LCD_VidCommand(0x80);
					main_key = 0 ;
				}
			}
			else if (key =='4' )
			{
				if (main_key == 0)
				{
					LCD_VidCommand(0xC5);
					main_key = 4 ;
				}
				else if (main_key == 1)
				{
					LCD_VidCommand(0x80);
					main_key -=1;
				}
				else if (main_key == 2)
				{
					LCD_VidCommand(0x85);
					main_key -=1;
				}
				else if (main_key == 3)
				{
					LCD_VidCommand(0x8a);
					main_key -=1;
				}
				else if (main_key == 4)
				{
					LCD_VidCommand(0xC0);
					main_key -=1;
				}
			}
			else if (key =='8' )
			{
				if (main_key == 0)
				{
					RGB_vlu[1] =1;
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidDisplayNum_2(RGB_vlu[1]);
					LCD_VidCommand(cursor_beginning_1_line);
				}
				else if (main_key == 1)
				{
					RGB_vlu[3] =1;
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidDisplayNum_2(RGB_vlu[3]);
					LCD_VidCommand(0x85);
				}
				else if (main_key == 2)
				{
					RGB_vlu[5] =1;
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidDisplayNum_2(RGB_vlu[5]);
					LCD_VidCommand(0x8a);
				}
				else if (main_key == 3)
				{
					RGB_vlu[7] =1;
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidDisplayNum_2(RGB_vlu[7]);
					LCD_VidCommand(cursor_beginning_2_line);
				}
				else if (main_key == 4)
				{
					RGB_vlu[9] =1;
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidDisplayNum_2(RGB_vlu[9]);
					LCD_VidCommand(0xc5);
				}
				
			}
			else if (key =='2' )
			{
				if (main_key == 0)
				{
					RGB_vlu[1] =0;
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidDisplayChar(RGB_vlu[1]+48);
					LCD_VidCommand(cursor_beginning_1_line);
				}
				else if (main_key == 1)
				{
					RGB_vlu[3] =0;
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidDisplayChar(RGB_vlu[3]+48);
					LCD_VidCommand(0x85);
				}
				else if (main_key == 2)
				{
					RGB_vlu[5] =0;
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidDisplayChar(RGB_vlu[5]+48);
					LCD_VidCommand(0x8a);
				}
				else if (main_key == 3)
				{
					RGB_vlu[7] =0;
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidDisplayChar(RGB_vlu[7]+48);
					LCD_VidCommand(cursor_beginning_2_line);
				}
				else if (main_key == 4)
				{
					RGB_vlu[9] =0;
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidCommand(Shift_cursor_right);
					LCD_VidDisplayChar(RGB_vlu[9]+48);
					LCD_VidCommand(0xc5);
				}
			}
			else if (key =='=' )
			{
				break;
			}
		}
		else if (key == 'f')
		{
			keyReleased =1;
		}
		
		
	}
}