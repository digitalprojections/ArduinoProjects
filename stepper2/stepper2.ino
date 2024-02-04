/*  05/25/2019, by rickbme - DFS
 * 
 * Just a simple routine to turn off and on the driver to adjust the vref voltage
 * This way I could just push this code to the nano, adjust it, then push the main code back
 * Too lazy to integrate it into the main test code. :)  Besides, i'd end up hitting the wrong
 * key anyways!
 */

#define sleep_pwr 7


void setup() {
Serial.begin(9600);

pinMode(sleep_pwr, OUTPUT);

digitalWrite(sleep_pwr,LOW);


}

void loop() {
    Serial.println("Stepper Driver Board Adjustment"); 
    Serial.println("Select an Option");
    Serial.println("1. Turn OFF driver");   
    Serial.println("2. Turn ON driver to test Vref");
    while (Serial.available()==0);
    float driver_Check=Serial.parseFloat(); 
    if(driver_Check == 1){   
    digitalWrite(sleep_pwr,LOW);
    Serial.println();
    Serial.println("Stepper driver off");
    Serial.println();
    }
    if(driver_Check == 2){ 
      Serial.println("Remember to disconnect stepper before adjusting!!");
      digitalWrite(sleep_pwr,HIGH); 
      Serial.println();
      Serial.println("**************** Stepper driver ON!****************"); 
      Serial.println();
    }
}    
