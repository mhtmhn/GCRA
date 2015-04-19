//(c)S1NIS73R
#define dbug
#define dbug_aux
//Arduino
#include <SPI.h>
#include<Servo.h>

//RF24 by TMRh20 https://tmrh20.github.io/RF24
#include <RF24.h>

//GCRA by S1N1S73R
#include<GCRA.h>

//GCRABASE by S1N1S73R
#include <GCRABASE.h>

//nRF24L01+
const uint64_t address = 0xABCDABCD71LL;
RF24 radio(9, 8); //(CE,CSN)
struct packet
{
  float y, p, r;
  int fx, fy, fz; //Flex1,Flex2,Flex3
} intercept;

//GCRABASE
GCRABASE base;
int delay_val;

//Switches
boolean current_mode = 0;
boolean fx_switch = 0; //Felx1Switch
boolean fy_switch = 0; //Felx2Switch
boolean fz_switch = 0; //Felx3Switch

//Switching Timers/Requisites
boolean start = 1;
unsigned long start_tick = 0;
unsigned long ticker = 0;

void setup()
{
  //Debug
#ifdef dbug
  while (!Serial) {}
  Serial.begin(115200);
#endif

#ifdef dbug_aux
  Serial.println("Mode,Flex1Switch,Flex2Switch,Flex3Switch,Time,Yaw,Pitch,Roll,Flex1,Flex2,Flex3");
#else
  Serial.println("Yaw,Pitch,Roll,Flex1,Flex2,Flex3");
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
  radio.openReadingPipe(1, address);
  radio.startListening();
recheck://Flag for goto command
  if (radio.available())
  {
    radio.read( &intercept, sizeof(intercept) );

    //Debug
#ifdef dbug
#ifdef dbug_aux
    Serial.print(current_mode); Serial.print(",");
    Serial.print(fx_switch); Serial.print(",");
    Serial.print(fy_switch); Serial.print(",");
    Serial.print(fz_switch); Serial.print(",");
    Serial.print(millis()); Serial.print(",");
#endif
    Serial.print(intercept.y); Serial.print(",");
    Serial.print(intercept.p); Serial.print(",");
    Serial.print(intercept.r); Serial.print(",");
    Serial.print(intercept.fx); Serial.print(",");
    Serial.print(intercept.fy); Serial.print(",");
    Serial.println(intercept.fz);
#endif
  }
  else
  {
    goto recheck;//Checks radio availability again
  }

  //Flex Emulated Switches
  if (intercept.fx < 720) {
    fx_switch = 1;
  } else {
    fx_switch = 0;
  }
  if (intercept.fy < 710) {
    fy_switch = 1;
  } else {
    fy_switch = 0;
  }
  if (intercept.fz < 740) {
    fz_switch = 1;
  } else {
    fz_switch = 0;
  }

  //Mode Switching (folding index and middle finger simultaneously changes mode)
  if (fy_switch && fz_switch)
  {
    if (start)
    {
      start_tick = millis();
      start = !start;
    }
    ticker = millis() - start_tick;
    if (ticker > 3000)
    {
      current_mode = !current_mode;
      start = 1;
      start_tick = 0;
    }
  }
  else
  {
    start = 1;
    start_tick = 0;
  }
  if (current_mode) {
    mode1();
  } else {
    mode2;
  }

}

//Mode 1 Function
void mode1()
{
}

//Mode 2 Function
void mode2()
{
  if (intercept.p > 0.1)
  {
    delay_val = mapit(intercept.p, 0.1, 1.5, 220, 50);
    base.move(left, delay_val);
  }
  else if (intercept.p < -0.6)
  {
    delay_val = mapit(intercept.p, -0.6, -1.5, 220, 50);
    base.move(right, delay_val);
  }
  else if (intercept.r > 0.5)
  {
    delay_val = mapit(intercept.r, 0.5, 1.5, 220, 50);
    base.move(backward, delay_val);
  }
  else if (intercept.r < -0.5)
  {
    delay_val = mapit(intercept.r, -0.5, -1.5, 220, 50);
    base.move(forward, delay_val);
    Serial.println(delay_val);
  }
  else
  {
    base.move(brake);
  }
}

//Custom Map Function
float mapit(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
