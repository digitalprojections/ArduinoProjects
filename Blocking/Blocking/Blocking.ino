// Blocking.pde
#include <Arduino.h>
#include "Mux.h"
using namespace admux;

//#include <AccelStepper.h>
#include "pitches.h"

Mux mux(Pin(A0, INPUT, PinType::Analog), Pinset(6, 7, 8, 9));

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

int const programSteps = 128;
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
#define REST -1
int melody[] = {
  NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_F5, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_G5, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_F5, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,

  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_G5, NOTE_C5, NOTE_C5,
  NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,
  NOTE_F5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_C5,
  NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,

  NOTE_C6, NOTE_G5, NOTE_G5, REST, NOTE_C5,
  NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,
  NOTE_F5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_D5, NOTE_E5, NOTE_C6, NOTE_C6,
  NOTE_F6, NOTE_DS6, NOTE_CS6, NOTE_C6, NOTE_AS5, NOTE_GS5, NOTE_G5, NOTE_F5,
  NOTE_C6
};

int durations[] = {
  8, 8, 8,
  2, 2,
  8, 8, 8, 2, 4,
  8, 8, 8, 2, 4,
  8, 8, 8, 2, 8, 8, 8,
  2, 2,
  8, 8, 8, 2, 4,

  8, 8, 8, 2, 4,
  8, 8, 8, 2, 8, 16,
  4, 8, 8, 8, 8, 8,
  8, 8, 8, 4, 8, 4, 8, 16,
  4, 8, 8, 8, 8, 8,

  8, 16, 2, 8, 8,
  4, 8, 8, 8, 8, 8,
  8, 8, 8, 4, 8, 4, 8, 16,
  4, 8, 4, 8, 4, 8, 4, 8,
  1
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
    PlayStartTone();

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

        if (data == LOW) {

          pressedButton = i;

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
              //anytime Enter is pressed, the car starts executing the program
              //Start move
              programmingMode = false;
            }
            //12-15 DIRECTIONS, LEFT, DOWN, UP, RIGHT respectively
            else {
              //Assign programmed motion value as is, 12 to 16
              Moves[step] = i;
              Tones[step] = tones[i];
              PlayTone(tones[pressedButton]);
              //Advance program step
              step++;
            }
          } else {
            if (pressedButton == EnterBtn)
              pressedButton = -1;
            //Start move
            programmingMode = false;
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

//sequence of tones to play before the program execution
void PlayStartTone() {
  engineStop = false;

  int size = sizeof(durations) / sizeof(int);

  for (int note = 0; note < size; note++) {
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000 / durations[note];
    tone(speakerPort, melody[note], duration);

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);

    //stop the tone playing:
    noTone(speakerPort);
  }
  //lastly
  engineStop = true;
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
