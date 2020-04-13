/*
  CF 2/1/19

  Reference Timing Code:
  Creates a time-controlled state transition from AUTON to DELAYED, to TELEOP, to GAMEOVER
  based on defined limits.  Communication from the interrupt-driven timer to the operational
  code is via the currentState variable in software and the MOTORENABLE (pin 13) in hardware.
  Variables in the code that affect fair play of the game and should therefore not be modified
  by users are marked with a "DNM" (Do Not Modify) acronym.

  Uses a single PWM pin to control motors by using a (non-pwm) pin as the direction bit, and
  the pwm pin as either 0-255 for one direction, or 255-0 for the other.
  
  Please keep the #defines for the timer limits at the very top of your code
*/
/********************************************************************************/
#include <avr/interrupt.h>  // used for interrupts

#define autonTimeLimit 15000      // at 30 seconds auton ends (DNM)
#define delayTimeLimit 30000      // at 90 seconds delay ends (delay is 60) (DNM)
#define teleopTimeLimit 150000    // at 210 seconds teleop ends (teleop is 120) (DNM)
#define releaseDuration 250      // duration (in ms) of gate open in response to switch (DNM)
#define MOTORENABLE 13            // turns on/off all motors based on state (DNM)
#define AUTON 0                   // values for currentState (DNM):
#define DELAYED 1                 // DNM
#define TELEOP 2                  // DNM
#define GAMEOVER 3                // DNM
char getstr;

// global variables:
volatile unsigned long currentTime;          // time since reset in ms (DNM)
volatile byte currentState;                  // current game time period (DNM)
/*****************************************************************************/


// #define ENA 13       // enable for right side of car
// #define ENB 13     // enable for left side of car
#define IN1 2       // left side back
#define IN2 3       // left side fwd
#define IN3 4       // right side fwd
#define IN4 5       // right side back
// #define IN5 6      // flap fwd
// #define IN6 7      // flap back

String BlueToothIDCmd = String("AT+NAME=SERPENTINE");
void setup() {
  pinMode(MOTORENABLE, OUTPUT);       // turns on/off all motors (DNM)
  digitalWrite(MOTORENABLE, LOW);     // turn off motors (DNM)
  Serial.begin(9600);                 // initialize serial port
  Serial.println();
  Serial.println("Ball Game test software starting up...");
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  currentTime = 0;            // time in ms (DNM)
  currentState = AUTON;       // starts in auton mode (DNM)
  setUpTimer();               // initial timer2 for 1ms irqs (DNM)
  /* PUT YOUR CODE HERE */
  if (currentState == AUTON)
    autonomous(200);
  else if (currentState == DELAYED)
    ;     // do nothing in delay period
  else if (currentState == TELEOP)
    teleoperated(128);
  else if (currentState == GAMEOVER)
    Serial.print(BlueToothIDCmd); // do nothing at end of game
  
  
  // pinMode(IN5, OUTPUT);
  // fwd(255);
}

void loop() {
  byte cntr = 0;
  if (currentState == AUTON)
    autonomous(cntr);
  else if (currentState == DELAYED)
    ;     // do nothing in delay period
  else if (currentState == TELEOP)
    teleoperated(cntr);
  else if (currentState == GAMEOVER)
    ;     // do nothing at end of game
  cntr++;
}

void autonomous(byte val) {
  Serial.println("in autonomous");
  while(currentTime < 600){
      fwd(200); 
  }
  stopCar();
  /* PUT YOUR CODE HERE */
            // go forward at fractional speed
}
void teleoperated(byte val) {
  Serial.println("in teleop");
  getstr=Serial.read();
  if(getstr=='f')
  {
    fwd(200);
  }
  else if(getstr=='b')
  {
    rev(255);
  }
  else if(getstr=='l')
  {
    left(255);
  }
  else if(getstr=='r')
  {
    right(150);
  }
  else if(getstr=='s')
  {
    stopCar();     
  }
  // if(getstr == 'o'){
  //   openFlap();
  // }
  // else if(getstr == 'c'){
  //   closeFlap();
  // }
}

void rev(byte pwmValue) {                // forward at 0<=pwmValue<=255
  // pwmValue = pwmValue;             // reverse the bits to get 255-pwmValue
  analogWrite(IN2, pwmValue);
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, HIGH);
  analogWrite(IN4, 255 - pwmValue);
}
void fwd(byte pwmValue) {                // reverse at 0<=pwmValue<=255
  digitalWrite(IN1, HIGH);
  analogWrite(IN2, 255 - pwmValue);
  digitalWrite(IN3, LOW);
  analogWrite(IN4, pwmValue);
}
void left(byte pwmValue){
  analogWrite(IN2, pwmValue);
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  analogWrite(IN4, pwmValue);
}
void right(byte pwmValue){
  analogWrite(IN2, 255 - pwmValue);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, HIGH);
  analogWrite(IN4, 255 - pwmValue);
}
void stopCar(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
// void closeFlap(byte pwmValue){
//   analogWrite(IN5, pwmValue);
//   delay(20);
//   digitalWrite(IN5, LOW);
// }
// void openFlap(byte pwmValue){
//   analogWrite(IN5, pwmValue);
//   delay(60);
//   digitalWrite(IN5, LOW);
// }
/***********************************************/
ISR(TIMER2_COMPA_vect) {   // DO NOT MODIFY
  currentTime++;                        // increment time in ms
  if (currentTime < autonTimeLimit) {   // update currentState and motor enable
    currentState = AUTON;
    digitalWrite(MOTORENABLE, 1);
  }     // turn on motors
  else if (currentTime < delayTimeLimit) {
    currentState = DELAYED;
    digitalWrite(MOTORENABLE, 0);
  }     // turn off motors
  else if (currentTime < teleopTimeLimit) {
    currentState = TELEOP;
    digitalWrite(MOTORENABLE, 1);
  }
  else {
    currentState = GAMEOVER;
    digitalWrite(MOTORENABLE, 0);
  }
} 

/***********************************************/

/***********************************************/
void setUpTimer() {         // DO NOT MODIFY
  // most of this taken from "amandaghassaei" on instructables
  cli();// interrupts
  //set timer2 interrupt at 1kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 1khz increments
  OCR2A = 249;// = (16*10^6) / (1000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS22, CS21 and CS00 bits for 256 prescaler
  TCCR2B |= (1 << CS22);   // should be setup for 64 prescaler, but this works...
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
  sei();//allow interrupts
} /***************************************************/

