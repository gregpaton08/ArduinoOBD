#include <EEPROM.h>

void setup() {
 Serial.begin(38400); 
 for (int i = 0; i < 1024; ++i) {
   Serial.write(EEPROM.read(i));
   delay(100);
 }
}

void loop() {
  
}
