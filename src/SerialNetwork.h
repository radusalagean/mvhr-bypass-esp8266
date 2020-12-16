#ifndef SERIAL_NETWORK_H
#define SERIAL_NETWORK_H

#include "lib/mvhr-bypass-common/arduino-esp8266/BaseSerialNetwork.h"
#include "HardwareSerialWrapper.h"

class SerialNetwork;

#include "Socket.h"

class SerialNetwork : public BaseSerialNetwork
{
private:
    HardwareSerialWrapper dataLineSerial = HardwareSerialWrapper(&Serial);
    HardwareSerialWrapper debugLineSerial = HardwareSerialWrapper(&Serial1);
    void processPacket();
    template<typename T>
    void processPacketBodyObject();

public:
    SerialNetwork();
    void requestInitData();
    void requestHrModeAuto();
    void requestHrModeManual();
    void requestEnableHr();
    void requestDisableHr();
    void requestApplyStateTemperatures(State* state);
};

#endif