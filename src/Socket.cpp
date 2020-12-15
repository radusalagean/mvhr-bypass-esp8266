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

// SEND

void Socket::send(JsonObject& root)
{
    int outputLength = measureJson(root);
    char* jsonString = new char[outputLength + 1];
    serializeJson(root, jsonString, outputLength + 1);
    webSocket.sendTXT(clientNum, jsonString, outputLength);
    delete[] jsonString;
}

void Socket::prepareStateJsonObject(JsonObject& obj, State* state)
{
    obj["hrModeAuto"] = state->hrModeAuto;
    obj["hrDisabled"] = state->hrDisabled;
    obj["intEvMin"] = state->intEvMin;
    obj["extAdMin"] = state->extAdMin;
    obj["extAdMax"] = state->extAdMax;
    obj["hysteresis"] = state->hysteresis;
}

void Socket::prepareTemperaturesJsonObject(JsonObject& obj, Temperatures* temperatures)
{
    obj["extEv"] = temperatures->extEv;
    obj["extAd"] = temperatures->extAd;
    obj["intAd"] = temperatures->intAd;
    obj["intEv"] = temperatures->intEv;
}

template <>
void Socket::send(InitData* initData)
{
    const int capacity = JSON_OBJECT_SIZE(4 + STATE_NUM_OF_FIELDS + TEMPERATURES_NUM_OF_FIELDS);
    JsonObject root = StaticJsonDocument<capacity>().to<JsonObject>();
    root[SOCKET_KEY_EVENT] = SOCKET_EVENT_RESPONSE_INIT_DATA;
    JsonObject dataObj = root.createNestedObject(SOCKET_KEY_DATA);
    JsonObject stateObj = dataObj.createNestedObject(SOCKET_KEY_STATE);
    prepareStateJsonObject(stateObj, &initData->state);
    JsonObject temperaturesObj = dataObj.createNestedObject(SOCKET_KEY_TEMPERATURES);
    prepareTemperaturesJsonObject(temperaturesObj, &initData->temperatures);
    send(root);
}

template <>
void Socket::send(State* state)
{
    const int capacity = JSON_OBJECT_SIZE(3 + STATE_NUM_OF_FIELDS);
    JsonObject root = StaticJsonDocument<capacity>().to<JsonObject>();
    root[SOCKET_KEY_EVENT] = SOCKET_EVENT_RESPONSE_STATE;
    JsonObject dataObj = root.createNestedObject(SOCKET_KEY_DATA);
    prepareStateJsonObject(dataObj, state);
    send(root);
}

template <>
void Socket::send(Temperatures* temperatures)
{
    const int capacity = JSON_OBJECT_SIZE(3 + TEMPERATURES_NUM_OF_FIELDS);
    JsonObject root = StaticJsonDocument<capacity>().to<JsonObject>();
    root[SOCKET_KEY_EVENT] = SOCKET_EVENT_RESPONSE_TEMPERATURES;
    JsonObject dataObj = root.createNestedObject(SOCKET_KEY_DATA);
    prepareTemperaturesJsonObject(dataObj, temperatures);
    send(root);
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
        clientNum = num;
        serialNetwork->requestInitData();
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