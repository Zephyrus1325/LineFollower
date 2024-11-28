#pragma once
#include "Arduino.h"
#include "defines.h"


class SensorReader{  
    private:
    int calibrationLow[7] = {0, 0, 0, 0, 0, 0, 0};
    int calibrationHigh[7] = {4095, 4095, 4095, 4095, 4095, 4095, 4095 };
    
    int average = 0;

    int biggestSensor = 3;
    const int totalSensors = 7;



    void calibrateSensors(){
        for(int sensor = 0; sensor < totalSensors; sensor++){
            int average = 0;
            const int totalSamples = 1000; // Numero de amostras usadas para calcular o valor minimo
            for(int i = 0; i < totalSamples; i++){
                int reading = readRaw(sensor);
                average += reading;
            }
            calibrationLow[sensor] = average/totalSamples;
        }
    }

    void checkBiggest(int biggestID){
        //if(abs(biggestID - biggestSensor) <= 1){
        //    biggestSensor = biggestID;
        //}
        if(biggestID != -10){
            biggestSensor = biggestID;
        }
        
    }


    int readRaw(int sensor){
        switch(sensor){
            case 0:
                return analogRead(PIN_SENSOR_LLL);    
            case 1:
                return analogRead(PIN_SENSOR_LL); 
            case 2:
                return analogRead(PIN_SENSOR_L); 
            case 3:
                return analogRead(PIN_SENSOR_C); 
            case 4:
                return analogRead(PIN_SENSOR_R); 
            case 5:
                return analogRead(PIN_SENSOR_RR); 
            case 6:
                return analogRead(PIN_SENSOR_RRR); 
            default:
                return -1;   
        }
    }

    int readSensor(int sensor){
        int maxValue = 2000;
        int reading = map(readRaw(sensor), calibrationLow[sensor], calibrationHigh[sensor], 0, maxValue);
        reading = constrain(reading, 0, maxValue);
        return reading; 
    }


    public:
    SensorReader(){}

    void begin(){
        analogReadResolution(12);
        analogSetWidth(12);
        analogSetClockDiv(1);
        analogSetAttenuation(ADC_11db);
        pinMode(PIN_SENSOR_ENABLE, OUTPUT);
        pinMode(PIN_SENSOR_LLL, INPUT);
        pinMode(PIN_SENSOR_LL, INPUT);
        pinMode(PIN_SENSOR_L, INPUT);
        pinMode(PIN_SENSOR_C, INPUT);
        pinMode(PIN_SENSOR_R, INPUT);
        pinMode(PIN_SENSOR_RR, INPUT);
        pinMode(PIN_SENSOR_RRR, INPUT);
        digitalWrite(PIN_SENSOR_ENABLE, HIGH);
        calibrateSensors();
    }

    void update(){
        checkBiggest(getBiggestSensor());
        //Serial.print(biggestSensor);
        //Serial.print(" | ");
    }


    // Retorna um int de -1000 a 1000 (aproximadamente) com a leitura dos sensores
    // -1000 = linha à esquerda
    //  1000 = linha à direita
    int getReading(){
        int weights[5] = {-10,-5, 0, 5, 10};
        // Faz uma media com pesos de cada sensor
        int weightedAverage = (weights[0] * readSensor(0) + weights[1] * readSensor(1) + weights[2] * readSensor(2) + weights[3] * readSensor(3) + weights[4] * readSensor(4)); //(weights[0] + weights[1] + weights[2] + weights[3] + weights[4]); 
        average = average * 0.995f + weightedAverage * 0.005f;
        return average;
    }


    // Retorna o maior sensor lido no momento
    // Retorno de -10 quer dizer que nenhum sensor está sendo relevamentemente grande
    int getBiggestSensor(){
        int biggestValue = 200;
        int biggestId = -10;
        int average = 0;
        for(int i = 0; i < totalSensors; i++){
            Serial.print(readSensor(i));
            Serial.print(" | ");
            
            if(readSensor(i) > biggestValue){
                biggestValue = readSensor(i);
                biggestId = i;
            }
            //if(readSensor(i) == 2000){
            //    average += i;
            //}
        }
        //if(biggestValue == 2000){
        //    biggestId = average / totalSensors;
        //}
        //Serial.print(average);
        //Serial.print(" | ");
        //Serial.println(biggestId);
        return biggestId;
    }
    
    int getActive(){
        return biggestSensor;
    }

    void printRaw(){
        for(int i = 0; i < totalSensors; i++){
            Serial.print(readRaw(i));
            Serial.print(" | ");
        }
    }

    void printReading(){
        for(int i = 0; i < totalSensors; i++){
            Serial.print(readSensor(i));
            Serial.print(" | ");
        }
    }

    // Retorna o ultimo sensor com o maior pico
    int getBiggest(){
        return biggestSensor;
    }

    int getGlobalAverage(){
        int average = 0;
        for(int i = 0; i < totalSensors; i++){
            average += readSensor(i);
        }
        average /= totalSensors;
        return average;
    }
    
    int findCentroid(){
        const int multiplier = 1000;
        int centroid = 0;
        int torque = 0;
        int mass = 1;
        for(int channel = 1; channel < totalSensors+1; channel++){
            int sensorValue = readSensor(channel-1);
            mass += sensorValue;
            torque += sensorValue*channel;
        }
        centroid = (torque * multiplier)/ mass;
        // Se o sensor estiver fora da linha       
        if(getBiggestSensor() < 0){
            centroid = (biggestSensor + 1) * multiplier;
            //centroid = biggestSensor < 3 ? multiplier : biggestSensor > 3 ? 7 * multiplier : biggestSensor;
        }
        return centroid;
    } 
};