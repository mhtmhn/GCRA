//(c)S1NIS73R
#define dbug

//Arduino
#include <SPI.h>

//RF24 by TMRh20 https://tmrh20.github.io/RF24
#include <RF24.h>

//GCRABASE by S1N1S73R
#include <GCRABASE.h>

//nRF24L01+
const uint64_t address = 0xABCDABCD71LL;
RF24 radio(9,8);//(CE,CSN)
struct packet
{
  float y,p,r;
  int fx,fy,fz;
}intercept;

//GCRABASE
GCRABASE base;
int delay_val;

void setup()
{
  //Debug
  #ifdef dbug
  Serial.begin(115200);
  #endif
  
  //nRF24L01+
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);
  radio.setAutoAck(1);
  radio.setCRCLength(RF24_CRC_8);
}

void loop() 
{
  //nRF24L01+
  radio.openReadingPipe(1,address);
  radio.startListening();
  if (radio.available()) 
    {
      radio.read( &intercept, sizeof(intercept) );             
    }
    
  //Debug
  #ifdef dbug      
      Serial.print(intercept.y); Serial.print(" ");
      Serial.print(intercept.p); Serial.print(" ");
      Serial.print(intercept.r); Serial.print(" ");
      Serial.print(intercept.fx); Serial.print(" ");
      Serial.print(intercept.fy); Serial.print(" ");
      Serial.print(intercept.fz); Serial.print(" ");
      Serial.println();
  #endif  
  
  //Mode 2
  mode2();
}

void mode2()
{
  if(intercept.p>0.1)
  {
    delay_val=mapit(intercept.p,0.1,1.5,220,50);
    base.move(left,delay_val);  
  }
  else if(intercept.p<-0.6)
  {
    delay_val=mapit(intercept.p,-0.6,-1.5,220,50);
    base.move(right,delay_val);
  }
  else if(intercept.r>0.5)
  {
    delay_val=mapit(intercept.r,0.5,1.5,220,50);
    base.move(backward,delay_val);  
  }
  else if(intercept.r<-0.5)
  {
    delay_val=mapit(intercept.r,-0.5,-1.5,220,50);
    base.move(forward,delay_val);  
   Serial.println(delay_val); 
  }
  else
  {
    base.move(brake);
  }
}

float mapit(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
