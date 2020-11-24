#include "SerialNetwork.h"

SerialNetwork::SerialNetwork() : BaseSerialNetwork(&dataLineSerial, &debugLineSerial, &debugLineSerial)
{
}

void SerialNetwork::processPacket()
{
    switch (transmissionPacket.code)
    {
    case LOCAL_CONTRACT_CODE_RESPONSE_STATE:
        receiveState(transmissionPacket.bodyPtr);
        break;
    
    default:
        break;
    }
}

void SerialNetwork::receiveState(const byte* body)
{
    debugLineSerial.println("ESP: Received state");
    size_t sizeOfState = sizeof(State);
    State* statePtr = new State;
    memcpy(statePtr, body, sizeOfState);
    Socket::sendState(statePtr);
    delete statePtr;
}

void SerialNetwork::requestState()
{
    TransmissionPacket packet = {LOCAL_CONTRACT_CODE_REQUEST_STATE, 0, NULL};
    sendPacket(packet);
}