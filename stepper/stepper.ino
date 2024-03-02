/*   
 *   Basic example code for controlling a stepper without library
 *      
 *   by Dejan, https://howtomechatronics.com
 */
#include <SoftwareSerial.h>
#include <IRremote.hpp>

// defines pins
#define stepPin 2
#define dirPin 5
#define m1 6
#define m2 7
#define m3 8

//control input timing
long startTime;
long elapsedTime;

constexpr uint8_t RECV_PIN{ 9 };
constexpr uint8_t LED{ 12 };

uint16_t state = 0;
uint16_t previousState = 0;

//20 button remote control mapping
const uint16_t onoffBtn = 0x45;   //On Off
const uint16_t playBtn = 0x15;    //Play/Stop
const uint16_t fasterBtn = 0x40;  //Plus
const uint16_t slowerBtn = 0x19;  //Minus
const uint16_t rightBtn = 0x9;    //Right, Forward
const uint16_t leftBtn = 0x7;     //Left, Backward
const uint16_t cBtn = 0xD;        // change step precision?
const uint16_t menuBtn = 0x47;    //Menu, not used???
const uint16_t testBtn = 0x44;    //demo mode? replay all modes, each mode for 3 seconds
const uint16_t returnBtn = 0x43;  //previous mode
//numeric buttons
const uint16_t zeroBtn = 0x16;
const uint16_t oneBtn = 0xC;
const uint16_t twoBtn = 0x18;
const uint16_t threeBtn = 0x5E;
const uint16_t fourBtn = 0x8;
const uint16_t fiveBtn = 0x1C;
const uint16_t sixBtn = 0x5A;
const uint16_t sevenBtn = 0x42;
const uint16_t eightBtn = 0x52;
const uint16_t nineBtn = 0x4A;

bool running = false;
int speedDelay = 10;
int lastSpeedDelay = 10;
int stepLevel = 3;

bool testMode = false;

void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(LED, OUTPUT);  // Lüfter

  digitalWrite(m1, HIGH);
  digitalWrite(m2, HIGH);
  digitalWrite(m3, HIGH);

  startTime = millis();

  Serial.begin(9600);

  IrReceiver.begin(RECV_PIN);
  Serial.print(F("Ready to receive IR signals at pin "));
  Serial.println(RECV_PIN);
}
void loop() {

  elapsedTime = millis() - startTime;

  if (elapsedTime > 300) {
    state = irReceive();
    if (state > 0) {
      switch (state) {
        case cBtn:
          ChangeStepLevel();
          break;
        case onoffBtn:
          digitalWrite(LED, !digitalRead(LED));
          break;
        case leftBtn:
          leftDirection();
          break;
        case rightBtn:
          rightDirection();
          break;
        case playBtn:
          running = !running;
          break;
        case fasterBtn:
          speedDelay = speedDelay - 10;
          if (speedDelay < 4) {
            speedDelay = 4;
          }
          break;
        case slowerBtn:
          speedDelay = speedDelay + 10;
          break;
        case zeroBtn:
          speedDelay = 5000;
          break;
        case oneBtn:
          speedDelay = 4500;
          break;
        case twoBtn:
          speedDelay = 4000;
          break;
        case threeBtn:
          speedDelay = 3500;
          break;
        case fourBtn:
          speedDelay = 3000;
          break;
        case fiveBtn:
          speedDelay = 2500;
          break;
        case sixBtn:
          speedDelay = 2000;
          break;
        case sevenBtn:
          speedDelay = 1500;
          break;
        case eightBtn:
          speedDelay = 1000;
          break;
        case nineBtn:
          speedDelay = 500;
          break;
        case testBtn:
          testMode = true;
          running = true;
          break;
      }
      Serial.print("speedDelay: ");
      Serial.println(speedDelay);
      previousState = state;
      state = 0;
      lastSpeedDelay = speedDelay;
      startTime = millis();
    }
  }

  if (running) {
    if (testMode) {
      elapsedTime = millis() - startTime;
      if (elapsedTime < 5000) {
        speedDelay = 10;
      } else if (elapsedTime < 10000) {
        speedDelay = 5000;
      } else {
        delay(2000);
        testMode = false;
        speedDelay = lastSpeedDelay;
      }
    }
    Run();
  }
}

void ChangeStepLevel() {
  stepLevel++;
  if (stepLevel > 3) {
    stepLevel = 0;
  }

  switch (stepLevel) {
    case 0:
      digitalWrite(m1, LOW);
      digitalWrite(m2, LOW);
      digitalWrite(m3, LOW);
      break;
    case 1:
      digitalWrite(m1, HIGH);
      digitalWrite(m2, LOW);
      digitalWrite(m3, LOW);
      break;
    case 2:
      digitalWrite(m1, HIGH);
      digitalWrite(m2, HIGH);
      digitalWrite(m3, LOW);
      break;
    case 3:
      digitalWrite(m1, HIGH);
      digitalWrite(m2, HIGH);
      digitalWrite(m3, HIGH);
      break;
  }
}

void rightDirection() {
  digitalWrite(dirPin, HIGH);  // Enables the motor to move in a particular direction
}
void leftDirection() {
  digitalWrite(dirPin, LOW);  // Enables the motor to move in a particular direction
}

void Run() {
  if (true) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speedDelay);  // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speedDelay);
  }
}


uint16_t irReceive() {
  uint16_t received{ 0 };

  if (IrReceiver.decode()) {
    IrReceiver.printIRResultShort(&Serial);
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      // We have an unknown protocol here, print more info
      IrReceiver.printIRResultRawFormatted(&Serial, true);
    }
    if (IrReceiver.decodedIRData.protocol == NEC) {
      received = IrReceiver.decodedIRData.command;
      Serial.print("Command: 0x");
      Serial.println(received, HEX);
    }
    IrReceiver.resume();
  }
  return received;
}