//Calibrate one block move
/*  Does 5 consecutive one block moves.  Use it to fine tune RIGHTSLOWDOWN and LEFTSLOWDOWN
to make the car go straight (5 consecutive moves will exaggerate any error in straightness).
Use it also to fine tune ONEBLOCKTIME: again, 5 consecutive moves will exaggerate any error
in the length of a one block move.
*/
//CF 9/30/17

#define ENA 5                 // pins to enable drives  ENA = right side
#define ENB 11                // ENB - left side
#define IN1 6                 // H-bridge pins
#define IN2 7                 // IN1, IN2 are right side
#define IN3 8                 // IN3, IN4 are left
#define IN4 9

// the following are for getting your car to go straight in forward()
#define RIGHTSLOWDOWN 18      // a number here slows down the right wheels
#define LEFTSLOWDOWN 0      // a number here slows down the left wheels
// times for right and left turns:
#define RIGHTTURNTIME 230     // time for a 90 degree turn
#define LEFTTURNTIME 240
#define ONEBLOCKTIME 400     // time for one block forward

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
  for (i=0; i<5; i++) {
    forwardOneBlock();
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
  analogWrite(ENA, 255-RIGHTSLOWDOWN);
  analogWrite(ENB, 255-LEFTSLOWDOWN);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void back(){
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void left(){
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW); 
}

void right(){
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
} 

void allStop(){
   digitalWrite(ENA, LOW);
   digitalWrite(ENB, LOW);
} 

