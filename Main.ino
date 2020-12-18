#include "src/Network.h"
#include "src/Ota.h"
#include "src/SerialNetwork.h"
#include "src/lib/mvhr-bypass-common/arduino-esp8266/LocalContract.h"
#include "src/lib/mvhr-bypass-common/arduino-esp8266/TransmissionPacket.h"
#include "src/Socket.h"

#define BAUD_RATE 9600
// #define BAUD_RATE 115200 // to be used for direct serial communication

// #define MEMORY_DEBUG

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

#ifdef MEMORY_DEBUG
extern "C" // https://stackoverflow.com/a/1041880
{
#include "user_interface.h"
}
unsigned long lastMemCheck = 0L;
#endif

void loop() {
    ota.handle();
    serialNetwork.handleOutstandingPackets();
    Socket::loop();

#ifdef MEMORY_DEBUG
    if (millis() - lastMemCheck > 1000)
    {
        printFreeMemory();
        lastMemCheck = millis();
    }
#endif
}

#ifdef MEMORY_DEBUG
void printFreeMemory()
{
    Serial1.print(F("Free memory: "));
    Serial1.print(system_get_free_heap_size());
    Serial1.println(F(" bytes"));
}
#endif