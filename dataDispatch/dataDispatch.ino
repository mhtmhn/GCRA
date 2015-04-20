//(c)S1NIS73R
//#define dbug

//Arduino
#include "Wire.h"
#include <SPI.h>

//RF24 by TMRh20 https://tmrh20.github.io/RF24
#include <RF24.h>

//MPU-6050, I2Cdev Library by Jeff Rowberg https://github.com/jrowberg/i2cdevlib
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
MPU6050 mpu;	//MPU6050 mpu(0x69); // <-- use for AD0 high

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// MPU orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

//ISR
volatile bool mpuInterrupt = false;	// indicates whether MPU interrupt pin has gone high
void dmpDataReady() 
{
	mpuInterrupt = true;
}

//nRF24L01+
const uint64_t address = 0xABCDABCD71LL;
RF24 radio(9,8);//(CE,CSN)
boolean sent;
struct packet
{
	float y,p,r;
	int fx,fy,fz;
}dispatch;

void setup() 
{
	#ifdef dbug
	Serial.begin(115200);
	while (!Serial); // wait for Leonardo enumeration, others continue immediately
	#endif
	
	//Flex
	dispatch.fx=0;
	dispatch.fy=0;
	dispatch.fz=0;
	
	//nRF24L01+
	radio.begin();
	radio.setPALevel(RF24_PA_MAX);
	radio.setDataRate(RF24_2MBPS);
	radio.setAutoAck(1);
	radio.setRetries(1,10);
	radio.setCRCLength(RF24_CRC_8);
	
	Wire.begin();
	TWBR = 24;
	boolean error = true;
	while (error) 
	{
		Wire.beginTransmission(104);
		error = Wire.endTransmission(); // if error = 0, we are properly connected
		if (error) 
		{ // if we aren't properly connected, try connecting again and loop
			#ifdef dbug
			Serial.println("Not properly connected to I2C, trying again");
			#endif
			Wire.begin();
			TWBR = 24; // 400kHz I2C clock
		}
	}
	#ifdef dbug
	Serial.println("Properly connected to I2C");
	// initialize device
	Serial.println(F("Initializing I2C devices..."));
	#endif
	mpu.initialize();
	#ifdef dbug
	// verify connection
	Serial.println(F("Testing device connections..."));
	Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
	// load and configure the DMP
	Serial.println(F("Initializing DMP..."));
	#endif
	devStatus = mpu.dmpInitialize();
	
	if (devStatus == 0) 
	{
		// turn on the DMP, now that it's ready
		#ifdef dbug
		Serial.println(F("Enabling DMP..."));
		#endif
		mpu.setDMPEnabled(true);
		
		// enable Arduino interrupt detection
		#ifdef dbug
		Serial.println(F("Enabling interrupt detection (Arduino external int.4)..."));
		#endif
		
		attachInterrupt(4, dmpDataReady, RISING);
		mpuIntStatus = mpu.getIntStatus();
		
		// set our DMP Ready flag so the main loop() function knows it's okay to use it
		#ifdef dbug
		Serial.println(F("DMP ready! Waiting for first interrupt..."));
		#endif
		dmpReady = true;
		
		// get expected DMP packet size for later comparison
		packetSize = mpu.dmpGetFIFOPacketSize();
	} 
	#ifdef dbug
	else 
	{
		// ERROR!
		// 1 = initial memory load failed
		// 2 = DMP configuration updates failed
		// (if it's going to break, usually the code will be 1)
		Serial.print(F("DMP Initialization failed (code "));
		Serial.print(devStatus);
		Serial.println(F(")"));
	}
	#endif
}

void loop() 
{
	if (!dmpReady) return;
	
	while (!mpuInterrupt && fifoCount < packetSize) 
	{	
                //Ensure Connection while waiting...
		boolean error = true;
		while (error) 
		{
			Wire.beginTransmission(104);
			error = Wire.endTransmission(); // if error = 0, we are properly connected
			if (error) 
			{ // if we aren't properly connected, try connecting again and loop
				Wire.begin();
				TWBR = 24; // 400kHz I2C clock
			}
		}
	}
	mpuInterrupt = false;
	mpuIntStatus = mpu.getIntStatus();
	fifoCount = mpu.getFIFOCount();
	
	if ((mpuIntStatus & 0x10) || fifoCount == 1024) 
	{
		#ifdef dbug
		Serial.println(F("FIFO overflow!"));
		Serial.println(F("Purging!"));	
		#endif
		mpu.resetFIFO();		
	} 
	else if (mpuIntStatus & 0x02) 
	{
		while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
		mpu.getFIFOBytes(fifoBuffer, packetSize);
		fifoCount -= packetSize;
		mpu.dmpGetQuaternion(&q, fifoBuffer);
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
		dispatch.y=ypr[0];
		dispatch.p=ypr[2];
		dispatch.r=ypr[1];
		
		//Flex
		dispatch.fx=analogRead(A0);
		delayMicroseconds(100);
		dispatch.fy=analogRead(A1);
		delayMicroseconds(100);
		dispatch.fz=analogRead(A2);
		delayMicroseconds(100);
		
		//nRF24L01+
		radio.openWritingPipe(address);
		radio.stopListening();
		sent=radio.writeFast( &dispatch, sizeof(dispatch) );
		delayMicroseconds(500);
                #ifdef dbug
		Serial.print(dispatch.y); Serial.print(" ");
		Serial.print(dispatch.p); Serial.print(" ");
		Serial.println(dispatch.r);
		#endif
                mpu.resetFIFO();
	}
}
