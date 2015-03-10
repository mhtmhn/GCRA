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
#include<GCRA.h>

GCRA::GCRA(int pin_0,int pin_1,int pin_2,int pin_3,int pin_4,int pin_5,int pin_6)
{
servo_0_pin=pin_0;
servo_1_pin=pin_1;
servo_2_pin=pin_2;
servo_3_pin=pin_3;
servo_4_pin=pin_4;
servo_5_pin=pin_5;
servo_6_pin=pin_6;
}

void GCRA::init()
{
}

void GCRA::config(int servo_dps,int servo_delay)
{
servo_dps=cfg_dps;
servo_delay=cfg_delay;
}

void GCRA::actuate(int arm_part,int arm_angle)
{
}