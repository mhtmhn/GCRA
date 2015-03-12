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
#ifndef _GCRA_H_
#define _GCRA_H_
#include<Arduino.h>
#include<Servo.h>


class GCRA
{
private:
Servo servo_0,servo_1,servo_2,servo_3,servo_4,servo_5,servo_6;
int s_pin_0,s_pin_1,s_pin_2,s_pin_3,s_pin_4,s_pin_5,s_pin_6;
int std_us_min,std_us_max,mini_us_min,mini_us_max,cfg_dps,cfg_delay,cfg_base_offset;
public:
GCRA(int,int,int,int,int,int,int);//object parameters initialize servo pins for servo0-6(5+2(mini) servos)
void cfg(int,int,int);	//(servo deg/step,delay in milliseconds,offset servo_2 by given value to match servo_1 as they share same DOF (default:0))
void servocfg(int,int,int,int);//configure servo pulse width in micro seconds ;init(standard servo min,standard servo max,mini servo min,mini servo max)
void init(int,int,int,int,int,int,int);//initialize with 7 predefined angles for robotic arm
void actuate(int,int);		//(part to actuate,angle to snap to)
};

#endif