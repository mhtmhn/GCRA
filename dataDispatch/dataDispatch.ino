//(c)S1NIS73R
//#define dbug
#include <SPI.h>
#include <RF24.h>

//nRF24L01+
const uint64_t address = 0xABCDABCD71LL;
RF24 radio(9,8);//(CE,CSN)
boolean sent;
struct packet
{
  float ax,ay,az;
  int gx,gy,gz,fx,fy,fz;
}dispatch;

void setup() 
{
  //Debug
  #ifdef dbug
  Serial.begin(115200);
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
  radio.setRetries(2,15);
  radio.setCRCLength(RF24_CRC_8);
  radio.openWritingPipe(address);
}

void loop() 
{
  //Flex
  dispatch.fx=analogRead(A2);
  dispatch.fy=analogRead(A0);
  dispatch.fz=analogRead(A1);
  
  //nRF24L01+
  radio.stopListening();
  sent=radio.writeFast( &dispatch, sizeof(dispatch) );
  
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
