//Calibrate right turn
/*  Does 8 consecutive right turns.  Use it to fine tune RIGHTTURNTIME (8 consecutive turns
will exaggerate any error in your 90 degree turn).
*/
//CF 9/30/17
#define DEBUG                 //comment this line (only) out to get rid of debug code
#ifdef DEBUG
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
  #define DEBUG_PRINTDEC(x) Serial.print(x,DEC)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTDEC(x)
#endif

#define ENA 5                 // pins to enable drives  ENA = right side
#define ENB 11                // ENB - left side
#define IN1 6                 // H-bridge pins
#define IN2 7                 // IN1, IN2 are right side
#define IN3 8                 // IN3, IN4 are left
#define IN4 9

// the following are for getting your car to go straight in forward()
#define RIGHTSLOWDOWN 0       // a number here slows down the right wheels
#define LEFTSLOWDOWN 0        // a number here slows down the left wheels
// times for right and left turns:
#define RIGHTTURNTIME 240  // time for a 90 degree turn
#define LEFTTURNTIME 235
#define ONEBLOCKTIME 370      // time for one block forward

// function prototypes
void allStop();
void forward();
void back();
void right();
void left();
void forwardOneBlock();
void right90();
void left90();

void setup(){
  int i;                      // array index
  
  Serial.begin(9600);
  //  the following code is to calibrate RIGHTTURNTIME
  for (i=0; i<5; i++) {
    forwardOneBlock90();
    delay(1000);
  }
}

void loop() {
  //empty loop
}

void right90() {
  right();
  delay(RIGHTTURNTIME);
  allStop();
}
  
void left90() {
  left();
  delay(LEFTTURNTIME);
  allStop();
}
  
void forwardOneBlock() {
  forward();
  delay(ONEBLOCKTIME);
  allStop();
}
  
void forward(){
  DEBUG_PRINTLN("In forward()");
  analogWrite(ENA, 255-RIGHTSLOWDOWN);
  analogWrite(ENB, 255-LEFTSLOWDOWN);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void back(){
  DEBUG_PRINTLN("In back()");
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void left(){
  DEBUG_PRINTLN("In left()");
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW); 
}

void right(){
  DEBUG_PRINTLN("In right()");
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
} 

void allStop(){
  DEBUG_PRINTLN("In allStop()");
   digitalWrite(ENA, LOW);
   digitalWrite(ENB, LOW);
} 

