/*
MIT License

Copyright (c) 2020 lewis he

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <Arduino.h>
#include <Wire.h>
#include "MAX30208.h"

MAX30208_Class sensor;

const uint8_t i2c_sda = 15;
const uint8_t i2c_scl = 13;

uint8_t checkAddr(void)
{
    Wire.beginTransmission(0x50);
    if (Wire.endTransmission() == 0) {
        return 0x50;
    }
    Wire.beginTransmission(0x51);
    if (Wire.endTransmission() == 0) {
        return 0x51;
    }
    return 0;
}

void setup()
{
    Serial.begin(115200);

    Wire.begin(i2c_sda, i2c_scl);

    uint8_t slave_address  = checkAddr();
    if (slave_address == 0) {
        Serial.println("Ooops, MAX30208 not detected ... Check your wiring!");
        while (1);
    }
    sensor.begin(Wire, slave_address);
    sensor.enableDATARDY();
    sensor.startConvert();
}

void loop()
{
    uint8_t mask = sensor.getINT();
    if (mask & MAX30208_INT_STATUS_TEMP_RDY) {
        uint8_t samplesTaken = 0;
        sensor.check();
        float t = sensor.readTemperature();
        DBG("RDY = samples:%u Temperature:%.2f", samplesTaken, t);
        sensor.startConvert();
    }

    delay(1000);
}
