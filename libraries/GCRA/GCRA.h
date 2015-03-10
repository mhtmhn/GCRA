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
#include<Servo.h>

//Config for servo min-max pulse width
//Standard servos
#define std_ms_min 550
#define std_ms_max 2400
//Mini servos
#define mini_ms_min 550
#define mini_ms_max 2200

class GCRA
{
private:
Servo servo_0,servo_1,servo_2,servo_3,servo_4,servo_5,servo_6;
int servo_0_pin,servo_1_pin,servo_2_pin,servo_3_pin,servo_4_pin,servo_5_pin,servo_6_pin,cfg_dps,cfg_delay;
public:
GCRA(int,int,int,int,int,int,int);//object parameters initialize servo pins for servo0-6(5+2(mini) servos)
void init();
void config(int,int);	//(servo deg/step,delay)
void actuate(int,int);		//(part to actuate,angle to snap to)
};

#endif