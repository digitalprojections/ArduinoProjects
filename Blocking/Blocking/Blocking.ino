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

//timer
int startTime;
int elapsedTime;
int pressedButton = -1;

// Define a stepper and the pins it will use
AccelStepper stepper;  // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
/*
 * Creates a Mux instance.
 *
 * 1st argument is the SIG (signal) pin (Arduino digital input pin 3).
 * 2nd argument is the S0-S3 (channel control) pins (Arduino pins 8, 9, 10, 11).
 */
Mux mux(Pin(A0, INPUT, PinType::Analog), Pinset(8, 9, 10, 11));

void setup() {
  stepper.setMaxSpeed(2000.0);
  stepper.setAcceleration(100.0);
  stepper.setSpeed(2000);
  // Serial port initialization.
  Serial.begin(9600);
  while (!Serial) /* Waits for serial port to connect (needed for Leonardo only) */
    ;
}

void loop() {
  elapsedTime = millis() - startTime;
  if (elapsedTime > 500) {
    RunMotor();


    byte data;
    for (byte i = 0; i < mux.channelCount(); i++) {
      data = mux.read(i) /* Reads from channel i (returns HIGH or LOW) */;

      if (data == LOW && pressedButton != i) {
        pressedButton = i;
        Serial.print("Push button at channel ");
        Serial.print(i);
        //   Serial.print(" is ");
        //   Serial.println(data == LOW ? "pressed" : "not pressed");
        Serial.println();
        startTime = millis();
        pressedButton = -1;
      } 
    }
  }
}

void RunMotor() {
  stepper.moveTo(0);
  stepper.run();
  stepper.moveTo(500);
  stepper.run();
}
