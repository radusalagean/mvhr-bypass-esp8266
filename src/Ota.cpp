#include "Ota.h"

void Ota::init()
{
    ArduinoOTA.setPort(OTA_PORT);

    ArduinoOTA.setHostname(OTA_HOSTNAME);

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else { // U_FS
            type = "filesystem";
        }

        Serial1.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial1.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial1.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial1.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
        Serial1.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
        Serial1.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
        Serial1.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
        Serial1.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
        Serial1.println("End Failed");
        }
    });
    ArduinoOTA.begin();
}

void Ota::handle()
{
    ArduinoOTA.handle();
}