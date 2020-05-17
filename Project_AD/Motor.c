/*
 * Motor.c
 *
 * Created: 4/7/2020 3:54:51 PM
 *  Author: Costa
 */ 

#include "Motor.h"
#include "Dio_CFG.h"

volatile uint8 Motor1_counter;
volatile uint8 Motor2_counter;
volatile uint8 Motor3_counter;
uint8 Motor1_cycle;
uint8 Motor1_change;
uint8 Motor2_cycle;
uint8 Motor2_change;
uint8 Motor3_cycle ;
uint8 Motor3_change;
uint8 Motor1_stop_f;
uint8 Motor2_stop_f;
uint8 Motor3_stop_f;

void Motor_VidInit(uint8 device_state[])
{
	Motor1_counter =0 ;
	Motor2_counter =0 ;
	Motor3_counter =0 ;
	
	Motor1_cycle = CYCLE;
	Motor2_cycle = CYCLE;
	Motor3_cycle = CYCLE;
	
	Motor1_change = device_state[0];
	Motor2_change = device_state[1];
	Motor3_change = device_state[2];
	(device_state[0]==NO_SPEED) ? (Motor1_stop_f = 1) : (Motor1_stop_f = 0) ;
	(device_state[1]==NO_SPEED) ? (Motor2_stop_f = 1) : (Motor2_stop_f = 0) ;
	(device_state[2]==NO_SPEED) ? (Motor3_stop_f = 1) : (Motor3_stop_f = 0) ;
	Dio_VidWritePin(PA,0,HIGH);
	Dio_VidWritePin(PA,1,HIGH);
	Dio_VidWritePin(PA,2,HIGH);
	//LCD_VidDisplayChar(device_state[0]+48);
	//LCD_VidDisplayChar(device_state[1]+48);
	//LCD_VidDisplayChar(device_state[2]+48);
	////LCD_VidDisplayChar(Motor1_stop_f+48);
	//LCD_VidDisplayChar(' ');
}

void Set_VidMotor_Speed(Motor_t moto, Speed_t speed)
{
	switch(moto)
	{
		case MOTOR_1:
			Motor1_change = speed;
			break;
		case MOTOR_2:
			Motor2_change = speed;
			break;
		case MOTOR_3:
			Motor3_change = speed;
			break;
		default:break;
	}
}

void Motor_VidOpen(Motor_t moto)
{
	switch(moto)
	{
		case MOTOR_1:
			Dio_VidWritePin(PA,0,HIGH);
			break;
		case MOTOR_2:
			Dio_VidWritePin(PA,1,HIGH);
			break;
		case MOTOR_3:
			Dio_VidWritePin(PA,2,HIGH);
			break;
		default:break;
	}
}

void Motor_VidStop(Motor_t moto)
{
	switch(moto)
	{
		case MOTOR_1:
			Dio_VidWritePin(PA,0,LOW);
			break;
		case MOTOR_2:
			Dio_VidWritePin(PA,1,LOW);
			break;
		case MOTOR_3:
			Dio_VidWritePin(PA,2,LOW);
			break;
		default:break;
	}
}