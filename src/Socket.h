#ifndef SOCKET_H
#define SOCKET_H

#include <WebSocketsServer.h>
#include "lib/mvhr-bypass-common/arduino-esp8266/State.h"
#include "ArduinoJson.h"

class Socket;

#include "SerialNetwork.h"

#define WEB_SOCKET_SERVER_PORT 25484

class Socket
{
private:
    static int clientNum;
    static SerialNetwork* serialNetwork;
    static WebSocketsServer webSocket;

public:
    static void init(SerialNetwork* serialNetwork);
    static void loop();
    static void sendState(State* state);
    static void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);

};

#endif