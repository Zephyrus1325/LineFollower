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
timer printTimer{0,20,true,true,true};
int testLeft = 0;
int testRight = 0;

void beginHandler(){
    sensor.begin();
    leftMotor.begin();
    rightMotor.begin();
    //motorPID.setKp(0.025f);
    motorPID.setKp(0.02f);
    motorPID.setKd(0.0f);
    motorPID.setKd(0.0f);
    motorPID.setSetpoint(4000);
}

void setMotorSpeed(int forward, int turnage){
    leftMotor.setSpeed((forward - turnage) * 1);
    rightMotor.setSpeed(forward + turnage);
}

void updateMotor(bool isRacing){
    // Atualizar PID
    if(PIDtimer.CheckTime()){
        
    }
    motorPID.update();
    sensor.update(); // Atualiza Sensores
    // Se não estiver em uma corrida, checar se há uma calibração ativa
    if(!isRacing && (testLeft || testRight)){
        motorPID.setError(0);
        // Calibrar motor esquerdo
        if(testLeft && leftCalibrationTimer.CheckTime()){
            switch(testLeft){
                case 1:
                    leftMotor.setSpeed(-200);
                    break;
                case 2:
                    leftMotor.setSpeed(200);
                    break;
                default:
                    leftMotor.setSpeed(0);
                    break;
            }
            testLeft--;
        }
        // Calibrar motor direito
        if(testRight && rightCalibrationTimer.CheckTime()){
            switch(testRight){
                case 1:
                    rightMotor.setSpeed(-200);
                    break;
                case 2:
                    rightMotor.setSpeed(200);
                    break;
                default:
                    rightMotor.setSpeed(0);
                    break;
            }
            testRight--;
        }
    } else if(!isRacing){
        // Se não estiver em uma corrida, e nem calibrando nada, forçar os motores em zero
        motorPID.setError(0);
        leftMotor.setSpeed(0);
        rightMotor.setSpeed(0);
    } else {
        // Se estiver em uma corrida, deixa o pau torar
        float power = 1.07f;
        motorPID.setActualValue(sensor.findCentroid());
        //int forward = 70 - pow(1.06, abs(motorPID.getOutput()));
        int forward = 50;
        int rotation = (pow(power, abs(motorPID.getOutput())) - 1) * motorPID.getOutput()/(abs(motorPID.getOutput())+0.1) ;

        if(printTimer.CheckTime()){
            Serial.print(motorPID.getError());
            Serial.print(" | ");
            Serial.print(motorPID.getOutput());
            Serial.print(" | ");
            Serial.print(forward - rotation);
            Serial.print(" | ");
            Serial.println(forward + rotation);
        }
        
        setMotorSpeed(forward, rotation);
    }
}