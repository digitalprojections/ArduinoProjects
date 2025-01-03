// Blocking.pde
#include <Arduino.h>
#include "Mux.h"
using namespace admux;

//#include <AccelStepper.h>
#include "pitches.h"

int muxSigDataPin = 2;

Mux mux(Pin(muxSigDataPin, INPUT, PinType::Digital), Pinset(6, 7, 8, 9));

//timer main
int startTime;
int elapsedTime;
//timer car
int startTimeCar;
int elapsedTimeCar;
//timer tone
int startTimeTone;
int elapsedTimeTone;
//timer winkers
int startWink;
int elapsedWink;
//timer move: Forward, backward, right, left
int startMove;
int elapsedMove;


int pressedButton = -1;

bool moveDirectionExpected = true;

int const TotalStepCount = 128;
int runningStep = 0;
int presetStep = 0;
int waitInterval = 30;  //it controls the acceleration and deceleration
//bool engineStop = true;

//backward (white LEDs)
bool BackwardMoveSignal = false;
//turn signals
bool rightTurn = false;
bool leftTurn = false;
//STOP, brakes hit
bool stopLightOn = false;
//headlights
bool HeadLights = false;

bool EngineerMode = false;
bool WaitForUserInput = true;  //run or wait for input

bool ClearData = false;

//Tones
#define speakerPin 3

#define stopLigtsPin A3
//LED winkers
#define rightPin A4
#define leftPin A5

#define headLightsPin 4
#define engineerModeLed 5


#define enterDirectionLed A0
#define enterDurationLed A1
#define clearDataLed A2

//music tone pause?
#define REST -1

int StepTones[TotalStepCount];
//values
int StepMoves[TotalStepCount];
int StepDurations[TotalStepCount];
int step = 0;
//Move
enum Direction {
  Stop = 9,
  ForwardMove = 12,
  LeftMove,
  RightMove,
  BackwardMove
};

//PRESET DATA START
#pragma region PRESET MOTIONS
//Presets
enum Preset {
  PRESET_1,
  PRESET_2,
  PRESET_3,
  PRESET_4,
  PRESET_5,
  PRESET_6,
  PRESET_7,
  PRESET_8,
  PRESET_9
};

//Motion Model. Used by both preset and program modes
typedef struct {
  int motion, duration;
} motionModel;

//Preset motions
motionModel preset_one[] = {
  { ForwardMove, 100 },
  { BackwardMove, 100 },
  { LeftMove, 150 },
  { ForwardMove, 100 },
  { BackwardMove, 100 },
  { RightMove, 150 },
  { ForwardMove, 100 },
  { RightMove, 150 },
  { BackwardMove, 100 }
};

motionModel preset_two[] = {
  { RightMove, 100 },
  { ForwardMove, 100 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { ForwardMove, 100 },
  { BackwardMove, 100 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { BackwardMove, 100 }
};

motionModel preset_three[] = {
  { ForwardMove, 50 },
  { RightMove, 80 },
  { RightMove, 80 },
  { RightMove, 80 },
  { ForwardMove, 50 },
  { RightMove, 80 },
  { RightMove, 80 },
  { RightMove, 80 },
  { ForwardMove, 50 },
  { RightMove, 80 },
  { RightMove, 80 },
  { RightMove, 80 },
  { ForwardMove, 50 },
  { RightMove, 80 },
  { RightMove, 80 },
  { RightMove, 80 },
  { ForwardMove, 50 },
  { RightMove, 80 },
  { RightMove, 80 },
  { RightMove, 80 },
  { ForwardMove, 50 },
  { RightMove, 80 },
  { RightMove, 80 },
  { RightMove, 80 }
};

motionModel preset_four[] = {
  { ForwardMove, 80 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { ForwardMove, 80 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { ForwardMove, 80 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { ForwardMove, 80 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { ForwardMove, 80 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { ForwardMove, 80 },
  { LeftMove, 50 },
  { LeftMove, 50 },
  { LeftMove, 50 }
};

motionModel preset_five[] = {
  { ForwardMove, 100 },
  { RightMove, 50 },
  { RightMove, 50 },
  { RightMove, 50 },
  { RightMove, 50 },
  { RightMove, 50 },
  { RightMove, 50 },
  { RightMove, 50 },
  { RightMove, 50 },
  { ForwardMove, 100 }
};

motionModel preset_six[] = {
  { ForwardMove, 50 },
  { RightMove, 50 },
  { ForwardMove, 50 },
  { LeftMove, 50 },
  { ForwardMove, 50 },
  { RightMove, 50 },
  { ForwardMove, 50 },
  { RightMove, 50 },
  { ForwardMove, 50 },
  { RightMove, 50 },
  { ForwardMove, 50 },
  { RightMove, 50 },
  { ForwardMove, 50 },
  { RightMove, 50 },
  { ForwardMove, 50 },
  { LeftMove, 50 }
};

motionModel preset_seven[] = {
  { BackwardMove, 50 },
  { ForwardMove, 50 },
  { BackwardMove, 60 },
  { ForwardMove, 60 },
  { BackwardMove, 70 },
  { ForwardMove, 70 },
  { BackwardMove, 80 },
  { ForwardMove, 80 },
  { BackwardMove, 100 },
  { ForwardMove, 100 }
};

motionModel preset_eight[] = {
  { ForwardMove, 50 },
  { RightMove, 50 },
  { ForwardMove, 50 },
  { RightMove, 50 },
  { ForwardMove, 50 },
  { RightMove, 50 },
  { ForwardMove, 50 },
  { RightMove, 50 },
  { ForwardMove, 50 },
  { RightMove, 50 },
  { ForwardMove, 50 },
  { RightMove, 50 },
  { ForwardMove, 50 },
  { RightMove, 50 },
  { ForwardMove, 50 },
  { RightMove, 50 },
  { ForwardMove, 50 },
  { RightMove, 50 },
  { ForwardMove, 50 },
  { LeftMove, 50 },
  { ForwardMove, 50 },
  { LeftMove, 50 },
  { ForwardMove, 50 },
  { LeftMove, 50 },
  { ForwardMove, 50 },
  { LeftMove, 50 },
  { ForwardMove, 50 },
  { LeftMove, 50 }
};

motionModel preset_nine[] = {
  { BackwardMove, 50 },
  { RightMove, 50 },
  { BackwardMove, 50 },
  { RightMove, 50 },
  { BackwardMove, 50 },
  { RightMove, 50 },
  { BackwardMove, 50 },
  { RightMove, 50 },
  { BackwardMove, 50 },
  { RightMove, 50 }
};
//Preset motion durations
#pragma endregion
//PRESET DATA END

//All 16 mux keys
enum InputKey {
  One,              //0
  Ten,              //1
  Hundred,          //2
  Four,             //3
  Five,             //4
  Six,              //5
  Seven,            //6
  Eight,            //7
  Nine,             //8, also Stop step
  Mode,             //9
  ClearBtn,         //10
  EnterBtn,         //11
  BackwardMoveBtn,  //12
  RightBtn,         //13
  ForwardBtn,       //14
  BackwardBtn       //15
};

#pragma region INIT TONE
//Initialize Tone
bool initialized;
int InitMelody[] = {
  NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_F5, NOTE_C6
};
int InitDurations[] = {
  8, 8, 8,
  2, 2
};
#pragma endregion

//Start tone
int startMelody[] = {
  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6
};
int startDurations[] = {
  8, 8, 8, 2, 4
};
//Start tone end

#pragma region FULL Melody

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
#pragma endregion

int motorA1 = 12;
int motorA2 = 11;
int motorB1 = 10;
int motorB2 = 13;

void setup() {

  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  pinMode(enterDirectionLed, OUTPUT);  //A0 blue
  pinMode(enterDurationLed, OUTPUT);   //A1 green
  pinMode(clearDataLed, OUTPUT);       //A2 red

  //LED winkers
  pinMode(rightPin, OUTPUT);
  pinMode(leftPin, OUTPUT);

  //stop tail lights
  pinMode(stopLigtsPin, OUTPUT);

  //Headlights
  pinMode(headLightsPin, OUTPUT);
  //headlights OFF
  digitalWrite(headLightsPin, LOW);

  //toggle LED (OFF = toy mode/ ON = engineer mode)
  pinMode(engineerModeLed, OUTPUT);

  //mux SIG input
  pinMode(muxSigDataPin, INPUT);
  // Serial port initialization.
  Serial.begin(9600);

  PlayInitTone();
  delay(300);
  ClearSteps();

  //Green LED to indicate input range of 1-9
  digitalWrite(enterDurationLed, HIGH);
}

void SetEngineerModeLed() {
  if (EngineerMode) {
    digitalWrite(engineerModeLed, HIGH);
    Serial.print("ENGINEER MODE ON");
    delay(100);
  } else {
    digitalWrite(engineerModeLed, LOW);
    Serial.print("ENGINEER MODE OFF");
    //Green LED to indicate input range of 1-9
    digitalWrite(enterDurationLed, HIGH);
    delay(100);
  }

  if (ClearData) {
    digitalWrite(clearDataLed, HIGH);
  } else {
    digitalWrite(clearDataLed, LOW);
  }
}

void loop() {

  FlashTurnLights();

  RunMotor();


  if (EngineerMode) {
    /*ENGINEER MODE*/

    //SET Led state for programming mode
    if (moveDirectionExpected) {
      digitalWrite(enterDurationLed, LOW);
      digitalWrite(enterDirectionLed, HIGH);
    } else {
      digitalWrite(enterDurationLed, HIGH);
      digitalWrite(enterDirectionLed, LOW);
    }

    //timeout between button clicks
    elapsedTime = millis() - startTime;
    if (WaitForUserInput) {
      if (elapsedTime > 100) {
        byte data;
        for (byte i = 0; i < mux.channelCount(); i++) {
          data = mux.read(i) /* Reads from channel i (returns HIGH or LOW) */;
          if (data == LOW) {
            pressedButton = i;
            Serial.println(i);

            //Clear action works unconditionally
            if (pressedButton == ClearBtn) {
              //Reset
              step = 0;
              ClearSteps();
            }

            if (pressedButton == Mode) {
              EngineerMode = false;
              SetEngineerModeLed();
            }
            if (moveDirectionExpected && pressedButton == EnterBtn) {
              //anytime Enter is pressed, the car starts executing the program
              //Start move
              CountDownTimer();
              WaitForUserInput = false;
              step = 0;
              return;
            }

            if (step < TotalStepCount) {
              //Forward, backward, right, left
              if (moveDirectionExpected) {
                //12-15 DIRECTIONS, BackwardMove, DOWN, UP, RIGHT respectively
                if (pressedButton > EnterBtn) {
                  StepMoves[step] = pressedButton;
                  StepTones[step] = melody[pressedButton];
                  //Assign programmed motion value as is, 12 to 16
                  PlayTone(melody[pressedButton]);
                  moveDirectionExpected = false;
                }
              } else {
                //values between 1-9
                if (pressedButton < Mode) {
                  //Assign programmed motion value
                  StepDurations[step] = (pressedButton * 10) + 50;
                  StepTones[step] = melody[pressedButton];
                  PlayTone(melody[pressedButton]);
                  moveDirectionExpected = true;
                  //Advance program step
                  step++;
                }
              }
            } else {
              //step overflow. Ignore?
            }
          }
        }
        startTime = millis();
        noTone(speakerPin);
      }
    }
  } else {
    digitalWrite(enterDirectionLed, LOW);
    /*EngineerMode FALSE, "ToyMode", Preset MODE*/
    //user input expected
    if (WaitForUserInput) {
      elapsedTime = millis() - startTime;
      if (elapsedTime > 100) {
        byte data;
        for (byte i = 0; i < mux.channelCount(); i++) {
          data = mux.read(i) /* Reads from channel i (returns HIGH or LOW) */;
          if (data == LOW) {
            pressedButton = i;
            Serial.print(pressedButton);
            //0-9 NUMBERS
            if (pressedButton < Mode && pressedButton >= 0) {
              //Assign programmed motion value
              if (pressedButton == PRESET_1) {
                //Preset 1
                int sizeOfArray = sizeof(preset_one);
                int sizeOfPreset = sizeOfArray / sizeof(motionModel);
                for (int j = 0; j < sizeOfPreset; j++) {
                  StepMoves[presetStep] = preset_one[j].motion;
                  StepTones[presetStep] = melody[j];
                  StepDurations[presetStep] = preset_one[j].duration;
                  presetStep++;
                  if (presetStep >= TotalStepCount) {
                    break;
                  }
                }
              } else if (pressedButton == PRESET_2) {
                //Preset 2
                int sizeOfArray = sizeof(preset_two);
                int sizeOfPreset = sizeOfArray / sizeof(motionModel);
                for (int j = 0; j < sizeOfPreset; j++) {
                  StepMoves[presetStep] = preset_two[j].motion;
                  StepTones[presetStep] = melody[j];
                  StepDurations[presetStep] = preset_two[j].duration;
                  presetStep++;
                  if (presetStep >= TotalStepCount) {
                    break;
                  }
                }
              } else if (pressedButton == PRESET_3) {
                //Preset 3
                int sizeOfArray = sizeof(preset_three);
                int sizeOfPreset = sizeOfArray / sizeof(motionModel);
                for (int j = 0; j < sizeOfPreset; j++) {
                  StepMoves[presetStep] = preset_three[j].motion;
                  StepTones[presetStep] = melody[j];
                  StepDurations[presetStep] = preset_three[j].duration;
                  presetStep++;
                  if (presetStep >= TotalStepCount) {
                    break;
                  }
                }
              } else if (pressedButton == PRESET_4) {
                //Preset 4
                int sizeOfArray = sizeof(preset_four);
                int sizeOfPreset = sizeOfArray / sizeof(motionModel);
                for (int j = 0; j < sizeOfPreset; j++) {
                  StepMoves[presetStep] = preset_four[j].motion;
                  StepTones[presetStep] = melody[j];
                  StepDurations[presetStep] = preset_four[j].duration;
                  presetStep++;
                  if (presetStep >= TotalStepCount) {
                    break;
                  }
                }
              } else if (pressedButton == PRESET_5) {
                //Preset 5
                int sizeOfArray = sizeof(preset_five);
                int sizeOfPreset = sizeOfArray / sizeof(motionModel);
                for (int j = 0; j < sizeOfPreset; j++) {
                  StepMoves[presetStep] = preset_five[j].motion;
                  StepTones[presetStep] = melody[j];
                  StepDurations[presetStep] = preset_five[j].duration;
                  presetStep++;
                  if (presetStep >= TotalStepCount) {
                    break;
                  }
                }
              } else if (pressedButton == PRESET_6) {
                //Preset 6
                int sizeOfArray = sizeof(preset_six);
                int sizeOfPreset = sizeOfArray / sizeof(motionModel);
                for (int j = 0; j < sizeOfPreset; j++) {
                  StepMoves[presetStep] = preset_six[j].motion;
                  StepTones[presetStep] = melody[j];
                  StepDurations[presetStep] = preset_six[j].duration;
                  presetStep++;
                  if (presetStep >= TotalStepCount) {
                    break;
                  }
                }
              } else if (pressedButton == PRESET_7) {
                //Preset 7
                int sizeOfArray = sizeof(preset_seven);
                int sizeOfPreset = sizeOfArray / sizeof(motionModel);
                for (int j = 0; j < sizeOfPreset; j++) {
                  StepMoves[presetStep] = preset_seven[j].motion;
                  StepTones[presetStep] = melody[j];
                  StepDurations[presetStep] = preset_seven[j].duration;
                  presetStep++;
                  if (presetStep >= TotalStepCount) {
                    break;
                  }
                }
              } else if (pressedButton == PRESET_8) {
                //Preset 8
                int sizeOfArray = sizeof(preset_eight);
                int sizeOfPreset = sizeOfArray / sizeof(motionModel);
                for (int j = 0; j < sizeOfPreset; j++) {
                  StepMoves[presetStep] = preset_eight[j].motion;
                  StepTones[presetStep] = melody[j];
                  StepDurations[presetStep] = preset_eight[j].duration;
                  presetStep++;
                  if (presetStep >= TotalStepCount) {
                    break;
                  }
                }
              } else if (pressedButton == PRESET_9) {
                //Preset 9
                int sizeOfArray = sizeof(preset_nine);
                int sizeOfPreset = sizeOfArray / sizeof(motionModel);
                for (int j = 0; j < sizeOfPreset; j++) {
                  StepMoves[presetStep] = preset_nine[j].motion;
                  StepTones[presetStep] = melody[j];
                  StepDurations[presetStep] = preset_nine[j].duration;
                  presetStep++;
                  if (presetStep >= TotalStepCount) {
                    break;
                  }
                }
              }

              digitalWrite(enterDurationLed, LOW);
              delay(100);
              digitalWrite(enterDurationLed, HIGH);
              pressedButton = -1;
            } else if (pressedButton == Mode) {

              Serial.print("ENGINEER MODE ON    ");
              EngineerMode = true;
              SetEngineerModeLed();
              ClearSteps();
            }
            //10=Clear, 11=Enter
            else if (pressedButton == ClearBtn) {
              //Reset
              ClearSteps();
            } else if (pressedButton == EnterBtn) {
              //anytime Enter is pressed, the car starts executing the program
              //Start move
              runningStep = 0;
              Serial.println(presetStep);
              for (int k = presetStep; k < 128; k++) {
                StepMoves[k] = -1;
              }
              presetStep = 0;
              CountDownTimer();
              WaitForUserInput = false;
              HeadLights = true;
              digitalWrite(stopLigtsPin, LOW);
            }
          }
        }
        startTime = millis();
        noTone(speakerPin);
      }
    } else {
      /*WaitForUserInput FALSE*/
    }
  }
}

void CountDownTimer() {
  //three beeps
  for (int i = 0; i < 3; i++) {
    digitalWrite(headLightsPin, HIGH);
    digitalWrite(stopLigtsPin, HIGH);
    digitalWrite(rightPin, HIGH);
    digitalWrite(leftPin, HIGH);
    digitalWrite(enterDurationLed, HIGH);
    digitalWrite(enterDirectionLed, HIGH);
    digitalWrite(engineerModeLed, HIGH);
    digitalWrite(clearDataLed, HIGH);
    PlayTone(NOTE_C5);
    delay(100);
    digitalWrite(headLightsPin, LOW);
    digitalWrite(stopLigtsPin, LOW);
    digitalWrite(rightPin, LOW);
    digitalWrite(leftPin, LOW);
    digitalWrite(enterDurationLed, LOW);
    digitalWrite(enterDirectionLed, LOW);
    digitalWrite(engineerModeLed, EngineerMode);
    digitalWrite(clearDataLed, LOW);
    noTone(speakerPin);
    delay(500);
  }
  //final tone before start
  PlayStartTune();
  delay(300);
}

void RunMotor() {
  if (!WaitForUserInput) {
    if (runningStep < TotalStepCount && StepMoves[runningStep] > 0) {
      switch (StepMoves[runningStep]) {
        case BackwardMove:
          GoBackward();
          Serial.print("Go BACK ");
          break;
        case ForwardMove:
          GoForward();
          Serial.print("Go FORWARD ");
          break;
        case RightMove:
          TurnRight();
          Serial.print("Go RIGHT ");
          break;
        case LeftMove:
          TurnLeft();
          Serial.print("Go LEFT ");
          break;
        case Nine:
          StopMoving();
          delay(StepDurations[runningStep]);
          break;
        default:
          break;
      }
    } else {
      runningStep = TotalStepCount;
      WaitForUserInput = true;
      moveDirectionExpected = true;
      startTime = millis();

      StopMoving();
      BackwardMoveSignal = false;
      HeadLights = false;
      stopLightOn = false;
      digitalWrite(stopLigtsPin, LOW);
      ClearSteps();
      //engineStop = true;
      Serial.print("final: ");
      Serial.println(runningStep);
    }
  }
}

void PlayTone(int t) {
  tone(speakerPin, t, 300);
}

//Init Melody
#pragma region PlayInitTone
void PlayInitTone() {
  if (!initialized) {
    int size = sizeof(InitDurations) / sizeof(int);

    for (int note = 0; note < size; note++) {
      //to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int duration = 1000 / InitDurations[note];
      tone(speakerPin, InitMelody[note], duration);

      //to distinguish the notes, set a minimum time between them.
      //the note's duration + 30% seems to work well:
      int pauseBetweenNotes = duration * 0.30;
      delay(pauseBetweenNotes);

      //stop the tone playing:
      noTone(speakerPin);
      initialized = true;
    }
  }
}

void PlayStartTune() {
  int size = sizeof(startDurations) / sizeof(int);

  for (int note = 0; note < size; note++) {
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000 / startDurations[note];
    tone(speakerPin, startMelody[note], duration);

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 0.30;
    delay(pauseBetweenNotes);
    //stop the tone playing:
    noTone(speakerPin);
  }
}
#pragma endregion

void FlashTurnLights() {
  //leftTurn
  elapsedWink = millis() - startWink;
  if (elapsedWink >= 100) {
    if (leftTurn) {
      startWink = millis();
      if (digitalRead(leftPin) == HIGH) {
        digitalWrite(leftPin, LOW);
      } else {
        digitalWrite(leftPin, HIGH);
      }
    } else {
      digitalWrite(leftPin, LOW);
    }
    if (rightTurn) {
      startWink = millis();
      if (digitalRead(rightPin) == HIGH) {
        digitalWrite(rightPin, LOW);
      } else {
        digitalWrite(rightPin, HIGH);
      }
    } else {
      digitalWrite(rightPin, LOW);
    }
  }

  if (HeadLights) {
    digitalWrite(headLightsPin, HIGH);
  } else {
    digitalWrite(headLightsPin, LOW);
  }
}

void FlashStopLights() {
  if (stopLightOn) {
    digitalWrite(stopLigtsPin, HIGH);
    stopLightOn = false;
  } else {
    digitalWrite(stopLigtsPin, LOW);
    stopLightOn = true;
  }
}

void TurnRight() {
  int duration = StepDurations[runningStep];
  int lastHalfLimit = duration - waitInterval;
  int dur = waitInterval;
  leftTurn = false;
  rightTurn = true;
  for (int i = 0; i < duration; i++) {
    FlashTurnLights();
    //ON
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    delay(10);
    //OFF
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, LOW);
    delay(dur);
    if (dur <= 1) {
      dur = 1;
    }
    if (i < lastHalfLimit) {
      dur--;
    } else {
      dur++;
      if (i >= duration - 1) {
        //END STEP
        runningStep++;
      }
    }
  }

  StopMoving();
}
void TurnLeft() {
  int duration = StepDurations[runningStep];
  int lastHalfLimit = duration - waitInterval;
  int dur = waitInterval;
  rightTurn = false;
  leftTurn = true;
  for (int i = 0; i < duration; i++) {
    FlashTurnLights();
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    delay(10);
    //OFF
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, LOW);
    delay(dur);
    if (dur <= 1) {
      dur = 1;
    }
    if (i < lastHalfLimit) {
      dur--;
    } else {
      dur++;
      if (i >= duration - 1) {
        //END STEP
        runningStep++;
      }
    }
  }

  StopMoving();
}
void GoForward() {
  BackwardMoveSignal = false;
  int duration = StepDurations[runningStep];
  int lastHalfLimit = duration - waitInterval;
  int dur = waitInterval;
  for (int i = 0; i < duration; i++) {
    //ON
    //ON
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);
    delay(10);
    //OFF
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, LOW);
    delay(dur);
    if (dur <= 1) {
      dur = 1;
    }
    if (i < lastHalfLimit) {
      dur--;
    } else {
      dur++;
      if (i >= duration - 1) {
        //END STEP
        runningStep++;
      }
    }
  }

  StopMoving();
  // delay(300);
  // PlayTone(StepTones[runningStep]);
  // delay(300);
  // StepMoves[runningStep] = -1;
  //runningStep++;
}
void GoBackward() {
  int duration = StepDurations[runningStep];
  int lastHalfLimit = duration - waitInterval;
  int dur = waitInterval;
  for (int i = 0; i < duration; i++) {
    //ON
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);
    delay(10);
    //OFF
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, LOW);
    delay(dur);
    if (dur <= 1) {
      dur = 1;
    }
    if (i < lastHalfLimit) {
      dur--;
    } else {
      dur++;
      if (i >= duration - 1) {
        //END STEP
        runningStep++;
      }
    }
    BackwardMoveSignal = true;
  }

  StopMoving();
}
void StopMoving() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
  BackwardMoveSignal = false;
  digitalWrite(stopLigtsPin, HIGH);
  rightTurn = leftTurn = false;
  digitalWrite(rightPin, LOW);
  digitalWrite(leftPin, LOW);
}

//clear steps
void ClearSteps() {
  for (int i = 0; i < TotalStepCount; i++) {
    StepMoves[i] = 0;
    StepDurations[i] = 0;
    StepTones[i] = 0;
  }

  runningStep = 0;
  presetStep = 0;
  HeadLights = false;
  rightTurn = false;
  leftTurn = false;
  stopLightOn = false;
  digitalWrite(clearDataLed, HIGH);
  PlayTone(NOTE_A5);
  delay(200);
  digitalWrite(clearDataLed, LOW);
  noTone(speakerPin);
}
