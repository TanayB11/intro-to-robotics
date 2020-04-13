  //Ultrasonic Ranger
  
  #define ECHO A4
  #define TRIG A5
  
  #include <Servo.h> 
  Servo rangerServo;

  // function prototypes
  void allStop();
  void forward();
  void back();
  void right();
  void left();
  
  int findRange(){  //Dist in cm
    int range;
    digitalWrite(TRIG,LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(20);
    digitalWrite(TRIG, LOW);
    range = pulseIn(ECHO,HIGH); //Pulse width measure in difference of microseconds
    range = range/58;
    return(range);
    delay(500);
  }
  
  void setup(){
    Serial.begin(9600);
    for (int c=0;c<10;c++){
      int servoR = 20;
      int servoL = 180;
      int servoC = 110;
      pinMode(ECHO,INPUT);
      pinMode(TRIG, OUTPUT);
      rangerServo.attach(3);
      rangerServo.write(servoL);
      delay(1000);
      Serial.println(findRange());
      rangerServo.write(servoR);
      delay(1000);
      Serial.println(findRange());
      rangerServo.write(servoC);
      delay(1000);
      Serial.println(findRange());
    }
  }
  
  void loop(){
  }