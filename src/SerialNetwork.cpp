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
        processPacketBodyObject<InitData>();
        break;
    case LOCAL_CONTRACT_CODE_RESPONSE_STATE:
        processPacketBodyObject<State>();
        break;
    case LOCAL_CONTRACT_CODE_RESPONSE_TEMPERATURES:
        processPacketBodyObject<Temperatures>();
        break;
    default:
        break;
    }
}

template<typename T>
void SerialNetwork::processPacketBodyObject()
{
    T* t = receivePacketBodyObject<T>();
    Socket::send(t);
    delete t;
}

// REQUEST

void SerialNetwork::requestInitData()
{
    send(LOCAL_CONTRACT_CODE_REQUEST_INIT_DATA);
}

void SerialNetwork::requestHrModeAuto()
{
    send(LOCAL_CONTRACT_CODE_REQUEST_HR_MODE_AUTO);
}

void SerialNetwork::requestHrModeManual()
{
    send(LOCAL_CONTRACT_CODE_REQUEST_HR_MODE_MANUAL);
}

void SerialNetwork::requestEnableHr()
{
    send(LOCAL_CONTRACT_CODE_REQUEST_ENABLE_HR);
}

void SerialNetwork::requestDisableHr()
{
    send(LOCAL_CONTRACT_CODE_REQUEST_DISABLE_HR);
}

void SerialNetwork::requestApplyStateTemperatures(State* state)
{
    send(LOCAL_CONTRACT_CODE_REQUEST_APPLY_STATE_TEMPERATURES, state);
}