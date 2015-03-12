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
//note D:X means X is default value
#ifndef _GCRA_H_
	#define _GCRA_H_
	#include<Arduino.h>
	#include<Servo.h>
//Literals
#define waist 0	//parts of robotic arm. Note:wrist_p=wrist pitch,wrist_r=wrist roll in actuate()
#define shoulder 1
#define elbow 2
#define wrist_p 3
#define wrist_r 4
#define tool 5
//Class
	class GCRA
	{
		private:
		
		//datatypes
		Servo servo_0,servo_1,servo_2,servo_3,servo_4,servo_5,servo_6;
		int s_pin_0,s_pin_1,s_pin_2,s_pin_3,s_pin_4,s_pin_5,s_pin_6;
		int std_us_min,std_us_max,mini_us_min,mini_us_max,cfg_dps,cfg_delay,cfg_base_offset;
		
		public:
		//methods
		GCRA(void);	//simply creating an object will set pins of servo0->6 to default arduino pins(D:pin 0 to pin 6)
		
		GCRA(int,int,int,int,int,int,int);	//object parameters initialize servo pins for servo0->6(5(standard)+2(mini) servos)
		
		void cfg(int servo_dps=1,int servo_delay=10,int servo_offset=0);	/*(servo deg/step(D:1 degree),delay in milliseconds(D:10ms),
																			offset servo_2 by given value(in degrees)to match servo_1 as
																			they share same DOF (D:0 degrees))*/
		
		void servocfg(int std_min=544,int std_max=2400,int mini_min=544,int mini_max=2400);	/*configure servo pulse width in micro seconds ;
																							init(standard servo min,standard servo max,mini servo min,
																							mini servo max)(D:544,2500 for both)*/
		
		void init(int,int,int,int,int,int,int);	//initialize with 7 predefined angles for robotic arm
		
		void actuate(int,int,bool inv=0);	/*actuate(part to actuate,angle to snap to,invert angles) 
												note:Flipped a servo?..No problem use: actuate(x.., y.., 1)*/
		
	};
#endif