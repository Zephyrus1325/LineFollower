#pragma once
#include "Arduino.h"
#include "sensorReader.h"
#include "motor.h"
#include "PID.h"
#include "timer.h"

PID motorPID;
Motor leftMotor(PIN_MOTOR_LEFT_A, PIN_MOTOR_LEFT_B, PIN_MOTOR_LEFT_PWM);
Motor rightMotor(PIN_MOTOR_RIGHT_A, PIN_MOTOR_RIGHT_B, PIN_MOTOR_RIGHT_PWM);


timer leftCalibrationTimer{0,5000,true,true,true};
timer rightCalibrationTimer{0,5000,true,true,true};
int testLeft = 0;
int testRight = 0;

void updateMotor(bool isRacing){
    // Se não estiver em uma corrida, checar se há uma calibração ativa
    if(!isRacing && (testLeft || testRight)){
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
        leftMotor.setSpeed(0);
        rightMotor.setSpeed(0);
    } else {
        // Se estiver em uma corrida, deixa o pau torar

    }
}

void setMotorSpeed(int forward, int turnage){
    leftMotor.setSpeed(forward + turnage);
    rightMotor.setSpeed(forward - turnage);
}