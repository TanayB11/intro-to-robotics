#define straightRight 250   // tune to get car straight
#define straightLeft 250   // 0 - 255
// but low values won't move the motors at all


#define ENA 5       // enable right side
#define ENB 11      // enable left side
#define IN1 6       // motor H-bridge controls
#define IN2 7
#define IN3 8
#define IN4 9
#define bumperSwitch 10 

void fwd() {              
  analogWrite (ENA, 0);   
  analogWrite (ENB, 0);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);       //right wheel fwd
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);  // left wheel fwd
  analogWrite(ENA, straightRight);    //turn on with appropriate PWM
  analogWrite(ENB, straightLeft);
}

void back() {                   //go full speed bk
  analogWrite (ENA, 0);   
  analogWrite (ENB, 0);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN1, LOW);      
  digitalWrite(IN4, LOW);
  digitalWrite(IN3, HIGH);       // left wheel fwd
  analogWrite(ENA, straightRight);
  analogWrite(ENB, straightLeft);
}


void turnL() {
  analogWrite (ENA, 0);   
  analogWrite (ENB, 0);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);       //right wheel fwd
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);       // left wheel bk
  analogWrite(ENA, straightRight);
  analogWrite(ENB, straightLeft);
}

void turnR() {
  analogWrite (ENA, 0);   
  analogWrite (ENB, 0);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN1, LOW);       // left wheel fwd
  digitalWrite(IN4, HIGH);
  digitalWrite(IN3, LOW);       // right wheel bk
  analogWrite(ENA, straightRight);
  analogWrite(ENB, straightLeft);
}


void halt() {
  analogWrite (ENA, 0);   
  analogWrite (ENB, 0);
}


void setup() {
  int allTime;
  Serial.begin(9600);     // start serial port, 9600 baud
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ENA, OUTPUT); 
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  bool isTurned = true;
  do {
      // moves forward until switch is hit
      do{
        fwd();
        delay(5);
      } while (digitalRead(bumperSwitch) == 1);{ //FLAG
        delay(50);
        halt();
      }
      //decides turn
      if (isTurned == true){
        turnL();
        delay(300);
        halt();
        isTurned = false;
      }
      else{
        turnR();
        delay(300);
        halt();
        isTurned = true;
      }
  } while (true);
}
  

//void loop() {
  // put your main code here, to run repeatedly:

//}



