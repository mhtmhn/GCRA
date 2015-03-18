//(c)S1NIS73R
//#define dbug
#include <SPI.h>
#include <RF24.h>

//nRF24L01+
const uint64_t address = 0xABCDABCD71LL;
RF24 radio(9,8);//(CE,CSN)
struct packet
{
  float ax,ay,az;
  int gx,gy,gz,fx,fy,fz;
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
      Serial.print(intercept.ax); Serial.print(" ");
      Serial.print(intercept.ay); Serial.print(" ");
      Serial.print(intercept.az); Serial.print(" ");
      Serial.print(intercept.gx); Serial.print(" ");
      Serial.print(intercept.gy); Serial.print(" ");
      Serial.print(intercept.gz); Serial.print(" ");
      Serial.print(intercept.fx); Serial.print(" ");
      Serial.print(intercept.fy); Serial.print(" ");
      Serial.print(intercept.fz); Serial.print(" ");
      Serial.println();
  #endif
}
