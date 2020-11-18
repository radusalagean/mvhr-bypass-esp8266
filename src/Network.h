#ifndef NETWORK_H
#define NETWORK_H

#include "Credentials.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

class Network
{
private:
    const char* ssid = WIFI_SSID;
    const char* password = WIFI_PASS;
public:
    void init();
    IPAddress getIpAddress();
};

#endif