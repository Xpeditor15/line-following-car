//this example uses the HC-05 Bluetooth module to wirelessly control the car 

//motor pins
const int ENA = 3, IN1 = 2, IN2 = 4, ENB = 5, IN3 = 6, IN4 = 7;

//ultrasonic sensor
const int TRIG = 10, ECHO = 11;

//tx and rx pins for the bluetooth module
const int RX = 0, TX = 1;

//default speed of the car
int speed = 130;

int initial = speed;

int boost = 255;

const double threshold = 6.5;
double distance = 0;
long duration = 0;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.begin(9600);
}


void pulse() {
  digitalWrite(TRIG, LOW);
  delay(5);
  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);
}


void forwards(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
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
  if (Serial.available() > 0) {
    char command = Serial.read();

    switch (command) {
      case 'F': 
        forwards(speed);
        break;
      case 'B':
        backwards(speed);
        break;
      case 'L':
        left(speed);
        break;
      case 'R':
        right(speed);
        break;
      case 'S':
        stop();
        break;
      case 'M':
        speed = 255;
        break;
      case 'm':
        speed = 130;
        break;
      case 'X':
        speed += 10;
        break;
      case 'Y':
        speed -= 10;
        break;
      default:
        break;
    }
  }
}
