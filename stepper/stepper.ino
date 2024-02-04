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

constexpr uint8_t RECV_PIN {9};
constexpr uint8_t LED {12};

uint16_t state = 0;
uint16_t previousState = 0;

const uint16_t onoffBtn = 0x40; //On Off
const uint16_t playBtn = 0x41; //Play/Stop
const uint16_t fasterBtn = 0x17; //Quick
const uint16_t slowerBtn = 0x13; //Slow
const uint16_t rightBtn = 0x5D; //Right
const uint16_t leftBtn = 0x5C; //Left

bool running = false;
int speedDelay = 10;

void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(m1,OUTPUT);
  pinMode(m2,OUTPUT);
  pinMode(m3,OUTPUT);
  pinMode (LED, OUTPUT);    // Lüfter

  digitalWrite(m1, HIGH);
  digitalWrite(m2, HIGH);
  digitalWrite(m3, HIGH);
  

  Serial.begin(9600);
 
  IrReceiver.begin(RECV_PIN);
  Serial.print(F("Ready to receive IR signals at pin "));
  Serial.println(RECV_PIN);
}
void loop() {
  state = irReceive();
    if(state > 0 && previousState != state && (state != fasterBtn || state != slowerBtn))
    {      
      switch (state)
      {
        case onoffBtn :
          digitalWrite (LED, !digitalRead(LED));
          break;
        case leftBtn :
          leftDirection();
          break;
        case rightBtn:
          rightDirection();
          break;
        case playBtn:
          running = !running;          
          break;
        case fasterBtn:
          speedDelay = speedDelay - 1;
          break;
        case slowerBtn:
          speedDelay = speedDelay + 1;
        break;
      }
      Serial.print("speedDelay: " + speedDelay);
      previousState = state;
    }

    if(running){
      Run();
    }
}



void rightDirection()
{
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
}
void leftDirection()
{
  digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
}

void Run(){
  if(true)
  {    // Makes 200 pulses for making one full cycle rotation
    
      digitalWrite(stepPin,HIGH); 
      delayMicroseconds(speedDelay);    // by changing this time delay between the steps we can change the rotation speed
      digitalWrite(stepPin,LOW); 
      delayMicroseconds(speedDelay); 
      
  }  
}

uint16_t irReceive() {
  uint16_t received{0};
  
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