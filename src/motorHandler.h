#pragma once
#include "Arduino.h"
#include "sensorReader.h"
#include "motor.h"
#include "PID.h"
#include "timer.h"


PID motorPID;
Motor leftMotor(PIN_MOTOR_LEFT_A, PIN_MOTOR_LEFT_B, PIN_MOTOR_LEFT_PWM);
Motor rightMotor(PIN_MOTOR_RIGHT_A, PIN_MOTOR_RIGHT_B, PIN_MOTOR_RIGHT_PWM);
SensorReader sensor;

timer leftCalibrationTimer{0,2000,true,true,true};
timer rightCalibrationTimer{0,2000,true,true,true};
timer PIDtimer{0,5,true,true,true};
timer turningTimer{0, 100,false,true,true};
timer printTimer{0,1,true,true,true};
int testLeft = 0;
int testRight = 0;
bool mode270 = false; // Determina se está em uma situação que pode havar um 270°
bool holdOn = false; // Determina se está em um arco de um 270°
bool foundU = false; // Determina se a primeira curva do 270 foi encontrada
void beginHandler(){
    sensor.begin();
    leftMotor.begin();
    rightMotor.begin();
    //motorPID.setKp(0.025f);
    motorPID.setKp(0.02f);
    motorPID.setKd(2.0f);
    motorPID.setSetpoint(4000);
}

void setMotorSpeed(int forward, int turnage){
    leftMotor.setSpeed((forward - turnage) * 1);
    rightMotor.setSpeed(forward + turnage);
}

void updateMotor(bool isRacing){
    // Atualizar PID
    motorPID.update();
    sensor.update(); // Atualiza Sensores

    // Checa se passou por um cruzamento
    //if(sensor.getGlobalAverage() > 1400){
    //    mode270 = true;
    //}
    //// Checa se está com a possibilidade de um 270 nos proximos instantes
    //if(mode270 && turningTimer.CheckTime()){
    //    // Se ficar fora do arco de uma curva, ou seja
    //    // Se está curvando em um 270
    //    if(sensor.getGlobalAverage() < 50 && (sensor.getBiggest() == 0 || sensor.getBiggest() == 6)){
    //        holdOn = true;
    //    }
    //    // Se encontrar a primeira linha
    //    if(holdOn && (sensor.getBiggestSensor() == 0 || sensor.getBiggestSensor() == 6) && sensor.getGlobalAverage() > 200){
    //        foundU = true;
    //    }
    //    
    //    // Se estiver passado completamente da primeira linha, desligue o modo 270
    //    if(foundU && sensor.getGlobalAverage() < 50){
    //        holdOn = false;
    //        foundU = false;
    //        mode270 = false;
    //    }
    //}

    // Ignore a primeira linha completamente se não estiver no modo270 com foundU ativado
    //if(!foundU){
    //    motorPID.setActualValue(sensor.findCentroid());
    //}
    motorPID.setActualValue(sensor.findCentroid());
    int forward = 50 - abs(motorPID.getOutput() * 0.2);
    int rotation = motorPID.getOutput();

    if(printTimer.CheckTime() && false){
        Serial.print(sensor.getGlobalAverage());
        Serial.print(" | ");
        Serial.print(sensor.getBiggestSensor());
        Serial.print(" | ");
        Serial.print(sensor.getBiggest());
        Serial.print(" | ");
        Serial.print(mode270);
        Serial.print(" | ");
        Serial.print(holdOn);
        Serial.print(" | ");
        Serial.println(foundU);
        
    }
    
    setMotorSpeed(forward, rotation);
}