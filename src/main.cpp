#include <Arduino.h>
#include "motorHandler.h"

void setup() {
    Serial.begin(115200);
	beginHandler(); // Inicia o Handler de Motores
}

void loop() {
    updateMotor(); // Atualiza o estado dos motores
}