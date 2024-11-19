#include <Arduino.h>
#include <pin.h>


void readAll(){
    float readingLL = analogRead(PIN_SENSOR_LL);
    float readingL  = analogRead(PIN_SENSOR_L);
    float readingC  = analogRead(PIN_SENSOR_C);
    float readingR  = analogRead(PIN_SENSOR_R);
    float readingRR = analogRead(PIN_SENSOR_RR);

    Serial.print(readingLL);
    Serial.print(" | ");
    Serial.print(readingL);
    Serial.print(" | ");
    Serial.print(readingC);
    Serial.print(" | ");
    Serial.print(readingR);
    Serial.print(" | ");
    Serial.println(readingRR);

}