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


//const uint32_t s1 = 1082110031;
const uint16_t s1 = 0x40; //On Off
const uint16_t s1 = 0x41; //Play/Stop
const uint16_t s1 = 0x17; //Quick
const uint16_t s1 = 0x13; //Slow
const uint16_t s1 = 0x5D; //Right
const uint16_t s1 = 0x5C; //Left

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
  BTSerial.begin(38400); // HC-05 default speed in AT command more

  IrReceiver.begin(RECV_PIN);
  Serial.print(F("Ready to receive IR signals at pin "));
  Serial.println(RECV_PIN);
}
void loop() {
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 32000; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(10);    // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(10); 
  }
  delay(1000); // One second delay
  

  digitalWrite(dirPin,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 32000; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(10);
  }
  delay(1000);

     if (irReceive() == s1) {
    digitalWrite(LED,!digitalRead(LED));
    /*
    switch (state)
    {
      case ON :
        digitalWrite (LED, HIGH);
        state = OFF;
        break;
      case OFF :
        digitalWrite (LED, LOW);
        state = ON;
        break;
    }
    Serial.print(state);
    */
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