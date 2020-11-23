#include "Network.h"

void Network::init()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial1.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }
}

IPAddress Network::getIpAddress()
{
    return WiFi.localIP();
}