// Blocking.pde
// -*- mode: C++ -*-
//
// Shows how to use the blocking call runToNewPosition
// Which sets a new target position and then waits until the stepper has
// achieved it.
//
// Copyright (C) 2009 Mike McCauley
// $Id: Blocking.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $
#include <Arduino.h>
#include "Mux.h"
using namespace admux;

#include <AccelStepper.h>
#include "pitches.h"
/*
 * Creates a Mux instance.
 *
 * 1st argument is the SIG (signal) pin (Arduino digital input pin 3).
 * 2nd argument is the S0-S3 (channel control) pins (Arduino pins 8, 9, 10, 11).
 */
Mux mux(Pin(A0, INPUT, PinType::Analog), Pinset(6, 7, 8, 9));

// Define a stepper and the pins it will use
//AccelStepper stepper;  // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

//timer main
int startTime;
int elapsedTime;
//timer car
int startTimeCar;
int elapsedTimeCar;
//timer tone
int startTimeTone;
int elapsedTimeTone;

int pressedButton = -1;

//two states: running and programming
bool programmingMode = true;

int const programSteps = 10;
int runningStep = 0;
bool engineStop = true;
//Tones
#define speakerPort A4
#define enterLed 2
#define clearDataLed 3
#define inputLed 4

int Tones[programSteps];
//values
int Moves[programSteps];
int step = 0;
//Move
enum Motion {
  Forward,
  Backward,
  Right,
  Left
};
int tones[] = {
  NOTE_B4,
  NOTE_C4,
  NOTE_CS4,
  NOTE_D4,
  NOTE_DS4,
  NOTE_B6,
  NOTE_B2,
  NOTE_A2,
  NOTE_A5,
  NOTE_B3,
  NOTE_B5,
  NOTE_C2,
  NOTE_D3,
  NOTE_E3,
  NOTE_AS5,
  NOTE_DS2
};
enum Button {
  Zero,
  One,
  Two,
  Three,
  Four,
  Five,
  Six,
  Seven,
  Eight,
  Nine,
  Ten,
  ClearBtn,
  EnterBtn,
  LeftBtn,
  BackwardBtn,
  ForwardBtn,
  RightBtn
};

int motorA1 = 12;
int motorA2 = 11;
int motorB1 = 10;
int motorB2 = 13;

void setup() {

  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  pinMode(inputLed, OUTPUT);
  pinMode(clearDataLed, OUTPUT);
  pinMode(enterLed, OUTPUT);

  //stepper
  //stepper.setMaxSpeed(1000.0);
  //stepper.setAcceleration(50.0);
  //stepper.setSpeed(1000);

  pinMode(A1, OUTPUT);
  // Serial port initialization.
  Serial.begin(9600);
}

void loop() {

  if (programmingMode) {
    digitalWrite(inputLed, HIGH);
    digitalWrite(clearDataLed, LOW);
    digitalWrite(enterLed, LOW);
  } else {
    digitalWrite(inputLed, LOW);
    digitalWrite(clearDataLed, LOW);
    digitalWrite(enterLed, HIGH);
  }

  elapsedTime = millis() - startTime;
  if (!programmingMode) {
    if (engineStop) {
      engineStop = false;
      RunMotor();
    }

    if (elapsedTime > 1000 && runningStep < programSteps) {
      startTime = millis();
      Serial.print("step: ");
      Serial.println(runningStep);
      PlayTone(Tones[runningStep]);
      digitalWrite(A1, HIGH);
    }
  } else {
    if (elapsedTime > 500) {
      byte data;
      for (byte i = 0; i < mux.channelCount(); i++) {
        data = mux.read(i) /* Reads from channel i (returns HIGH or LOW) */;

        if (data == LOW && pressedButton != i) {
          pressedButton = i;
          Serial.print("Push button at channel ");
          Serial.println(i);

          if (step < programSteps) {

            //0-9 NUMBERS
            if (pressedButton < 10) {
              //Assign programmed motion value
              Moves[step] = i * 500;
              Tones[step] = tones[i];
              PlayTone(tones[pressedButton]);
              //Advance program step
              step++;
            }
            //10=Clear, 11=Enter
            else if (pressedButton == ClearBtn) {
              //Reset
              step = 0;
              ClearSteps();
            } else if (pressedButton == EnterBtn) {
              //Enter
            }
            //12-15 DIRECTIONS, LEFT, DOWN, UP, RIGHT respectively
            else {
              //Assign programmed motion value
              Moves[step] = i * 500;
              Tones[step] = tones[i];
              PlayTone(tones[pressedButton]);
              //Advance program step
              step++;
            }
          } else {
            if (pressedButton == EnterBtn)
              //Start move
              programmingMode = false;
            pressedButton = -1;
            step = 0;
          }
          //next input OK
          startTime = millis();
        }
      }
      noTone(speakerPort);
    }
  }
}

void RunMotor() {

  while (!engineStop) {
    if (runningStep < programSteps) {
      //stepper.moveTo(0);
      //stepper2.moveTo(0);
      //stepper.move(Moves[runningStep]);
      //stepper.runToNewPosition(Moves[runningStep]);

      GoForward();
      delay(Moves[runningStep]);
      StopMoving();
      delay(300);
      runningStep++;
    } else {
      runningStep = 0;
      programmingMode = true;
      StopMoving();
    }
    engineStop = true;
    Serial.print("runningStep");
    Serial.println(runningStep);
  }
}

void PlayTone(int t) {
  tone(speakerPort, t, 100);
  delay(100);
}

void GoForward() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
}
void GoRight() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
}
void GoLeft() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
}
void StopMoving() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
}

//clear steps
void ClearSteps() {
  PlayTone(NOTE_A5);
  delay(200);
  noTone(speakerPort);
}
