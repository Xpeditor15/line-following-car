//this example uses 3 ultrasonic sensors HC-SR04 on top to detect and follow the object

//motor pins
const int ENA = 3, IN1 = 2, IN2 = 4, ENB = 5, IN3 = 6, IN4 = 7;

//top ultrasonic sensor
const int t_trig = 10, t_echo = 11;

//right ultrasonic sensor
const int r_trig = 13, r_echo = 12;

//left ultrasonic sensor
const int l_trig = 8, l_echo = 9;

int speed = 130;

int initial = 0;

int mode = 0;

const double l_threshold = 5;
const double h_threshold = 25;
const double lost = 40;
double distance;
long duration;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(t_trig, OUTPUT);
  pinMode(t_echo, INPUT);
  pinMode(r_trig, OUTPUT);
  pinMode(r_echo, INPUT);
  pinMode(l_trig, OUTPUT);
  pinMode(l_echo, INPUT);
  pinMode(b_trig, OUTPUT);
  pinMode(b_echo, INPUT);

  Serial.begin(9600);
}


void pulse(int trig, int echo) {
  digitalWrite(trig, LOW);
  delay(5);
  digitalWrite(trig, HIGH);
  delay(10);
  digitalWrite(trig, LOW);
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


double calc_distance(int echo) {
  long time = pulseIn(echo, HIGH);
  double distance = time/2*0.0343;
  return distance;
}


void search() {
  int direction = 0;
  int i = 0;
  int limit = 10;
  while (1) {
    pulse(l_trig, l_echo);
    double l_distance = calc_distance(l_echo);
    pulse(t_trig, t_echo);
    double t_distance = calc_distance(t_echo);
    pulse(r_trig, r_echo);
    double r_distance = calc_distance(r_echo);

    if (t_distance <= lost) break;

    if (l_distance <= lost) {
      left(150);
      delay(5);
    } else if (r_distance <= lost) {
      right(150);
      delay(5);
    } else if (l_distance > lost && t_distance > lost && r_distance > lost) {
      if (direction == 0) {
        left(150);
        delay(10);
        direction = 1;
        i += 5;
      } else if (direction == 1) {
        right(150);
        delay(10);
        direction = 0;
        i += 10;
      }
    }
  }
}


void loop() {
  pulse(t_trig, t_echo);
  distance = calc_distance(t_echo);

  if (distance > lost) {
    pulse(l_trig, l_echo);
    double l_distance = calc_distance(l_echo);
    pulse(r_trig, r_echo);
    double r_distance = calc_distance(r_echo);

    if (l_distance <= lost) {
      left(150);
      delay(10);
    } else if (r_distance <= lost) {
      right(150);
      delay(10);
    } else search();
  } else {
    forwards(100);
    delay(5);
  }
}