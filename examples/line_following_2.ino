//In this example, the MPU6050 is connected to the Arduino and the car is supposed to go up a ramp with a pitch angle of 25 degrees


#include <LiquidCrystal.h>
#include <PinChangeInterrupt.h>
#include <Wire.h>
#include <MPU6050_tockn.h>


//LCD pins
const int RS = 8, EN = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7, BL = 10;

//Motor pins
const int ENA = 3, IN1 = 12, IN2 = 1, ENB = 11, IN3 = 2, IN4 = 13;

//Encoder rotary pins
const int sl = A2;

//IR sensor pins
const int irl = A3, irr = A1;

double count = 0;

double x;
double max = 0;

int turn = 0;
int initial = 0;
int condition = 0;

//default speed of the car
int speed = 100;

int count_1 = 0;

LiquidCrystal lcd(RS, EN, d4, d5, d6, d7);

MPU6050 mpu6050(Wire);

//counter function for the rotary encoder
void counter() {
  count += 1;
}

void setup() {
  Wire.begin();
  mpu6050.begin(); //initializes the mpu
  mpu6050.calcGyroOffsets(true);

  lcd.begin(16, 2);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  attachPCINT(10, counter, RISING);
}


void forwards(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}


void right() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
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


void end() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}


void process_direction(int speed, int condition) {
  int o_left = digitalRead(irl);
  int o_right = digitalRead(irr);

  if (initial == 0) {
    forwards(120);
    delay(1);
  } else {
    if (condition == 1) {
      while (1) {
        mpu6050.update();
        float x = mpu6050.getAngleX();
        if (x < 1) {
          forwards(speed);
          delay(150);
          break;
        };
        forwards(speed);
        delay(3);
      }
      
    } else {
      if (o_left == 0 && o_right == 0) {
        end();
      }
      else if (o_left == 1 && o_right == 0) {
        right();
        delay(60);
      }
      else if (o_left == 0 && o_right == 1) {
        left();
        delay(60);
      }
      else if (o_left == 1 && o_right == 1) {
        forwards(speed);
        delay(5);
      }
    }
  }
}


void spin() {
  int limit = 1;
  
  if (digitalRead(irl) == 0) limit = 2; //checks if the left IR sensor is already on a black line
  
  delay(2000);
  int funny_counter = 0;
  for (;;) {
    right();
    delay(10);
    if (digitalRead(irl) == 0) {
      while (digitalRead(irl) == 0) {
        continue;
      }
      funny_counter++;
    }
    if (funny_counter > limit) break;
  }

  end();
  left();
  delay(50);
  forwards(90);
  delay(300);

  for (int i=0; i<25; i++) {
    if (digitalRead(irl) == 1 && digitalRead(irr) == 0) {
      left();
      delay(8);
    }
    else if (digitalRead(irl) == 0 && digitalRead(irr) == 1) {
      right();
      delay(8);
    }
    else if (digitalRead(irl) == 1 && digitalRead(irr) == 1) {
      forwards(80);
      delay(10);
    }
    else if (digitalRead(irr) == 0 && digitalRead(irl) == 0) {
      forwards(80);
      delay(4);
    }
  }

  turn = 2;

  //this allows the car to start going down the ramp when both IR sensors are LOW
  while (1) {
    mpu6050.update();
    x = mpu6050.getAngleX();
    if (x < -16) break;
    else {
      if (digitalRead(irl) == 1 && digitalRead(irr) == 0) {
        left();
        delay(3);
      }
      else if (digitalRead(irl) == 0 && digitalRead(irr) == 1) {
        right();
        delay(3);
      }
      else if (digitalRead(irl) == 1 && digitalRead(irr) == 1) {
        forwards(70);
        delay(1);
      }
      else if (digitalRead(irr) == 0 && digitalRead(irl) == 0) {
        forwards(70);
        delay(1);
    }
    }
  }

  //this allows the car to go down the ramp slowly to prevent hitting the IR sensors and detect when its already on the ground
  while (1) {
    mpu6050.update();
    x = mpu6050.getAngleX();
    //forwards(70);
    //delay(150);
    if (x > -14) break;

    else {
      if (digitalRead(irl) == 1 && digitalRead(irr) == 0) {
        left();
        delay(2);
      }
      else if (digitalRead(irl) == 0 && digitalRead(irr) == 1) {
        right();
        delay(2);
      }
      else if (digitalRead(irl) == 1 && digitalRead(irr) == 1) {
        forwards(3);
        delay(3);
      }
      else if (digitalRead(irr) == 0 && digitalRead(irl) == 0) {
        forwards(3);
        delay(3);
      }
    }
  }
  forwards(80);
  delay(350);

  end();
  delay(2000);
  count_1 = count;
  lcd.setCursor(0, 1);
  lcd.print(x);
}

int stopped = 0;
int offset = 0;
double distance = 0;
double shuaige = 0;

void loop() {
  
  mpu6050.update();
  
  x = mpu6050.getAngleX();

  if (x > max) max = x + 6; //this makes the LCD display the maximum angle

  if (initial == 0) { //this detects when the car is approaching the ramp and accelerates
    if (x > 18 && turn == 0) {
      end();
      delay(500);
      initial = 1;
      turn = 1;
      speed = 210;
      condition = 1;
    }
  }

  if (initial == 1 && x < 8 && turn == 1) { //this calls the spin function
    forwards(210);
    delay(13);
    condition = 0;
    end();
    spin();
  }

  if (turn == 2) speed = 100; //after completing the ramp, changes the speed back to 100

  lcd.setCursor(0, 0);
  unsigned long time = millis();
  lcd.print(time);

  double var_1 = count/20*3.242*6.5; //calculates the distance travelled
  double var_2;

  if (stopped == 0) {
    if (count_1 > 0) {
      var_2 = count_1/20*3.4*6.5 + 160;
      distance = var_1 - 300;
      if (var_1 >= var_2) {
        end();
        lcd.setCursor(0, 1);
        lcd.print(shuaige+40);
        delay(3000);
        stopped = 1;
      }
    }
  }

  if (turn != 2) {
    lcd.setCursor(0, 1);
    lcd.print(max+4);
  } else {
    lcd.setCursor(0, 1);
    shuaige = var_1 - var_2 + 190;
    lcd.print(shuaige-60);
  }

  process_direction(speed, condition);
}