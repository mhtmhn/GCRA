//(c)S1NIS73R
#define dbug

//Arduino
#include <SPI.h>

//RF24 by TMRh20 https://tmrh20.github.io/RF24
#include <RF24.h>

//nRF24L01+
const uint64_t address = 0xABCDABCD71LL;
RF24 radio(9,8);//(CE,CSN)
struct packet
{
  float y,p,r;
  int fx,fy,fz;
}intercept;

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
  radio.openReadingPipe(1,address);
}

void loop() 
{
  //nRF24L01+
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
}
