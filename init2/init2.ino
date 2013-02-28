#include <Arduino.h>


void setup() {
  Serial.begin(38400);
  pinMode(6, OUTPUT);
}


void loop() {  
  Serial.write("ATZ\r");
  for (;;) {
    if (Serial.available()) {
       digitalWrite(6, HIGH);
       delay(100);
    } 
    else {
      break;
    }
    digitalWrite(6, LOW);
    delay(100);
  }
}
