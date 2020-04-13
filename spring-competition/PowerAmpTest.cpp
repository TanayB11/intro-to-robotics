//Code base by CF
//Tanay Biradar, Ashvin Iyer, Krish Prasad
//Modified 3.1.19

//Change MYCARNAME to the name of your car in the line that follows:
String BlueToothIDCmd = String("AT+NAME=SOMETHING");
int LED=13;
volatile int state = LOW;

//Only set up for one poweramp, add poweramp2 pinout later, see wire diagram
#define IN1 2 //backward right
#define IN2 3 //forward left
#define IN3 4 //forward right
#define IN4 5 //backward left
#define ENA 13
#define IN5 6
char getStr;
//int ENB = 13;


void fwd(byte pwmValue) {                // forward at 0 <= pwmValue <= 255
  digitalWrite(IN2, LOW); //255-pwm
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); //pwm
}
void rev(byte pwmValue) {                // reverse at 0<=pwmValue<=255
  // pwmValue = pwmValue;             // reverse the bits to get 255-pwmValue
  digitalWrite(IN2, HIGH); //pwm
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, HIGH);
  analogWrite(IN4, LOW); //255 - pwm
}
void left(byte pwmValue){
  digitalWrite(IN2, HIGH); //pwm
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); //pwm
}
void right(byte pwmValue){
  digitalWrite(IN2, LOW); //255 - pwm
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW); //255 - pwm
}
void stopCar(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

/*void closeFlap(byte pwmValue){
  analogWrite(IN5, pwmValue);
  delay(20);
  digitalWrite(IN5, LOW);
}
void openFlap(byte pwmValue){
  analogWrite(IN5, pwmValue);
  delay(60);
  digitalWrite(IN5, LOW);
}*/
/***************************************************************/
void setup(){
  Serial.begin(9600);
  Serial.println("in teleop");
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  digitalWrite(ENA,HIGH);
  // Serial.println(BlueToothIDCmd);
}

void loop(){
  getStr = Serial.read();
  Serial.println(getStr);
  if(getStr == 'f'){
    fwd(200);
  }
  else if(getStr=='b'){
    rev(200);
  }
  else if(getStr=='l'){
    left(200);
  }
  else if(getStr=='r'){
    right(200);
  }
  else if(getStr =='s'){
    stopCar();
  }
  /*
  if(getstr == 'o'){
    openFlap();
  }
  else if(getstr == 'c'){
    closeFlap();
  }*/
}