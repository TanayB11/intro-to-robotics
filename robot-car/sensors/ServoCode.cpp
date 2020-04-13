//Servo calibration for Elegoo car
//CF 9/25/17
/*
Moves the servo from zero to 180 degrees, then finally to 90 degrees (straight ahead)
Calibration R: 20
            L: 180
            S: 110
*/
  //Servo calibrated values
  int servoR = 20;
  int servoL = 180;
  int servoC = 110;

#include <Servo.h> 

Servo rangerServo;

void setup() 
{ 
  rangerServo.attach(3);

  for (int i=1; i<=90; i++) {
    rangerServo.write(90+i);
    delay(20);
  }
  for (int i=1; i<=90; i++) {
    rangerServo.write(90-i);
    delay(20);
  }
  rangerServo.write(servoC);
} 

void loop() { /* empty loop */ } 
