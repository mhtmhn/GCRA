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
#include<GCRABASE.h>

GCRABASE::GCRABASE(void)
{
	b_pin_x11=A0;
	b_pin_x12=A1;
	b_pin_pwm=11;
	b_pin_x21=A3;
	b_pin_x22=A4;
	pinMode(b_pin_x11,OUTPUT);
	pinMode(b_pin_x12,OUTPUT);
	pinMode(b_pin_pwm,OUTPUT);
	pinMode(b_pin_x21,OUTPUT);
	pinMode(b_pin_x22,OUTPUT);
}

GCRABASE::GCRABASE(int a, int b, int c, int d, int e)
{
	b_pin_x11=a;
	b_pin_x12=b;
	b_pin_pwm=c;
	b_pin_x21=d;
	b_pin_x22=e;
	pinMode(b_pin_x11,OUTPUT);
	pinMode(b_pin_x12,OUTPUT);
	pinMode(b_pin_pwm,OUTPUT);
	pinMode(b_pin_x21,OUTPUT);
	pinMode(b_pin_x22,OUTPUT);	
}

void GCRABASE::move(int a, int b)
{
	base_direction=a;
	pwm_value=b;
	switch(base_direction)
	{
		case 0:
		digitalWrite(b_pin_pwm,HIGH);
		delayMicroseconds(50);
		
		digitalWrite(b_pin_x11,LOW);
		digitalWrite(b_pin_x12,HIGH);
		digitalWrite(b_pin_x21,HIGH);
		digitalWrite(b_pin_x22,LOW);
		
		analogWrite(b_pin_pwm,pwm_value);
		
		brake_factor=0;
		break;
		
		case 1:
		digitalWrite(b_pin_pwm,HIGH);
		delayMicroseconds(50);
		
		digitalWrite(b_pin_x11,HIGH);
		digitalWrite(b_pin_x12,LOW);
		digitalWrite(b_pin_x21,LOW);
		digitalWrite(b_pin_x22,HIGH);
		
		analogWrite(b_pin_pwm,pwm_value);
		
		brake_factor=1;
		break;
		
		case 2:
		digitalWrite(b_pin_pwm,HIGH);
		delayMicroseconds(50);
		
		digitalWrite(b_pin_x11,HIGH);
		digitalWrite(b_pin_x12,LOW);
		digitalWrite(b_pin_x21,HIGH);
		digitalWrite(b_pin_x22,LOW);
		
		analogWrite(b_pin_pwm,pwm_value);
		
		brake_factor=2;
		break;
		
		case 3:
		digitalWrite(b_pin_pwm,HIGH);
		delayMicroseconds(50);
		
		digitalWrite(b_pin_x11,LOW);
		digitalWrite(b_pin_x12,HIGH);
		digitalWrite(b_pin_x21,LOW);
		digitalWrite(b_pin_x22,HIGH);
		
		analogWrite(b_pin_pwm,pwm_value);
		
		brake_factor=3;
		break;
		
		case 4:
		switch(brake_factor)
		{
			case 0:
			digitalWrite(b_pin_pwm,HIGH);
			delayMicroseconds(50);
		
			digitalWrite(b_pin_x11,HIGH);
			digitalWrite(b_pin_x12,LOW);
			digitalWrite(b_pin_x21,LOW);
			digitalWrite(b_pin_x22,HIGH);
			
			digitalWrite(b_pin_pwm,LOW);
			brake_factor=4;
			break;
			
			case 1:
			digitalWrite(b_pin_pwm,HIGH);
			delayMicroseconds(50);
		
			digitalWrite(b_pin_x11,LOW);
			digitalWrite(b_pin_x12,HIGH);
			digitalWrite(b_pin_x21,HIGH);
			digitalWrite(b_pin_x22,LOW);
			
			digitalWrite(b_pin_pwm,LOW);
			brake_factor=4;
			break;
			
			case 2:
			digitalWrite(b_pin_pwm,HIGH);
			delayMicroseconds(50);
		
			digitalWrite(b_pin_x11,LOW);
			digitalWrite(b_pin_x12,HIGH);
			digitalWrite(b_pin_x21,LOW);
			digitalWrite(b_pin_x22,HIGH);
			
			digitalWrite(b_pin_pwm,LOW);
			brake_factor=4;
			break;
			
			case 3:
			digitalWrite(b_pin_pwm,HIGH);
			delayMicroseconds(50);
		
			digitalWrite(b_pin_x11,HIGH);
			digitalWrite(b_pin_x12,LOW);
			digitalWrite(b_pin_x21,HIGH);
			digitalWrite(b_pin_x22,LOW);
			
			digitalWrite(b_pin_pwm,LOW);
			brake_factor=4;
			break;
			
			case 4:
			digitalWrite(b_pin_pwm,HIGH);
			delayMicroseconds(50);
		
			digitalWrite(b_pin_x11,LOW);
			digitalWrite(b_pin_x12,LOW);
			digitalWrite(b_pin_x21,LOW);
			digitalWrite(b_pin_x22,LOW);
		
			digitalWrite(b_pin_pwm,LOW);
			break;
		}
		break;
	}
}

