//(c)S1NIS73R
//#define dbug
//#define dbug_aux
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
RF24 radio(7, 8); //(CE,CSN)
struct packet
{
  float y, p, r;
  int fx, fy, fz; //Flex1,Flex2,Flex3
} intercept;

//GCRA
GCRA arm;

//GCRABASE
GCRABASE base;
int angle_buffer_1 = 100;
int delay_val;

//Switches
boolean current_mode = 0;
boolean fx_switch = 0; //Felx1Switch
boolean fy_switch = 0; //Felx2Switch
boolean fz_switch = 0; //Felx3Switch
int arm_mode = 0;

//Switching Timers/Requisites
boolean start = 1, wait_val_1 = 0, wait_val_2 = 0;
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
  Serial.println("Mode,ArmMode,Flex1Switch,Flex2Switch,Flex3Switch,Time,Yaw,Pitch,Roll,Flex1,Flex2,Flex3");
#else
  Serial.println("Yaw,Pitch,Roll,Flex1,Flex2,Flex3");
#endif

  //nRF24L01+
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);
  radio.setAutoAck(1);
  radio.setCRCLength(RF24_CRC_8);

  //GCRA
  arm.cfg(1, 0, 5);
  arm.servocfg(550, 2400, 550, 2200);
  arm.init(100, 20, 25, 10, 50, 96, 15);

  //RGB Led
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  //Start RGB Led
  rgbled(0);
  digitalWrite(12, HIGH);

  //Notify - Waiting for connection
  bool test;
  do //Blink red to wait
    //Continuously blink red if not connected
  {
    rgbled(6);
    delay(100);
    radio.openReadingPipe(1, address);
    radio.startListening();
    test = radio.available();
    rgbled(0);
    delay(100);
  }
  while (!test);
  rgbled(1); //Blink green twice if connected
  delay(100);
  rgbled(0);
  delay(100);
  rgbled(1);
  delay(100);
  rgbled(0);
  delay(100);
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
    Serial.print(arm_mode); Serial.print(",");
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
  if (intercept.fx < 710) {
    fx_switch = 1;
  } else if (intercept.fx > 780) {
    fx_switch = 0;
  }
  if (intercept.fy < 680) {
    fy_switch = 1;
  } else if (intercept.fy > 750) {
    fy_switch = 0;
  }
  if (intercept.fz < 710) {
    fz_switch = 1;
  } else if (intercept.fz > 780) {
    fz_switch = 0;
  }

  //Base / Arm Mode Switching (folding index and middle finger simultaneously changes mode)
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
  arm.actuate(tool, mapit(intercept.fx, 720, 790, 90, 0));
  if (!current_mode) {
    mode1();
  } else {
    mode2();
  }

}

//Mode 1 Function
void mode1()
{
  //Arm Part Mode Switching
  if (fy_switch && !fz_switch && !wait_val_1)
  {
    wait_val_1 = 1;
  }

  if (wait_val_1 == 1)
  {
    if (!fy_switch && !fz_switch)
    {
      if (arm_mode < 6)
      {
        arm_mode++;
      }
      wait_val_1 = 0;
    }
  }

  if (!fy_switch && fz_switch && !wait_val_2)
  {
    wait_val_2 = 1;
  }

  if (wait_val_2 == 1)
  {
    if (!fy_switch && !fz_switch)
    {
      if (arm_mode > 0)
      {
        arm_mode--;
      }
      wait_val_2 = 0;
    }
  }

  rgbled(arm_mode);
  //Arm Part Switching
  int temp;
  if (!wait_val_1 && !wait_val_2)
  {
    switch (arm_mode)
    {
      case 1:

        arm.actuate(waist, mapit(intercept.y, 1, -1, 0, 180));
        break;

      case 2:
        temp = mapit(intercept.p, 1, -1, 40, 180);
        arm.actuate(shoulder, temp);
        arm.actuate(elbow, temp);
        break;

      case 3:
        arm.actuate(elbow, mapit(intercept.p, 1.2, -0.80, 160, 40));
        break;

      case 4:
        arm.actuate(wrist_p, mapit(intercept.p, 1, -1, 40, 180));
        break;

      case 5:
        arm.actuate(wrist_r, mapit(intercept.r, 1, -1, 10, 170));
        break;
    }
  }

}

//Mode 2 Function
void mode2()
{
  rgbled(7);
  if (intercept.r > 0.1)
  {
    delay_val = mapit(intercept.r, 0.1, 1.5, 255, 0);
    base.move(left, delay_val);
  }
  else if (intercept.r < -0.6)
  {
    delay_val = mapit(intercept.r, -0.6, -1.5, 255, 0);
    base.move(right, delay_val);
  }
  else if (intercept.p > 0.3)
  {
    delay_val = mapit(intercept.p, 0.3, 1.5, 255, 0);
    base.move(backward, delay_val);
  }
  else if (intercept.p < -0.3)
  {
    delay_val = mapit(intercept.p, -0.3, -1.5, 255, 0);
    base.move(forward, delay_val);
    Serial.println(delay_val);
  }
  else
  {
    base.move(brake);
  }
}

//RGB Led Color Function (note:pwm limited to green only by servo library)
void rgbled(int a)
{
  switch (a)
  {
    case 1:
      analogWrite(9, 255);
      analogWrite(11, 0);
      analogWrite(10, 255); //green
      break;
    case 2:
      analogWrite(9, 0);
      analogWrite(11, 235);
      analogWrite(10, 255); //orange
      break;
    case 3:
      analogWrite(9, 255);
      analogWrite(11, 255);
      analogWrite(10, 0); //blue
      break;
    case 4:
      analogWrite(9, 0);
      analogWrite(11, 255);
      analogWrite(10, 0); //purple
      break;
    case 5:
      analogWrite(9, 0);
      analogWrite(10, 0);
      analogWrite(11, 255); //lime green
      break;
    case 6:
      analogWrite(9, 0);
      analogWrite(11, 255);
      analogWrite(10, 255); //red
      break;
    case 7:
      analogWrite(9, 255);
      analogWrite(11, 0);
      analogWrite(10, 0); //cyan
      break;
    default:
      analogWrite(9, 255);
      analogWrite(10, 255);
      analogWrite(11, 255);
      break;
  }
}
//Custom Map Function
float mapit(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
