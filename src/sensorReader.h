#pragma once
#include "Arduino.h"
#include "pin.h"


class SensorReader{
    int calibrationLow[5] = {0, 0, 0, 0, 0};
    int calibrationHigh[5] = {4095, 4095, 4095, 4095, 4095};
    
    private:
    int readRaw(int sensor){
        switch(sensor){
            case 0:
                return analogRead(PIN_SENSOR_LL);    
            case 1:
                return analogRead(PIN_SENSOR_L); 
            case 2:
                return analogRead(PIN_SENSOR_C); 
            case 3:
                return analogRead(PIN_SENSOR_R); 
            case 4:
                return analogRead(PIN_SENSOR_RR); 
            default:
                return -1;   
        }
    }

    int readSensor(int sensor){
        return map(readRaw(sensor), calibrationLow[sensor], calibrationHigh[sensor], 0, 4000);
    }


    public:
    SensorReader(){}

    void begin(){
        pinMode(PIN_SENSOR_ENABLE, OUTPUT);
        pinMode(PIN_SENSOR_LL, INPUT);
        pinMode(PIN_SENSOR_L, INPUT);
        pinMode(PIN_SENSOR_C, INPUT);
        pinMode(PIN_SENSOR_R, INPUT);
        pinMode(PIN_SENSOR_RR, INPUT);
        digitalWrite(PIN_SENSOR_ENABLE, HIGH);
    }


    // Retorna um int de -1000 a 1000 (aproximadamente) com a leitura dos sensores
    // -1000 = linha à esquerda
    //  1000 = linha à direita
    int getReading(){
        int weights[5] = {-10,-5, 0, 5, 10};
        // Faz uma media com pesos de cada sensor
        int weightedAverage = (weights[0] * readSensor(0) + weights[1] * readSensor(1) + weights[2] * readSensor(2) + weights[3] * readSensor(3) + weights[4] * readSensor(4)); //(weights[0] + weights[1] + weights[2] + weights[3] + weights[4]); 
        return weightedAverage;
    }
};