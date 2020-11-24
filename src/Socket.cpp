#include "Socket.h"

WebSocketsServer Socket::webSocket = WebSocketsServer(WEB_SOCKET_SERVER_PORT);
int Socket::clientNum = -1;
SerialNetwork* Socket::serialNetwork;

void Socket::init(SerialNetwork* serialNetwork)
{
    Socket::serialNetwork = serialNetwork;
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void Socket::loop()
{
    webSocket.loop();
}

void Socket::sendState(State* state)
{
    char response[20] = {};
    int length = sprintf(response, "%d %d %u %u %u %f", state->hrModeAuto, state->hrDisabled,
            state->intEvMin, state->extAdMin, state->extAdMax, state->hysteresis);
    webSocket.sendTXT(clientNum, response, length);
}

void Socket::webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial1.printf("[%u] Disconnected!\n", num);
        clientNum = -1;
        break;
    case WStype_CONNECTED:
    {
        IPAddress ip = webSocket.remoteIP(num);
        Serial1.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");

        clientNum = num;

        serialNetwork->requestState();
        break;
    }
    case WStype_TEXT:
        Serial1.printf("[%u] get Text: %s\n", num, payload);

        // send message to client
        // webSocket.sendTXT(num, "message here");

        // send data to all connected clients
        // webSocket.broadcastTXT("message here");
        break;
    case WStype_BIN:
        Serial1.printf("[%u] get binary length: %u\n", num, length);
        hexdump(payload, length);

        // send message to client
        // webSocket.sendBIN(num, payload, length);
        break;
    }
}