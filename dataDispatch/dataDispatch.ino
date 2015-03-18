//Uses I2Cdev device library code by Jeff Rowberg - https://github.com/jrowberg/i2cdevlib
//(c)S1NIS73R
//#define dbug
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include <SPI.h>
#include <RF24.h>

//MPU-6050
MPU6050 mpu; //For AD0=HIGH use mpu(0x69)

//MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

//MPU orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

//ISR
volatile bool mpuInterrupt = false;
void dmpDataReady() 
{
  mpuInterrupt = true;
}

//nRF24L01+
const uint64_t address = 0xABCDABCD71LL;
RF24 radio(9, 8); //(CE,CSN)
boolean sent;
struct packet
{
  float y, p, r;
  int fx, fy, fz;
} dispatch;

void setup()
{
  //Debug
#ifdef dbug
  Serial.begin(115200);
#endif

  //MPU-6050
  boolean error = true;
  while (error)
  {
    Wire.beginTransmission(104);//For AD0=HIGH use 105
    error = Wire.endTransmission();
    if (error)
    {
      Wire.begin();
      TWBR = 24;
    }
  }
  mpu.initialize();
  mpu.dmpInitialize();
  mpu.setDMPEnabled(true);
  attachInterrupt(4, dmpDataReady, RISING);//Digital pin 7 on Leonardo, Use Reference for other boards.
  mpuIntStatus = mpu.getIntStatus();
  dmpReady = true;
  packetSize = mpu.dmpGetFIFOPacketSize();
  

  //Flex
  dispatch.fx = 0;
  dispatch.fy = 0;
  dispatch.fz = 0;

  //nRF24L01+
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);
  radio.setAutoAck(1);
  radio.setRetries(2, 15);
  radio.setCRCLength(RF24_CRC_8);
  radio.openWritingPipe(address);
}

void loop()
{
   while (!mpuInterrupt && fifoCount < packetSize) 
  {
  //Flex
  dispatch.fx = analogRead(A0);
  dispatch.fy = analogRead(A1);
  dispatch.fz = analogRead(A2);
  
  //nRF24L01+
  radio.stopListening();
  sent = radio.writeFast( &dispatch, sizeof(dispatch) );
  
  //Debug
#ifdef dbug
  if (!sent)
  {
    Serial.println("Null Dispatch");
  }
  else
  {
    Serial.print("Dispatched Data:");
    Serial.print(dispatch.ax); Serial.print(",");
    Serial.print(dispatch.ay); Serial.print(",");
    Serial.print(dispatch.az); Serial.print(",");
    Serial.print(dispatch.gx); Serial.print(",");
    Serial.print(dispatch.gy); Serial.print(",");
    Serial.print(dispatch.gz); Serial.print(",");
    Serial.print(dispatch.fx); Serial.print(",");
    Serial.print(dispatch.fy); Serial.print(",");
    Serial.print(dispatch.fz);
    Serial.println();
  }
#endif
  }
  
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
  fifoCount = mpu.getFIFOCount();

  if ((mpuIntStatus & 0x10) || fifoCount == 1024) 
  {
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
    dispatch.p=ypr[1];
    dispatch.r=ypr[2];
  }
}

