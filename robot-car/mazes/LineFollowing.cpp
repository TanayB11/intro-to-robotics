//Commented by CF 9/8/17

#define LT1 digitalRead(10)   // Line Tracking sensor #1 (left side)
#define LT2 digitalRead(4)    // Line Tracking sensor #2 (center)
#define LT3 digitalRead(2)    // Line Tracking sensor #3 (right side)

#define ENA 5                 // pins to enable drives  ENA = right side
#define ENB 11                // ENB - left side
#define IN1 6                 // H-bridge pins
#define IN2 7                 // IN1, IN2 are right side
#define IN3 8                 // IN3, IN4 are left
#define IN4 9

#define ABS 100               // absolute speed?  This number sets speed: 127=max, 0=stop
                              // in practice a value of 64 barely moves the car

void forward(){
  analogWrite(ENA, ABS);      // PWM (pulse-width modulated) enable lines
  analogWrite(ENB, ABS);      // makes it go less than full speed (by a factor of ABS/127)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("go forward!");
}

void back(){
  analogWrite(ENA, ABS);      // PWM (pulse-width modulated) enable lines
  analogWrite(ENB, ABS);      // makes it go less than full speed (by a factor of ABS/127)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("go back!");
}

void left(){
  analogWrite(ENA, ABS);      // PWM (pulse-width modulated) enable lines
  analogWrite(ENB, ABS);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW); 
  Serial.println("go left!");
}

void right(){
  analogWrite(ENA, ABS);      // PWM (pulse-width modulated) enable lines
  analogWrite(ENB, ABS);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("go right!");
} 

void allStop(){
   digitalWrite(ENA, LOW);
   digitalWrite(ENB, LOW);
   Serial.println("Stop!");
} 

void setup(){
  Serial.begin(9600);
}

void loop() {
  if(LT2){          // if center sensor over line
    forward();      // go forward
  }
  else if(LT1) {    // if left sensor over line
    left();         // go left
    while(LT1);     // until left sensor not over line                     
  }   
  else if(LT3) {    // if right sensor over line
    right();        // go right
    while(LT3);     // until right sensor not over line
  }
  else{
    left();
    while(LT1);
  }
  //else allStop();
}

