#ifndef SOCKET_H
#define SOCKET_H

#include "ArduinoJson.h"
#include "lib/mvhr-bypass-common/arduino-esp8266/State.h"
#include "lib/mvhr-bypass-common/arduino-esp8266/InitData.h"
#include <WebSocketsServer.h>

class Socket;

#include "SerialNetwork.h"

#define WEB_SOCKET_SERVER_PORT 25484

class Socket
{
private:
    static int clientNum;
    static SerialNetwork* serialNetwork;
    static WebSocketsServer webSocket;
    static void send(JsonObject& root);

public:
    static void init(SerialNetwork* serialNetwork);
    static void loop();
    static void prepareStateJsonObject(JsonObject& obj, State* state);
    static void prepareTemperaturesJsonObject(JsonObject& obj, Temperatures* temperatures);
    template <typename T>
    static void send(T* data);
    static void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);
};

template <>
void Socket::send<InitData>(InitData* initData);
template <>
void Socket::send<State>(State* state);
template <>
void Socket::send<Temperatures>(Temperatures* temperatures);

#endif