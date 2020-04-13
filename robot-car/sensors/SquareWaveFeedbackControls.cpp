/*
  CF 9/15/18
  Demonstrates the variables and arithmetic for PID
  Makes the setpoint a square wave.
  Connect to motorized gearbox through L298 power amp
  Feedback on A4 pin (labelled "ECHO" on ultrasonics ranger connector) is an analog potentiomter
  Output is via pins 6 & 7 through the L298 power amp
  Connect to output #1 on the L298 board
*/
#include <avr/interrupt.h>
#define FDBKPIN A4          // connect analog potentiometer for feedback (ECHO pin on elegoo car)
#define ENA 5               // enable for L298 power amp
#define ENB 11              // enable for L298 second channel (not used)
#define MOTORPIN 6          // drive pins for L298 6 and 7 are used
#define IN2 7
#define MYLED 13            // output pin to show that irqs are working (should have 1kHz sq wave)

int cmdPos;                 // commanded position (setpoint) in ADC counts
int error;                  // servo error
int prevPos;                // for derivative gain, used to calc delta feedback or delta error
float effort;               // servo output
long int accumulator;       // for integral gain, accumulated error
int actPos;                 // actual position in ADC counts (feedback value, 10 bits)
int upperLimit;             // upper limit of setpoint square wave
int lowerLimit;             // lower limit of setpoint square wave
long int mSec;              // current # ms since last setpoint sq wave transition
long int mSecLimit;         // time per half cycle of sq wave in ms
float PGain;                // proportional gain
float IGain;                // integral gain
float DGain;                // derivative gain
boolean rampUp;             // true means ramping up (sq wave is at upperLimit), false means down
volatile boolean irqSet;    // global variable used to detect irqs
boolean checker;

void setup() {
  pinMode(MOTORPIN, OUTPUT);       // H-bridge drives
  pinMode(IN2, OUTPUT);
  pinMode(FDBKPIN, INPUT);
  pinMode(ENA, OUTPUT);       // H bridge enables
  pinMode(ENB, OUTPUT);
  pinMode(MYLED, OUTPUT);
  Serial.begin(9600);         // initialize serial port
  Serial.println();
  Serial.println("Basic feedback controls with square wave setpoint");
  digitalWrite(ENA, HIGH);    // enable H bridge
  digitalWrite(ENB, LOW);     // we only use the A side

  rampUp == true;             // start upward
  lowerLimit = 0;           // setpoint limits, default 200 and 800
  upperLimit = 700;
  mSecLimit = 8000;           // time for rampUp/down, CHANGE TO 16000!
  accumulator = 0;            // for integral action
  error = 0;                  // closed loop error for PID to act on
  prevPos = 0;              // for derivative action
  PGain = 3;                 // 1 works well, 4 is slightly underdamped
  IGain = 0.00;             //Integral gain
  DGain = 0.00;             //Derivative gain
  resetPos();
  setUpTimer();               // initial timer2 for 1ms irqs
}

void resetPos(){
  double actPosCheck = analogRead(FDBKPIN);           // read feedback (takes about 100us)
  if (actPosCheck > lowerLimit){
    analogWrite(ENA, -128);
  }
  else if (actPosCheck<lowerLimit){
    analogWrite(ENA,128);
  }
}
 
void loop() {
  // wait here for ms timer
  if (irqSet == true)                     // wait for irq: it should not be set here
    Serial.println("Irq overrrun");
  while (irqSet == false) {               // wait for irq
    // do nothing (wait)
  }

  // at this point, irq has just happened
  irqSet = false;                        // reset irq flag
  
  mSec++;                                  // another ms elapsed
  if (mSec >= mSecLimit) {                // create square wave for setpoint
    mSec = 0;                             //Reset mSec
    rampUp=!rampUp;                       //Negate rampUp
  }
  
  //Error checking and detection
  if (mSec >= 1000){           //Delay in checking because error is high to begin with
    if (error>=400 || error<=-400){      //Error can be +/-
      digitalWrite(ENA,0);    //Turn off pin power
      digitalWrite(ENB,0);
    }
  }
  
  //Triangle wave: pwr with respect to time
  if (rampUp==true){
    cmdPos=(lowerLimit+(mSec*(upperLimit-lowerLimit)/mSecLimit));
  }
  else{
    cmdPos=(upperLimit-(mSec*(upperLimit-lowerLimit)/mSecLimit));
  }
  
  // close the loop:
  actPos = analogRead(FDBKPIN);           // read feedback (takes about 100us)
  error = cmdPos - actPos;                // calc error
  accumulator += error;                   // integrate the error
  effort = PGain * error;                 // proportional gain
  effort += IGain * accumulator;          // integral gain
  effort += DGain * (prevPos - actPos);   // derivative gain (derivative on feedback)
  putOut(effort);                         // convert effort to PWM
  // housekeeping
  prevPos = actPos;                       // for derivative gain (derivative on feedback)
  //accumulator is handled before gains are applied
}

ISR(TIMER2_COMPA_vect) {
  static boolean toggle;
  if (toggle)                     // toggle the LED bit to indicate irqs are working
    digitalWrite(MYLED, true);
  else
    digitalWrite(MYLED, false);
  toggle = !toggle;
  irqSet = true;
}

void setUpTimer() {         // most of this taken from "amandaghassaei" on instructables
  cli();//stop interrupts
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
}

void putOut(float driveValue) {
  byte effortOut;             // servo output formatted for PWM (sign and magnitude)

  if (driveValue > 225) {               // saturate at 255
    driveValue = 255;
  }
  else if (driveValue < -255) {
    driveValue = -255;
  }
  // format for PWM
  if (driveValue >= 0) {                // analogWrite(IN1, effort);
    effortOut = byte(driveValue);
    analogWrite(MOTORPIN, effortOut);
    digitalWrite(IN2, LOW);
  }
  else {
    effortOut = byte(-driveValue);
    analogWrite(MOTORPIN, ~effortOut);
    digitalWrite(IN2, HIGH);
  }
}