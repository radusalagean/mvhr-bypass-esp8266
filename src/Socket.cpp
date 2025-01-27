#include "Socket.h"

WebSocketsServer Socket::webSocket = WebSocketsServer(WEB_SOCKET_SERVER_PORT);
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

void Socket::send(JsonObject& root, const int clientNum)
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

void Socket::send(const char* eventName, const int clientNum)
{
    const int capacity = JSON_OBJECT_SIZE(1);
    JsonObject root = StaticJsonDocument<capacity>().to<JsonObject>();
    root[SOCKET_KEY_EVENT] = eventName;
    send(root, clientNum);
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
    const int capacity = JSON_OBJECT_SIZE(2 + STATE_NUM_OF_FIELDS);
    JsonObject root = StaticJsonDocument<capacity>().to<JsonObject>();
    root[SOCKET_KEY_EVENT] = SOCKET_EVENT_RESPONSE_STATE;
    JsonObject dataObj = root.createNestedObject(SOCKET_KEY_DATA);
    prepareStateJsonObject(dataObj, state);
    send(root);
}

template <>
void Socket::send(Temperatures* temperatures)
{
    const int capacity = JSON_OBJECT_SIZE(2 + TEMPERATURES_NUM_OF_FIELDS);
    JsonObject root = StaticJsonDocument<capacity>().to<JsonObject>();
    root[SOCKET_KEY_EVENT] = SOCKET_EVENT_RESPONSE_TEMPERATURES;
    JsonObject dataObj = root.createNestedObject(SOCKET_KEY_DATA);
    prepareTemperaturesJsonObject(dataObj, temperatures);
    send(root);
}

void Socket::webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length)
{
    DeserializationError err;
    const int capacity = JSON_OBJECT_SIZE(4 + STATE_NUM_OF_FIELDS + TEMPERATURES_NUM_OF_FIELDS);
    StaticJsonDocument<capacity> doc;
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial1.printf("[%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED:
    {
        if (num != 0)
        {
            send(SOCKET_EVENT_RESPONSE_CONNECTION_BUSY, num);
            webSocket.disconnect(num);
            return;
        }
        IPAddress ip = webSocket.remoteIP(num);
        Serial1.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        serialNetwork->requestInitData();
        break;
    }
    case WStype_TEXT:
        Serial1.printf("[%u] get Text: %s\n", num, payload);
        err = deserializeJson(doc, payload);
        if (!err)
        {
            const char* event = doc[SOCKET_KEY_EVENT];
            if (strcmp(event, SOCKET_EVENT_REQUEST_HR_MODE_AUTO) == 0)
                serialNetwork->requestHrModeAuto();
            else if (strcmp(event, SOCKET_EVENT_REQUEST_HR_MODE_MANUAL) == 0)
                serialNetwork->requestHrModeManual();
            else if (strcmp(event, SOCKET_EVENT_REQUEST_ENABLE_HR) == 0)
                serialNetwork->requestEnableHr();
            else if (strcmp(event, SOCKET_EVENT_REQUEST_DISABLE_HR) == 0)
                serialNetwork->requestDisableHr();
            else if (strcmp(event, SOCKET_EVENT_REQUEST_APPLY_STATE_TEMPERATURES) == 0) 
            {
                JsonObject stateJson = doc[SOCKET_KEY_DATA];
                State* state = new State;
                state->intEvMin = stateJson["intEvMin"];
                state->extAdMin = stateJson["extAdMin"];
                state->extAdMax = stateJson["extAdMax"];
                state->hysteresis = stateJson["hysteresis"];
                serialNetwork->requestApplyStateTemperatures(state);
                delete[] state;
            }
        }
        break;
    case WStype_BIN:
        Serial1.printf("[%u] get binary length: %u\n", num, length);
        break;
    }
}