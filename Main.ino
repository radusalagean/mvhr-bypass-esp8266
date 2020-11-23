#include "src/Network.h"
#include "src/Ota.h"
#include "src/SerialNetwork.h"
#include "src/lib/mvhr-bypass-common/arduino-esp8266/LocalContract.h"
#include "src/lib/mvhr-bypass-common/arduino-esp8266/TransmissionPacket.h"

#define BAUD_RATE 9600
// #define BAUD_RATE 115200 // to be used for direct serial communication

Network network;
Ota ota;
SerialNetwork serialNetwork;

void setup() {
    Serial.begin(BAUD_RATE);
    Serial1.begin(BAUD_RATE);

    Serial1.println("Booting");
    
    network.init();
    ota.init();
    
    Serial1.println("Ready");
    Serial1.print("IP address: ");
    Serial1.println(network.getIpAddress());
}

unsigned long lastTime = 0UL;

void loop() {
    ota.handle();
    serialNetwork.handleOutstandingPackets();
    if (millis() - lastTime > 15000)
    {
        TransmissionPacket packet = {LOCAL_CONTRACT_CODE_REQUEST_STATE, 0, NULL};
        serialNetwork.sendPacket(packet);
        lastTime = millis();
    }
}