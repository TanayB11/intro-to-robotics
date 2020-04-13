//Servo calibration for Elegoo car
//CF 9/25/17
/*
Moves the servo from zero to 180 degrees, then finally to 90 degrees (straight ahead)
*/
#include <Servo.h> 

Servo rangerServo;

void setup() 
{ 
  rangerServo.attach(3);
  rangerServo.write(20);       // set servo to point to 10 degrees (left)
  delay(1000);
  rangerServo.write(180);     //point 180 degrees (right)
  delay(1000);
  rangerServo.write(110);      //point 90 degrees (straight ahead)
} 

void loop() { /* empty loop */ } 
