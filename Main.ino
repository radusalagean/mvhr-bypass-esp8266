#include "src/Network.h"
#include "src/Ota.h"
#include "src/SerialNetwork.h"
#include "src/lib/mvhr-bypass-common/arduino-esp8266/LocalContract.h"
#include "src/lib/mvhr-bypass-common/arduino-esp8266/TransmissionPacket.h"
#include "src/Socket.h"

#define BAUD_RATE 9600
// #define BAUD_RATE 115200 // to be used for direct serial communication

Network network;
Ota ota;
Socket socket;
SerialNetwork serialNetwork;

void setup() {
    Serial.begin(BAUD_RATE);
    Serial1.begin(BAUD_RATE);

    Serial1.println("Booting");
    
    network.init();
    ota.init();
    Socket::init(&serialNetwork);
    
    Serial1.println("Ready");
    Serial1.print("IP address: ");
    Serial1.println(network.getIpAddress());
}

void loop() {
    ota.handle();
    serialNetwork.handleOutstandingPackets();
    Socket::loop();
}