#include <Arduino.h>
#include "Mux.h"

using namespace admux;
/*
 * Creates a Mux instance.
 *
 * 1st argument is the SIG (signal) pin (Arduino analog input pin A0).
 * 2nd argument is the S0-S3 (channel control) pins (Arduino pins 8, 9, 10, 11).
 */
Mux mux1(Pin(A0, INPUT, PinType::Analog), Pinset(4, 5, 6, 7));
Mux mux2(Pin(A1, INPUT, PinType::Analog), Pinset(4, 5, 6, 7));


int tones[] = {
  294, 330, 349, 392, 440, 494, 523, 
  587, 659, 698, 784, 880, 988, 1080, 
  1100, 1200, 1300, 1400, 1500, 1600,
  1700, 1800, 1900, 2000, 2100, 2200,
  2300, 2400, 2500, 2600
};

bool toneOn = false;
int currenKey = -1;

void setup() {

  pinMode(3, OUTPUT);
  pinMode(12, OUTPUT);
  // Serial port initialization.
  Serial.begin(9600);
  while (!Serial) /* Waits for serial port to connect (needed for Leonardo only) */
    ;
}

/**
 * Reads the 16 channels and reports on the serial monitor the corresponding
 * value read by the A/D converter.
 */
void loop() {
  int data1;
  int data2;
  double convertedData1;
  double convertedData2;
  for (byte i = 0; i < 16; i++) {
    
      data1 = mux1.read(i) /* Reads from channel i (returns a value from 0 to 1023) */;
      data2 = mux2.read(i) /* Reads from channel i (returns a value from 0 to 1023) */;
    
    convertedData1 = (double)(data1)*100 / 1023;
    convertedData2 = (double)(data2)*100 / 1023;

    if (convertedData1 > 99) {
      Serial.print("at channel ");
      Serial.print(i);
      Serial.print(" is at ");
      Serial.print(convertedData1);
      Serial.println("%%");
      toneOn = true;
      tone(2, tones[i]);
      currenKey = i;
    } else if (currenKey == i && convertedData1 < 80) {
      noTone(2);
      currenKey = -1;
    }
    if (convertedData2 > 99) {
      Serial.print("at channel ");
      Serial.print(i + 16);
      Serial.print(" is at ");
      Serial.print(convertedData2);
      Serial.println("%%");
      toneOn = true;
      tone(2, tones[i + 16]);
      currenKey = i ;
    } else if (currenKey == i && convertedData2 < 80) {
      noTone(2);
      currenKey = -1;
    }
  }
}

int getData() {
}