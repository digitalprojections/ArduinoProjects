// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 13;    // the number of the LED pin
const int relayPin1 = 9; //relay
const int relayPin2 = 10; //relay

int mode = 0; // 0=rest, 1=moveRight; 2=moveLeft; 

// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);  
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
 Serial.begin(9600); // open the serial port at 9600 bps:
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    Serial.print("High");
    Serial.print("\n");
    delay(100);
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    digitalWrite(relayPin1, LOW);
    digitalWrite(relayPin2, LOW);
    ChangeMode();
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
    digitalWrite(relayPin1, HIGH);
    digitalWrite(relayPin2, HIGH);
  }

switch(mode){
  case 1:
  Serial.print("Right");
    Serial.print("\n");  
  break;
  case 2:
  Serial.print("Left");
    Serial.print("\n");
  break;
}
  delay(1000);
}


void ChangeMode()
{
  if(mode<2){    
    mode++;
    delay(1000);
  }
  else{
    mode = 0;
  }
}
