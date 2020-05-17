///*
 //* project_SR.c
 //*
 //* Created: 4/5/2020 7:42:33 PM
 //* Author : Costa
 //*/ 
//
//
//#include "Dio.h"
//#include "LCD_Driver.h"
//#include "STD_Types.h"
//#include "EEPROM.h"
//#include "Sensors.h"
//#include "Timer0.h"
//
//volatile uint8 Timer_counter =0 ;  
//
//int main(void)
//{
	//uint16 read =0 ;
	//double temp ;
	//char buffer[10];
	//Dio_VidInit();
	//LCD_VidInit();
	////ADC_vidInt();
	//Sensor_VidInit();
//
	///*
		//use Timer0
		//FCPU = 8 MZ , prescalar = 64 , time_of_each_count = 8 uS, OCR = 250
		//then need 50 count for each 100 ms
	//*/	
	///*cli();
	//TIMER0_SetOCR0(250);
	//TIMER0_CTC_Mode_Enable_Interrupt();
	//TIMER0_VidInit(CTC,DISCONNECTED);
	//TIMER0_VidStart(prescaler_64);
	//sei();*/
    //
	//
    //while (1) 
    //{
		///*if (Timer_counter == 100)
		//{
			//LCD_VidCommand(Clear_display_screen);
			//LCD_VidDisplayNum(read);
			//read +=1;
			//Timer_counter=0;
		//}*/
		//
		///*
			//LM35 Sensor use 10mv for each one temperature degree
			//and we use AREF (5v) and 10bit resolution (1024) 
			//then 5v / 1024 = 4.88mv for each step
			//10mv/4.88mv = 2.5 approximate (step size)
			//read = ADC_U16Read(ADC1_CH)/2.5; 
		//*/
		////read =LM35_U16Read(LM35_1); // LDR_U16Read();//ADC_U16Read(ADC0_CH)/((10)/(5000.0/1024.0));
		//
		///*
			//TSL251RD Sensor use 15.1mv for each one lighting degree
			//and we use AREF (5v) and 10bit resolution (1024) 
			//then 5v / 1024 = 4.88mv for each step
			//15.1mv/4.88mv = 3.09 approximate (step size)
			//read = ADC_U16Read(ADC1_CH)/3.09; 
			//read = ADC_U16Read(ADC1_CH)/((15.1)/((5000.0/1024.0)));
		//*/
		   //
		////temp =  read;
		///*LCD_VidDisplayString("MD  : ");
		//read =Motion_U8Read(MD_1);
		//itoa(read,buffer,10);
		//LCD_VidDisplayString(buffer);
		//LCD_VidCommand(Shift_cursor_right);
		//
		//read =Motion_U8Read(MD_2);
		//itoa(read,buffer,10);
		//LCD_VidDisplayString(buffer);
		//LCD_VidCommand(Shift_cursor_right);
		//
		//read =Motion_U8Read(MD_3);
		//itoa(read,buffer,10);
		//LCD_VidDisplayString(buffer);
		////LCD_VidDisplayNum(read);
		//
		//LCD_VidCommand(cursor_beginning_2_line);
		//LCD_VidDisplayString("LDR :");
//
		//read =Motion_U8Read(MD_4);
		//itoa(read,buffer,10);
		//LCD_VidDisplayString(buffer);
		//LCD_VidCommand(Shift_cursor_right);
		//
		//read =Motion_U8Read(MD_5);
		//itoa(read,buffer,10);
		//LCD_VidDisplayString(buffer);
		//LCD_VidCommand(Shift_cursor_right);
		//
		//read = LDR_U16Read(LDR_3);
		//itoa(read,buffer,10);
		//LCD_VidDisplayString(buffer);*/
		//
		//
		////_delay_ms(1500);
    //}
//}
///*
//ISR(TIMER0_COMP_vect)
//{
	//Timer_counter+=1;
//}*/