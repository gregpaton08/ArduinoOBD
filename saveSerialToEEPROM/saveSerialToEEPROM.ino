#include <EEPROM.h>

int address = 0;
bool prompt = false;

void setup() {
  Serial.begin(38400);
  pinMode(6, OUTPUT);
  /*digitalWrite(6, HIGH);
  for (;;) {
    Serial.write("AT Z\r");
    delay(500);
    while (Serial.available() > 0) {
      int c = Serial.read();
      if (c == '>') {
        break;
      }
      else {
        EEPROM.write(address, c);
        ++address;
      }
    } 
  }
  digitalWrite(6, LOW);
  delay(1000);*/
}

void loop() {
  if (false == prompt) {
    Serial.write("AT Z\r");
    delay(500);
  }
  while (Serial.available() > 0) {
    digitalWrite(6, HIGH);
    int c = Serial.read();
    if (c == '>') {
      prompt = !prompt;
      Serial.write("01 0c\r");
      delay(500);
      //c = Serial.read();
    }
    EEPROM.write(address, c);
    ++address;
    if (address > 1023) {
      address = 0;
    }
    delay(200);
  }
  digitalWrite(6, LOW);
}
