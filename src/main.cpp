#include <Arduino.h>
#include "util.h"
#include "sensorReader.h"

SensorReader sensor;

void setup() {
    Serial.begin(115200);
    sensor.begin();
}

void loop() {
  //readAll();
  Serial.println(sensor.getReading());
}