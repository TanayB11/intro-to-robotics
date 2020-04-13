//Stored Moves - Tanay Biradar
//Partnered with Veer Doshi

#define ARRAYLEN 12          // # entries in recording array (each entry is 2 bytes)
#define MOVETYPE 0            // indexes into moveList[] array
#define MOVETIME 1

#define CARSTOP 0             // literals for movetype in array 
#define CARFWD 1
#define CARLEFT 2
#define CARRIGHT 3
#define CARBACK 4
#define ENDOFLIST 5

#define LEFTTURNTIME 245    //Set according to calibration
#define RIGHTTURNTIME 235

#define ENA 5                 // pins to enable drives  ENA = right side
#define ENB 11                // ENB - left side
#define IN1 6                 // H-bridge pins
#define IN2 7                 // IN1, IN2 are right side
#define IN3 8                 // IN3, IN4 are left
#define IN4 9

// the following are for getting your car to go straight in forward()
#define RIGHTSLOWDOWN 0       // a number here slows down the right wheels
#define LEFTSLOWDOWN 0        // a number here slows down the left wheels

/* global variables */
//Move List: parameters are the direction and distance of move/turn. Calibrated.
unsigned int moveList[ARRAYLEN][2] = {
  {CARFWD,385},
  {CARLEFT,LEFTTURNTIME},
  {CARFWD,385},
  {CARRIGHT,RIGHTTURNTIME},
  {CARFWD,805},
  {CARLEFT,LEFTTURNTIME},
  {CARFWD,385},
  {CARRIGHT,RIGHTTURNTIME},
  {CARFWD,805},
  {CARLEFT,LEFTTURNTIME},
  {CARFWD,395},
  {ENDOFLIST, 0}
};

// function prototypes
void allStop();
void forward();
void back();
void right();
void left();

//Main logic
void setup(){
  Serial.begin(9600);
  //Goes through moveList, executing the moves in the move list until ENDOFLIST
  for (int i=0; i<ARRAYLEN; i++) {
    Serial.println(moveList[i][MOVETYPE]);
    Serial.println(moveList[i][MOVETIME]);
    if (moveList[i][MOVETYPE] == CARFWD) {
      forward();
      delay(moveList[i][MOVETIME]);
    } else if (moveList[i][MOVETYPE] == CARLEFT) {
      left();
      delay(moveList[i][MOVETIME]);
    } else if (moveList[i][MOVETYPE] == CARRIGHT) {
      right();
      delay(moveList[i][MOVETIME]);
    } else if (moveList[i][MOVETYPE] == ENDOFLIST) {
      allStop();
      i=ARRAYLEN;
      break;
    }
    
  }
  delay(1000);
}

void loop() {
  //empty loop
}

//Drive functions below
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
