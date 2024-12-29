//this example uses an ultrasonic sensor facing the bottom to detect and avoid the edge

//pins for ultrasonic sensor
const int TRIG = A0, ECHO = A1;

//pins for motor
const int ENA = 3, IN1 = 2, IN2 = 4, ENB = 5, IN3 = 6, IN4 = 7;

int speed = 100;
const double threshold = 4;
long duration = 0;
double distance = 0;
int i = 0;

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);
}


void pulse() {
  digitalWrite(TRIG, LOW);
  delay(5);
  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);
}


void forwards() {
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}


void backwards(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}


void left(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}


void right(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}


void stop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}


void loop() {
  // put your main code here, to run repeatedly:
  pulse();
  
  duration = pulseIn(ECHO, HIGH);

  distance = duration/2 * 0.0343;

  if (distance < 4) {
    forwards();
  }  
  
  else {
    stop();
    backwards(150);
    delay(1000);
    if (i == 0) {
      right(150);
      delay(300);
      i = 1;
    } else if (i == 1) {
      left(150);
      delay(200);
      i = 0;
    }
  }

  delay(5);
}
