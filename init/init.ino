/*************************************************************************
* Arduino OBD-II Data Logger
* Distributed under GPL v2.0
* Copyright (c) 2013 Stanley Huang <stanleyhuangyc@gmail.com>
* All rights reserved.
*************************************************************************/

#include <Arduino.h>
#include <OBD.h>
#include <SD.h>
#include <Wire.h>

#define SD_CS_PIN 10

COBD obd;

uint32_t filesize = 0;
uint32_t datacount = 0;


static uint32_t lastTime;
static int startDistance = 0;


void setup()
{
    // start serial communication at the adapter defined baudrate
    Serial.begin(38400);
    //OBDUART.begin(OBD_SERIAL_BAUDRATE);


    // init SD card
    pinMode(SD_CS_PIN, OUTPUT);


    // initiate OBD-II connection until success
    pinMode(6, OUTPUT);
    digitalWrite(6, HIGH);
    while (!obd.Init());
    digitalWrite(6, LOW);
    obd.ReadSensor(PID_DISTANCE, startDistance);

    delay(1000);

    lastTime = millis();
}

static char buf[32];
static int len = 0;
static byte pid = 0;
static int value = 0;

void RetrieveData(byte id)
{
    obd.Query(id);

    // flush data in the buffer
    if (len > 0) {
        if (datacount % 100 == 99) {
            sprintf(buf, "%4u KB", (int)(filesize >> 10));
        }

        switch (pid) {
        case PID_RPM:
            sprintf(buf, "%4d", value);
            break;
        case PID_SPEED:
            sprintf(buf, "%3d", value);
            break;
        case PID_DISTANCE:
            if (value >= startDistance) {
                sprintf(buf, "%d km   ", value - startDistance);
            }
            break;
        }
    }

    if (obd.GetResponse(id, value)) {
        uint32_t curTime = millis();
        len = sprintf(buf, "%d,%X,%d\n", (int)(curTime - lastTime), id, value);
        filesize += len;
        datacount++;
        lastTime = curTime;
        pid = id;
        return;
    }
    len = 0;
}

void loop()
{
    static char count = 0;

    switch (count++) {
    case 0:
    case 64:
    case 128:
    case 192:
        RetrieveData(PID_DISTANCE);
        break;
    case 4:
        RetrieveData(PID_COOLANT_TEMP);
        break;
    case 20:
        RetrieveData(PID_INTAKE_TEMP);
        break;
    }

    RetrieveData(PID_RPM);
    RetrieveData(PID_SPEED);
    RetrieveData(PID_THROTTLE);
    RetrieveData(PID_ABS_ENGINE_LOAD);
    
    
    digitalWrite(6, HIGH);
    delay(100);
    digitalWrite(6, LOW);
    delay(100);

    if (obd.errors > 2) {
        digitalWrite(SD_CS_PIN, LOW);
        for (int i = 0; !obd.Init(); i++) {
          
        }
        digitalWrite(SD_CS_PIN, HIGH);
        delay(1000);
        count = 0;
    }
}
  
