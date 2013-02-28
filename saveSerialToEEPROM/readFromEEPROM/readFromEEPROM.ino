#include <EEPROM.h>

int a = 0;

void setup() {
 Serial.begin(38400); 
}

void loop() {
  Serial.println(EEPROM.read(a), DEC);
  ++a;
  if (a > 1023) {
    a = 0;
  }
  delay(100);
}
