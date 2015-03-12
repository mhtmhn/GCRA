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
//Remember, even if you have 160 degree servos, 0 means minimum position and 180 is maximum position in actuate() for ease of use! 
//just put the right pulse width in micro seconds in servocfg() described below to make this happen smoothly.
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
		
		GCRA(int,int,int,int,int,int,int);	//object parameters initialize board pins for servo0->6(5(standard)+2(mini) servos)
		
		void cfg(int servo_dps=1,int servo_delay=100,int servo_offset=0);	//(servo deg/step,delay in milliseconds,offset servo_2 by given value(in degrees)to match servo_1)
																			//as they share same DOF(D:(1d/s,10ms,0degs))
		
		void servocfg(int std_min=544,int std_max=2400,int mini_min=544,int mini_max=2400);	//init(standard servo min,standard servo max,mini servo min,mini servo max)
																							//configure servo pulse width in micro seconds(D:544,2500 for both)
		
		void init(int,int,int,int,int,int,int);	//init(waist,shoulder1,shoulder2,elbow,wrist_p,wrist_r,tool)explicitly in angles
											/*Compulsorily initialize with 7 predefined angles for robotic arm's parts. 
											Offset in shoulder1,shoulder2 must be entered manually, even if configured in cfg()!
											always add offset to shoulder2 such that shoulder2 angle> shoulder1 angle else modify source code.
											This is because shoulder1 angle is used to operate shoulder 2 automatically in actuate(shoulder,angle) using offset
											from cfg(). Mostly offset does not cross 10 degrees.
											Remember: If servos are misaligned, 0 deg will be 180 deg in your POV
											so all angles must be tested to bring robotic arm in a required stable position*/
											
		void actuate(int,int,bool inv=0);	//actuate(part to actuate,angle to snap to,invert angles)
											/*note:Hey?..Flipped a servo?..No problem use: actuate(part,angle, 1)
											this can be used to make the servo move from 0-180 in your POV.*/
	};
#endif