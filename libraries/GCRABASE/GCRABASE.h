/*
	----------------------------------------
	https://github.com/sinisterdev/GCRA_MKI
	----------------------------------------
	S1N1S73RDevelopers-03/10/2015
	I invest time and resources providing 
	this open source code, Please provide 
	proper citations if you plan use it in 
	any of your projects. It may benefit 
	someone else as it did to you!
	----------------------------------------
*/
#ifndef _GCRABASE_H_
#define _GCRABASE_H_
#include<Arduino.h>
//Directions
#define forward 0
#define backward 1
#define right 2
#define left 3
#define brake 4
class GCRABASE
{
	private:
	int brake_factor,b_pin_x11,b_pin_x12,b_pin_pwm,b_pin_x21,b_pin_x22;
	int base_direction,pwm_value;
	public:
	GCRABASE(void);
	GCRABASE(int,int,int,int,int);
	void move(int, int=0);
};
#endif