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

void beginHandler(){
    sensor.begin();
    leftMotor.begin();
    rightMotor.begin();
    // Configurações possiveis
    // Fast af (Colocar rodas melhores)
    // Kp 0.03 Kd -42.0
    // Forward = 70 - abs(motorPID.getOutput() * 0.6f);
    
    // Normal boi
    // Kp 0.02 Kd -42.0
    // Forward = 50 - abs(motorPID.getOutput() * 0.6f);
    motorPID.setKp(0.032f);
    motorPID.setKd(-42.0f);
    motorPID.setSetpoint(4000);
}

void setMotorSpeed(int forward, int turnage){
    leftMotor.setSpeed(forward - turnage);
    rightMotor.setSpeed(forward + turnage);
}

void updateMotor(){
    // Atualizar PID
    motorPID.update();
    sensor.update(); // Atualiza Sensores

    motorPID.setActualValue(sensor.findCentroid());
    int forward = 55 - abs(motorPID.getOutput() * 0.6f);
    int rotation = motorPID.getOutput();
    Serial.print(forward);
    Serial.print(" | ");
    Serial.println(rotation);

    setMotorSpeed(forward, rotation);
    //setMotorSpeed(forward, rotation < 0 ? rotation * 1.1f : rotation); // Dir | Esquerda
}