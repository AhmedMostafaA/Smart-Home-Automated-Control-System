/*
 * Motor.h
 *
 * Created: 4/7/2020 3:55:05 PM
 *  Author: costa
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include "STD_Types.h"

#define CYCLE 8
/*#define LOW_SPEED 2
#define MEDIUM_SPEED 4
#define HIGH_SPEED 8*/

typedef enum
{
	 LOW_SPEED = 2,
	 MEDIUM_SPEED = 4,
	 HIGH_SPEED = 8,
	 NO_SPEED,
}Speed_t;

typedef enum
{
	MOTOR_1 = 0,
	MOTOR_2,
	MOTOR_3
}Motor_t;

extern void Motor_VidInit();
extern void Set_VidMotor_Speed(Motor_t moto, Speed_t speed);
extern void Motor_VidOpen(Motor_t moto);
extern void Motor_VidStop(Motor_t moto);

#endif /* MOTOR_H_ */