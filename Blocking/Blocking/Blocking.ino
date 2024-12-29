// Blocking.pde
#include <Arduino.h>
#include "Mux.h"
using namespace admux;
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>
#include "Adafruit_ST77xx.h"
#include <Adafruit_PCF8574.h>

//#include <AccelStepper.h>
#include "pitches.h"

int muxSignalPin = 2;

Mux mux(Pin(muxSignalPin, INPUT, PinType::Digital), Pinset(8, 7, 6, 4));

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

int const TotalStepCount = 128;
int runningStep = 0;
int waitInterval = 30;  //it controls the acceleration and deceleration
//bool engineStop = true;

//direction flag to flash the turn signals. The same method is used to flash either or both winkers
bool RightSignal = false;
bool BackwardMoveSignal = false;
//turn signals
bool leftTurn = false;
bool rightTurn = false;
//STOP, brakes hit
bool stopLightOn = false;
//headlights
bool HeadLights = false;

bool EngineerMode = false;
bool WaitForUserInput = true;

bool ClearData = false;

//Tones
#define speakerPin A2

#define stopLigtsPin A3
//LED winkers
#define BackwardMovePin A1
#define RightPin A0

#define headLightsPin 3
#define engineerModeLed 5


#define p_enterValueLed 5
#define p_enterDirectionLed 6
#define p_clearDataLed 7


#define TFT_CS 8    // if your display has CS pin
#define TFT_RST 10  // reset pin
#define TFT_DC 9    // data pin
#define TFT_MOSI 11
#define TFT_SCLK 13
//music tone pause?
#define REST -1

Adafruit_PCF8574 pcf;

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

int StepTones[TotalStepCount];
//values
int StepMoves[TotalStepCount];
int StepDurations[TotalStepCount];
int step = 0;
//Move
enum Direction {
  Stop = 9,
  BackwardMove = 12,
  ForwardMove,
  RightMove,
  LeftMove
};

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

int p_motorA1 = 1;
int p_motorA2 = 2;
int p_motorB1 = 3;
int p_motorB2 = 4;

void setup() {
  // Use this initializer (uncomment) if using a 1.3" or 1.54" 240x240 TFT:
  tft.init(240, 240, SPI_MODE3);  // Init ST7789 240x240
  tft.setRotation(2);

  Serial.println(F("Initialized"));
  // tft print function!
  tft.setTextSize(4);
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(20, 110);
  tft.fillRect(0, 0, 240, 240, ST77XX_BLUE);

  tft.fillRect(0, 105, 240, 40, ST77XX_ORANGE);
  tft.println("You Won!");

  if (!pcf.begin(0x20, &Wire)) {
    Serial.println("Couldn't find PCF8574");
    while (1)
      ;
  }
  Serial.println("Could find PCF8574");

  pcf.pinMode(p_motorA1, OUTPUT);
  pcf.pinMode(p_motorA2, OUTPUT);
  pcf.pinMode(p_motorB1, OUTPUT);
  pcf.pinMode(p_motorB2, OUTPUT);

  pcf.pinMode(p_clearDataLed, OUTPUT);       //7 green
  pcf.pinMode(p_enterDirectionLed, OUTPUT);  //6 blue
  pcf.pinMode(p_enterValueLed, OUTPUT);      //5 red

  //LED winkers
  pinMode(BackwardMovePin, OUTPUT);
  pinMode(RightPin, OUTPUT);

  //stop tail lights
  pinMode(stopLigtsPin, OUTPUT);

  //Headlights
  pinMode(headLightsPin, OUTPUT);
  //headlights OFF
  digitalWrite(headLightsPin, LOW);

  //toggle LED (OFF = toy mode/ ON = engineer mode)
  pinMode(engineerModeLed, OUTPUT);

  //mux SIG input
  pinMode(muxSignalPin, INPUT);
  // Serial port initialization.
  Serial.begin(9600);

  PlayInitTone();
  delay(300);
  ClearSteps();
}

void SetEngineerModeLed() {

  pcf.digitalWrite(7, LOW);  // turn LED on by sinking current to ground
  delay(100);
  pcf.digitalWrite(7, HIGH);  // turn LED off by turning off sinking transistor
  delay(100);

  if (EngineerMode) {
    if (digitalRead(engineerModeLed) == HIGH) {
      digitalWrite(engineerModeLed, LOW);
    }

  } else {
    if (digitalRead(engineerModeLed) == LOW) {
      digitalWrite(engineerModeLed, HIGH);
    }
  }

  if (ClearData) {
    digitalWrite(p_clearDataLed, HIGH);
  } else {
    digitalWrite(p_clearDataLed, LOW);
  }
}

void loop() {

  SetEngineerModeLed();
  FlashTurnLights();

  RunMotor();

  if (EngineerMode) {
    /*ENGINEER MODE*/

    if (programmingMode) {
      digitalWrite(p_enterValueLed, HIGH);
      //digitalWrite(p_, HIGH);
      //digitalWrite(p_, HIGH);
    } else {
      digitalWrite(p_enterValueLed, LOW);
      //digitalWrite(p_, LOW);
      //digitalWrite(p_, LOW);
    }

    elapsedTime = millis() - startTime;
    if (!programmingMode) {
      if (elapsedTime > 1000 && runningStep < TotalStepCount) {
        startTime = millis();
        Serial.print("step: ");
        Serial.println(runningStep);
        PlayTone(StepTones[runningStep]);
      }
    } else {
      if (elapsedTime > 100) {
        byte data;
        for (byte i = 0; i < mux.channelCount(); i++) {
          data = mux.read(i) /* Reads from channel i (returns HIGH or LOW) */;
          if (data == LOW) {
            pressedButton = i;
            Serial.println(i);
            if (step < TotalStepCount) {
              //0-9 NUMBERS
              if (pressedButton < Mode) {
                //Assign programmed motion value
                StepMoves[step] = i * 500;
                StepTones[step] = melody[i];
                PlayTone(melody[pressedButton]);
                //Advance program step
                step++;
              } else if (pressedButton == Mode) {
                EngineerMode = !EngineerMode;
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
              //12-15 DIRECTIONS, BackwardMove, DOWN, UP, RIGHT respectively
              else {
                //Assign programmed motion value as is, 12 to 16
                StepMoves[step] = i;
                StepTones[step] = melody[i];
                PlayTone(melody[pressedButton]);
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
          }
        }
        startTime = millis();
        noTone(speakerPin);
      }
    }
  } else {
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
            if (pressedButton < Mode) {
              //Assign programmed motion value
              int motionType = BackwardMove;
              for (int j = 0; j < 10; j++) {
                StepMoves[j] = motionType;
                motionType++;
                if (motionType > LeftMove) {
                  motionType = BackwardMove;
                }
                StepTones[j] = melody[j];
                StepDurations[j] = 100;
              }
            } else if (pressedButton == Mode) {
              EngineerMode = !EngineerMode;
            }
            //10=Clear, 11=Enter
            else if (pressedButton == ClearBtn) {
              //Reset
              ClearSteps();
            } else if (pressedButton == EnterBtn) {
              //anytime Enter is pressed, the car starts executing the program
              //Start move
              runningStep = 0;
              WaitForUserInput = false;
              programmingMode = false;  //not related to this section of MODE. Only relevant to EngineerMode
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

void RunMotor() {
  if (!WaitForUserInput) {
    if (runningStep < TotalStepCount && StepMoves[runningStep] >= 0) {
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
      programmingMode = true;
      WaitForUserInput = true;
      startTime = millis();

      StopMoving();
      RightSignal = false;
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
#pragma endregion

void FlashTurnLights() {
  //rightTurn

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
  digitalWrite(stopLigtsPin, LOW);

  int duration = StepDurations[runningStep];
  int lastHalfLimit = duration - waitInterval;
  int dur = waitInterval;
  for (int i = 0; i < duration; i++) {
    //ON
    digitalWrite(p_motorA1, LOW);
    digitalWrite(p_motorA2, HIGH);
    digitalWrite(p_motorB1, HIGH);
    digitalWrite(p_motorB2, LOW);
    delay(10);
    //OFF
    digitalWrite(p_motorA1, LOW);
    digitalWrite(p_motorA2, LOW);
    digitalWrite(p_motorB1, LOW);
    digitalWrite(p_motorB2, LOW);
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

  RightSignal = true;
}
void TurnLeft() {
  digitalWrite(BackwardMovePin, HIGH);
  int duration = StepDurations[runningStep];
  int lastHalfLimit = duration - waitInterval;
  int dur = waitInterval;
  for (int i = 0; i < duration; i++) {
    //ON
    digitalWrite(p_motorA1, HIGH);
    digitalWrite(p_motorA2, LOW);
    digitalWrite(p_motorB1, LOW);
    digitalWrite(p_motorB2, HIGH);
    delay(10);
    //OFF
    digitalWrite(p_motorA1, LOW);
    digitalWrite(p_motorA2, LOW);
    digitalWrite(p_motorB1, LOW);
    digitalWrite(p_motorB2, LOW);
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

  RightSignal = false;
}
void GoForward() {
  BackwardMoveSignal = false;
  int duration = StepDurations[runningStep];
  int lastHalfLimit = duration - waitInterval;
  int dur = waitInterval;
  for (int i = 0; i < duration; i++) {
    //ON
    digitalWrite(p_motorA1, LOW);
    digitalWrite(p_motorA2, HIGH);
    digitalWrite(p_motorB1, LOW);
    digitalWrite(p_motorB2, HIGH);
    delay(10);
    //OFF
    digitalWrite(p_motorA1, LOW);
    digitalWrite(p_motorA2, LOW);
    digitalWrite(p_motorB1, LOW);
    digitalWrite(p_motorB2, LOW);
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
    digitalWrite(p_motorA1, HIGH);
    digitalWrite(p_motorA2, LOW);
    digitalWrite(p_motorB1, HIGH);
    digitalWrite(p_motorB2, LOW);
    delay(10);
    //OFF
    digitalWrite(p_motorA1, LOW);
    digitalWrite(p_motorA2, LOW);
    digitalWrite(p_motorB1, LOW);
    digitalWrite(p_motorB2, LOW);
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
}
void StopMoving() {
  digitalWrite(p_motorA1, LOW);
  digitalWrite(p_motorA2, LOW);
  digitalWrite(p_motorB1, LOW);
  digitalWrite(p_motorB2, LOW);
  BackwardMoveSignal = false;
  digitalWrite(stopLigtsPin, HIGH);
}

//clear steps
void ClearSteps() {
  for (int i = 0; i < TotalStepCount; i++) {
    StepMoves[i] = -1;
    StepDurations[i] = -1;
    StepTones[i] = -1;
  }
  HeadLights = false;
  digitalWrite(p_clearDataLed, HIGH);
  PlayTone(NOTE_A5);
  delay(200);
  digitalWrite(p_clearDataLed, LOW);
  noTone(speakerPin);
}
