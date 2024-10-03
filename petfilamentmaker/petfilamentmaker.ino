/*
This is a program to control
temperature, stepper motor and its speed and LCD display
based on input from user
Date: 2024 July
Author: Fuzalov Ahmadkhon
*/


int ThermistorPin = A7;
int Vo;
float R1 = 10000;
float logR2, R2, T, C;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

bool relayOff = false;

int relayPinFilamentMaker = 2;//relay 1
int relayPinFilamentJoiner = 3;//relay 2

void setup() {
  pinMode(relayPinFilamentMaker, OUTPUT);
  Serial.begin(9600);
  digitalWrite(relayPinFilamentMaker, LOW);
}

void loop() {

  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;
  T = (T * 9.0)/ 5.0 + 32.0;

  C = (T-32)*(5/9);

  Serial.print("Temperature: "); 
  Serial.print(T);
  //Serial.println(" C); 

  delay(1000);

  if (T > 392) {
      digitalWrite(relayPinFilamentMaker, LOW);
      Serial.println("relay off");     
  }
  else{
    if(T < 392)
    {
      digitalWrite(relayPinFilamentMaker, HIGH);
      Serial.println("relay on"); 
    }    
  }

}