#define IN1 12
#define IN2 13
char getStr;
bool isRunning;

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  isRunning = true;
  digitalWrite(IN2,HIGH);
}

void loop() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    delayMicroseconds(20);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    delayMicroseconds(20);
}