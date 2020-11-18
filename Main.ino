#include "src/Network.h"
#include "src/Ota.h"

#define BAUD_RATE 9600

Network network;
Ota ota;

void setup() {
    Serial.begin(BAUD_RATE);
    Serial.println("Booting");
    
    network.init();
    ota.init();
    
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(network.getIpAddress());

    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    ota.handle();

    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
}