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
    debugLineSerial.print(statePtr->hrModeAuto);
    debugLineSerial.print(" ");
    debugLineSerial.print(statePtr->hrDisabled);
    debugLineSerial.print(" ");
    debugLineSerial.print(statePtr->intEvMin);
    debugLineSerial.print(" ");
    debugLineSerial.print(statePtr->extAdMin);
    debugLineSerial.print(" ");
    debugLineSerial.print(statePtr->extAdMax);
    debugLineSerial.print(" ");
    debugLineSerial.println(statePtr->hysteresis);
    delete statePtr;
}