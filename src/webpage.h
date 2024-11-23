#pragma once
#include "calibrationPage.h"
#include "defines.h"
#include "timer.h"
#include "ArduinoJSON.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

timer logUpdateTimer{0, 250, true, true, true};

AsyncWebServer server(80); // Declara um servidor e um websocket
AsyncWebSocket ws("/ws");
JsonDocument logDocument;



enum dataType {
    LOG,
    ENABLE_CALIBRATION,
    TEST,
    SET_PID,
    SET_THRESHOLDS
};

enum testType {
    LEFT_MOTOR,
    RIGHT_MOTOR,
    SENSOR_ARRAY
};  

void insertLog(String logName, String value){
    logDocument[logName] = value;
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}


// Handler de Mensagens recebidas do websocket
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    
    //Checa se recebeu um websocket valido, eu acho que é isso que esse if faz
    if(info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT){
        // Transforma os dados recebidos (text) em um Json (Json)
        JsonDocument json; 
        DeserializationError err = deserializeJson(json, data);
        // Em caso de erro de serialização
        if(err){
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(err.c_str());
            return;
        }
        const char *command = json["command"];
        if (strcmp(command, "setPID") == 0){
            const char *p = json["data"][0];
            const char *i = json["data"][1];
            const char *d = json["data"][2];
            String kp = p;
            String ki = i;
            String kd = d;
            motorPID.setKp(kp.toFloat());
            motorPID.setKi(ki.toFloat());
            motorPID.setKd(kd.toFloat());
        } else if (strcmp(command, "calibrate") == 0){
            const char *motor = json["data"];
            String motorSelected = motor;
            if(motorSelected.toInt() == 0){
                // Motor Esquerdo
                testLeft = 2;
            } else if(motorSelected.toInt() == 1){
                // Motor Direito
                testRight = 2;

            }
        } else if (strcmp(command, "test") == 0){  
        } else if (strcmp(command, "setValue") == 0){  
        }
    }
}

// Checa o que deve ser feito ao receber um evento do websocket
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}



void beginWebPage(){
    WiFi.mode(WIFI_AP);          // Inicia o ESP como um ACCESS POINT (ponto de acesso)
    WiFi.softAP(ssid, password); // Inicia o WIFI com as credenciais de rede
    Serial.println("Wifi Iniciado!");
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("Senha: ");
    Serial.println(ssid);
    Serial.print("IP: ");
    Serial.println(WiFi.softAPIP());
    ws.onEvent(onEvent);    // Define qual função deve rodar em um evento do WS
    server.addHandler(&ws); // Define qual é o handler de WS do servidor

    // Inicia o servidor em si
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
    });

    server.onNotFound(notFound);
    server.begin();
}

void updateWebPage(){
    if(logUpdateTimer.CheckTime()){
        char data[500]; // Cria um buffer de caracteres
        size_t len = serializeJson(logDocument, data); // Usa o buffer para escrever o JSON
        ws.textAll(data, len);  // Envia esse buffer no WS para todos os clientes
    }
}

