/*
This is a program to control
temperature, stepper motor and its speed and LCD display
based on input from user
Date: 2024 July
Author: Fuzalov Ahmadkhon
*/
#include <SoftwareSerial.h>

#include "AccelStepper.h"
// defines pins
#define stepPin 4
#define dirPin 5
#define motorInterfaceType 1

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

//control input timing
long startTime;
long elapsedTime;
int speedDelay = 2000;
int stepperPosition = 100;
#define stepsPerRevolution 200
//

#define ThermistorPin A7
int tempTime = 0;
int tempTimeLap = 0;
int Vo;
float R1 = 10000;
float logR2, R2, T, C;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

#define Potpin A1

bool relayOff = false;

int relayPinFilamentMaker = 2;   //relay 1
int relayPinFilamentJoiner = 3;  //relay 2

void setup() {

  pinMode(ThermistorPin, INPUT);
  pinMode(Potpin, INPUT);

  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);

  analogWrite(A0, LOW);
  analogWrite(A2, LOW);
  analogWrite(A6, LOW);

  //Stepper
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  // Set the maximum speed and acceleration:
  stepper.setMaxSpeed(10000);
  stepper.setAcceleration(100);
  stepper.setSpeed(10000);

  startTime = millis();
  //
  pinMode(relayPinFilamentMaker, OUTPUT);
  Serial.begin(9600);
  digitalWrite(relayPinFilamentMaker, LOW);
}

void loop() {
  //Stepper
  elapsedTime = millis() - startTime;

  if (elapsedTime > 1) {
    startTime = millis();
    stepperPosition++;
    Run();
  }


  tempTimeLap = millis() - tempTime;

  if (tempTimeLap > 1000) {

    tempTime = millis();

    Vo = analogRead(ThermistorPin);
    R2 = R1 * (1023.0 / (float)Vo - 1.0);
    logR2 = log(R2);
    T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
    C = T - 273.15;
    //C = (C * 9.0) / 5.0 + 32.0;

    //C = (T - 32) * (5 / 9);

    Serial.print("Temperature: ");
    Serial.print(C);
    Serial.println(" C");

    //Set temperature

    long pot = analogRead(Potpin);
    //Serial.println(pot);

    analogWrite(A3, pot);
    analogWrite(A4, 1023 - pot);
    analogWrite(A5, 500 - pot);

    //0-220
    long setTemp = map(pot, 0, 1023, 0, 220);
    if (C > setTemp) {
      digitalWrite(relayPinFilamentMaker, LOW);
      Serial.println("relay off");
    } else {
      if (C < setTemp) {
        digitalWrite(relayPinFilamentMaker, HIGH);
        Serial.println("relay on");
      }
    }
  }
}

void Run() {

  // Set the target position:
  //stepper.moveTo(stepperPosition);
  stepper.runSpeed();
  //}
  //
  //   // Set the spinning direction clockwise:
  //   digitalWrite(dirPin, HIGH);

  //   // Spin the stepper motor 1 revolution slowly:
  //   for (int i = 0; i < stepsPerRevolution; i++) {
  //     // These four lines result in 1 step:
  //     digitalWrite(stepPin, HIGH);
  //     delayMicroseconds(speedDelay);
  //     digitalWrite(stepPin, LOW);
  //     delayMicroseconds(speedDelay);
  //   }
}