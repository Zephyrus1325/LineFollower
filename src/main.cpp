#include <Arduino.h>
#include "util.h"
#include "sensorReader.h"
#include "motor.h"
SensorReader sensor;
Motor leftMotor(PIN_MOTOR_LEFT_A, PIN_MOTOR_LEFT_B, PIN_MOTOR_LEFT_PWM);
Motor rightMotor(PIN_MOTOR_RIGHT_A, PIN_MOTOR_RIGHT_B, PIN_MOTOR_RIGHT_PWM);

int biggestSensor = 2;

void checkBiggest(int biggestID){
  if(abs(biggestID - biggestSensor) <= 1){
    biggestSensor = biggestID;
  }
}


void setup() {
    Serial.begin(115200);
    sensor.begin();
    leftMotor.begin();
    rightMotor.begin();
}

void loop() {
  int baseSpeed = 180;
  int speedOffset = 150;
  //readAll();
  //Serial.println(sensor.getReading());
  //leftMotor.setSpeed(sensor.getReading()+5000);
  //rightMotor.setSpeed(-sensor.getReading()+5000);
  int leftSpeed = 0;
  int rightSpeed = 0;
  checkBiggest(sensor.getBiggest());
  switch(biggestSensor){
    case 0:
      leftSpeed = (-baseSpeed + speedOffset);
      rightSpeed = (baseSpeed + speedOffset);
      break;
    case 1:
      leftSpeed = (speedOffset);
      rightSpeed = (baseSpeed + speedOffset);
      break;
    case 2:
      leftSpeed = (baseSpeed);
      rightSpeed = (baseSpeed);
      break;
    case 3:
      leftSpeed = (baseSpeed + speedOffset);
      rightSpeed = (speedOffset);
      break;
    case 4:
      leftSpeed = (baseSpeed + speedOffset);
      rightSpeed = (-baseSpeed + speedOffset);
      break;
  }
  leftMotor.setSpeed(leftSpeed);
  rightMotor.setSpeed(rightSpeed);
  
  Serial.print(biggestSensor);
  Serial.print(" | ");
  Serial.print(leftSpeed);
  Serial.print(" | ");
  Serial.println(rightSpeed);
}