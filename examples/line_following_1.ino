//this example does not use the MPU6050 and is only able to follow the black-line path on flat grounds

#include <LiquidCrystal.h>
#include <PinChangeInterrupt.h>

//LCD pins
const int RS = 8, EN = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7, BL = 10;

//Motor pins
const int ENA = 2, IN1 = 12, IN2 = 2, ENB = 11, IN3 = 3, IN4 = 13;

//Encoder rotary pins
const int sl = A2

//IR sensor pins
const int irl = A3, irr = A1;

//Counter for left encoder
double count_l = 0;

LiquidCrystal lcd(RS, EN, d4, d5, d6, d7);

void counter() {
  count_l += 1;
}

void setup() {
  lcd.begin(16, 2);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A1, INPUT);

  attachPCINT(10, counter, RISING);
}


void forwards() {
  analogWrite(ENA, 130);
  analogWrite(ENB, 130);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}


void left() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}


void right() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}


void end() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}


void process_direction() {
  int output_left = digitalRead(irl);
  int output_right = digitalRead(irr);
  if (output_left == 0 && output_right == 0) {
    forwards();
    delay(5);
  } 

  else if (output_left == 0 && output_right == 1) {
    left();
    delay(60);
  }

  else if (output_left == 1 && output_right == 0) {
    right();
    delay(60);
  }

  else if (output_left == 0 && output_right == 0) {
    end();
  }

  delay(50);
}


void loop() {
  double var_1 = count_l/20;
  double var_2 = var_1*2*3.242*6.5;
  lcd.setCursor(0, 0);
  lcd.print(var_2, 3);
  lcd.setCursor(0, 1);
  lcd.print(count_l, 0);
  process_direction();
}