#include "SerialNetwork.h"

SerialNetwork::SerialNetwork() : BaseSerialNetwork(&dataLineSerial, &debugLineSerial, &debugLineSerial)
{
}

// RECEIVE

void SerialNetwork::processPacket()
{
    switch (transmissionPacket.code)
    {
    case LOCAL_CONTRACT_CODE_RESPONSE_INIT_DATA:
        receive<InitData>(transmissionPacket.bodyPtr);
        break;
    case LOCAL_CONTRACT_CODE_RESPONSE_STATE:
        receive<State>(transmissionPacket.bodyPtr);
        break;
    case LOCAL_CONTRACT_CODE_RESPONSE_TEMPERATURES:
        receive<Temperatures>(transmissionPacket.bodyPtr);
        break;

    default:
        break;
    }
}

template<typename T>
void SerialNetwork::receive(const byte* body)
{
    size_t size = sizeof(T);
    T* ptr = new T;
    memcpy(ptr, body, size);
    Socket::send(ptr);
    delete ptr;
}

// REQUEST

void SerialNetwork::requestInitData()
{
    TransmissionPacket packet = {LOCAL_CONTRACT_CODE_REQUEST_INIT_DATA};
    sendPacket(packet);
}

void SerialNetwork::requestState()
{
    TransmissionPacket packet = {LOCAL_CONTRACT_CODE_REQUEST_STATE};
    sendPacket(packet);
}

void SerialNetwork::requestTemperatures()
{
    TransmissionPacket packet = {LOCAL_CONTRACT_CODE_REQUEST_TEMPERATURES};
    sendPacket(packet);
}