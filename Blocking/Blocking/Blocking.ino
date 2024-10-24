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
/*
 * Creates a Mux instance.
 *
 * 1st argument is the SIG (signal) pin (Arduino digital input pin 3).
 * 2nd argument is the S0-S3 (channel control) pins (Arduino pins 8, 9, 10, 11).
 */
Mux mux(Pin(A0, INPUT, PinType::Analog), Pinset(8, 9, 10, 11));

// Define a stepper and the pins it will use
AccelStepper stepper;  // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

//timer main
int startTime;
int elapsedTime;
//timer car
int startTimeCar;
int elapsedTimeCar;
int pressedButton = -1;

//two states: running and programming
bool programmingMode = true;

//Tones

int Tones[10];
//values
int Moves[10];
int step = 0;
//Move
enum Motion {
  Forward,
  Backward,
  Right,
  Left
};
enum Button{
ForwardBtn, BackwardBtn, RightBtn, LeftBtn, EnterBtn
};

void setup() {
  //stepper
  stepper.setMaxSpeed(2000.0);
  stepper.setAcceleration(100.0);
  stepper.setSpeed(2000);
  // Serial port initialization.
  Serial.begin(9600);
  /* Waits for serial port to connect (needed for Leonardo only) */
  while (!Serial)
    ;
}

void loop() {

  elapsedTime = millis() - startTime;
  if (!programmingMode) {
    RunMotor();
  } else {
    if (elapsedTime > 500) {
      byte data;
      for (byte i = 0; i < mux.channelCount(); i++) {
        data = mux.read(i) /* Reads from channel i (returns HIGH or LOW) */;

        if (data == LOW && pressedButton != i) {
          pressedButton = i;
          Serial.print("Push button at channel ");
          Serial.print(i);
          Serial.println();

          if (step < 9) {
            //Assign programmed motion value
            Moves[step] == i;
            //Advance program step
            step++;
          } else {
            if (pressedButton == EnterBtn)
              //Start move
              programmingMode = false;
              pressedButton = -1;
          }
          startTime = millis();          
        }
      }
    }
  }
}

void RunMotor() {
  elapsedTimeCar = millis() - startTimeCar;
  if (elapsedTimeCar > 500) {
    stepper.moveTo(0);
    stepper.runToNewPosition(100);    
    stepper.runToNewPosition(500);
    startTimeCar = millis();
  }
}
