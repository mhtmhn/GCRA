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
servo_0.attach(pin_0,std_us_min,std_us_max);
servo_1.attach(pin_1,std_us_min,std_us_max);
servo_2.attach(pin_2,std_us_min,std_us_max);
servo_3.attach(pin_3,std_us_min,std_us_max);
servo_4.attach(pin_4,std_us_min,std_us_max);
servo_5.attach(pin_5,mini_us_min,mini_us_max);
servo_6.attach(pin_6,mini_us_min,mini_us_max);
}

void GCRA::cfg(int servo_dps,int servo_delay)
{
cfg_dps=servo_dps;
cfg_delay=servo_delay;
}

void GCRA::servocfg(int std_min,int std_max,int mini_min,int mini_max)
{
std_us_min=std_min;
std_us_max=std_max;
mini_us_min=mini_min;
mini_us_max=mini_max;
}

void GCRA::init(int a_0,int a_1,int a_2,int a_3,int a_4,int a_5,int a_6)
{

servo_0.write(a_0);
delay(cfg_delay);
servo_1.write(a_1);
delay(cfg_delay);
servo_2.write(a_2);
delay(cfg_delay);
servo_3.write(a_3);
delay(cfg_delay);
servo_4.write(a_4);
delay(cfg_delay);
servo_5.write(a_5);
delay(cfg_delay);
servo_6.write(a_6);
delay(cfg_delay);
}

void GCRA::actuate(int arm_part,int arm_angle)
{
}