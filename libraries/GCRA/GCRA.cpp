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

GCRA::GCRA(void)
{
	s_pin_0=0;
	s_pin_1=1;
	s_pin_2=2;
	s_pin_3=3;
	s_pin_4=4;
	s_pin_5=5;
	s_pin_6=6;
}

GCRA::GCRA(int pin_0,int pin_1,int pin_2,int pin_3,int pin_4,int pin_5,int pin_6)
{
	s_pin_0=pin_0;
	s_pin_1=pin_1;
	s_pin_2=pin_2;
	s_pin_3=pin_3;
	s_pin_4=pin_4;
	s_pin_5=pin_5;
	s_pin_6=pin_6;
}

void GCRA::cfg(int servo_dps,int servo_delay,int servo_offset)
{
	cfg_dps=servo_dps;
	cfg_delay=servo_delay;
	cfg_base_offset=servo_offset;
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
	servo_0.attach(s_pin_0,std_us_min,std_us_max);
	servo_0.write(a_0);delay(cfg_delay);
	servo_1.attach(s_pin_1,std_us_min,std_us_max);
	servo_1.write(a_1);delay(cfg_delay);
	servo_2.attach(s_pin_2,std_us_min,std_us_max);
	servo_2.write(a_2);delay(cfg_delay);
	servo_3.attach(s_pin_3,std_us_min,std_us_max);
	servo_3.write(a_3);delay(cfg_delay);
	servo_4.attach(s_pin_4,std_us_min,std_us_max);
	servo_4.write(a_4);delay(cfg_delay);
	servo_5.attach(s_pin_5,mini_us_min,mini_us_max);
	servo_5.write(a_5);delay(cfg_delay);
	servo_6.attach(s_pin_6,mini_us_min,mini_us_max);
	servo_6.write(a_6);delay(cfg_delay);
}

void GCRA::actuate(int arm_part,int arm_angle,bool inv)
{
	int set_angle=arm_angle;	//angle to set
	int cur_angle;				//current angle
	
	switch(arm_part)
	{
		
		case 0:
		if(inv)
		{
			set_angle=180-set_angle;
		}
		cur_angle=servo_0.read();
		if(set_angle<cur_angle)
		{
			for(int i=cur_angle;i>=set_angle;i-=cfg_dps)
			{
				servo_0.write(i);
				delay(cfg_delay);
			}
		}
		else
		{
			for(int i=cur_angle;i<=set_angle;i+=cfg_dps)
			{
				servo_0.write(i);
				delay(cfg_delay);
			}
		}		
		break;
		
		case 1:
		if(inv)
		{
			set_angle=180-set_angle;
		}
		if (set_angle>180-cfg_base_offset)
		{
			set_angle=180-cfg_base_offset;
		}
		cur_angle=servo_1.read();
		if(set_angle<cur_angle)
		{
			for(int i=cur_angle;i>=set_angle;i-=cfg_dps)
			{
				servo_1.write(i);
				servo_2.write(i+cfg_base_offset);
				delay(cfg_delay);
			}
		}
		else
		{
			for(int i=cur_angle;i<=set_angle;i+=cfg_dps)
			{
				servo_1.write(i);
				servo_2.write(i+cfg_base_offset);
				delay(cfg_delay);
			}
		}
		break;
		
		case 2:
		if(inv)
		{
			set_angle=180-set_angle;
		}
		cur_angle=servo_3.read();
		if(set_angle<cur_angle)
		{
			for(int i=cur_angle;i>=set_angle;i-=cfg_dps)
			{
				servo_3.write(i);
				delay(cfg_delay);
			}
		}
		else
		{
			for(int i=cur_angle;i<=set_angle;i+=cfg_dps)
			{
				servo_3.write(i);
				delay(cfg_delay);
			}
		}
		break;
		
		case 3:
		if(inv)
		{
			set_angle=180-set_angle;
		}
		cur_angle=servo_4.read();
		if(set_angle<cur_angle)
		{
			for(int i=cur_angle;i>=set_angle;i-=cfg_dps)
			{
				servo_4.write(i);
				delay(cfg_delay);
			}
		}
		else
		{
			for(int i=cur_angle;i<=set_angle;i+=cfg_dps)
			{
				servo_4.write(i);
				delay(cfg_delay);
			}
		}
		break;
		
		case 4:
		if(inv)
		{
			set_angle=180-set_angle;
		}
		cur_angle=servo_5.read();
		if(set_angle<cur_angle)
		{
			for(int i=cur_angle;i>=set_angle;i-=cfg_dps)
			{
				servo_5.write(i);
				delay(cfg_delay);
			}
		}
		else
		{
			for(int i=cur_angle;i<=set_angle;i+=cfg_dps)
			{
				servo_5.write(i);
				delay(cfg_delay);
			}
		}
		break;
		
		case 5:
		if(inv)
		{
			set_angle=180-set_angle;
		}
		cur_angle=servo_6.read();
		if(set_angle<cur_angle)
		{
			for(int i=cur_angle;i>=set_angle;i-=cfg_dps)
			{
				servo_6.write(i);
				delay(cfg_delay);
			}
		}
		else
		{
			for(int i=cur_angle;i<=set_angle;i+=cfg_dps)
			{
				servo_6.write(i);
				delay(cfg_delay);
			}
		}
		break;
		
	}
}
