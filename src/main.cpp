#include <Arduino.h>
#include "util.h"
#include "motorHandler.h"
#include "webpage.h"



SensorReader sensor;
bool isRacing = false;

void setup() {
    Serial.begin(115200);
    sensor.begin();
    leftMotor.begin();
    rightMotor.begin();
	//beginWebPage();
	testLeft = 2;
	testRight = 2;
}

void loop() {
    sensor.update(); // Atualiza Sensores
	sensor.printRaw();
	Serial.println();
    //updateMotor(isRacing); // Faz umas checagens dos motores
	//leftMotor.setSpeed(200);
	//rightMotor.setSpeed(200);
	//Serial.print(testLeft);
	//Serial.print(" | ");
	//Serial.println(testRight);
    // Se estiver correndo, faz o pau torar
	if(isRacing){

    }
}