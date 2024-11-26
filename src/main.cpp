#include <Arduino.h>
#include "util.h"
#include "motorHandler.h"
//#include "webpage.h"




bool isRacing = true;

void setup() {
    Serial.begin(115200);
	beginHandler();
	//beginWebPage();
}

void loop() {
    
	//sensor.printRaw();
	//Serial.println("");
	//sensor.printReading();
	//Serial.println();
    updateMotor(isRacing); // Faz umas checagens dos motores
	
	//Serial.print(testLeft);
	//Serial.print(" | ");
	//Serial.println(testRight);
    // Se estiver correndo, faz o pau torar
	if(isRacing){
		
    }
}