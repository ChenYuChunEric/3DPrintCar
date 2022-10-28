/*
   Gamepad module provides three different mode namely Digital, JoyStick and Accerleometer.

   You can reduce the size of library compiled by enabling only those modules that you want to
   use. For this first define CUSTOM_SETTINGS followed by defining INCLUDE_modulename.

   Explore more on: https://thestempedia.com/docs/dabble/game-pad-module/
*/
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>
#include <Servo.h>

//建立Servo
Servo Right;
Servo Left;
Servo Hand;

//360Servo停止值
int MG_stop = 90;

//90Servo數值
int Handpos = 10;

int delaytime = 50;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);      // make sure your Serial Monitor is also set at this baud rate.
  Dabble.begin("EricESP32");       //set bluetooth name of your device

  // Servo賦予Pin
  Right.attach(21);
  Left.attach(22);
  Hand.attach(23);

  // Servo初始化設定
  Right.write(MG_stop);
  Left.write(MG_stop);
  Hand.write(Handpos);
}

void loop() {
  Dabble.processInput();             //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
  //Serial.print("KeyPressed: ");
  if (GamePad.isUpPressed())
  {
    //Serial.print("Up");
    Right.write(0);
    Left.write(180);
    delay(delaytime);
  }
  if (GamePad.isDownPressed())
  {
    //Serial.print("Down");
    Right.write(180);
    Left.write(0);
    delay(delaytime);
  }
  if (GamePad.isLeftPressed())
  {
    //Serial.print("Left");
    Right.write(60);
    Left.write(60);
    delay(delaytime);
  }
  if (GamePad.isRightPressed())
  {
    //Serial.print("Right");
    Right.write(120);
    Left.write(120);
    delay(delaytime);
  }
  if (GamePad.isTrianglePressed())
  {
    //Serial.print("Triangle");
    if(Handpos <= 180){
      Handpos+=2;
    }
    if(Handpos >= 180){
      Handpos = 180;
    }
    Hand.write(Handpos);
    delay(delaytime);
  }
  if (GamePad.isCrossPressed())
  {
    //Serial.print("Cross");
    if(Handpos >= 10){
      Handpos-=2;
    }
    if(Handpos <= 10){
      Handpos = 10;
    }
    Hand.write(Handpos);
    delay(delaytime);
  }
  if (GamePad.isStartPressed())
  {
    Serial.print("Start");
    Right.write(MG_stop);
    Left.write(MG_stop);
    Hand.write(0);
  }
  if (GamePad.isSelectPressed())
  {
    Serial.print("Select");
  }
  else{
    Right.write(MG_stop);
    Left.write(MG_stop);
    Hand.write(Handpos);
  }
  Serial.println(Handpos);
  //Serial.print('\t');
}
