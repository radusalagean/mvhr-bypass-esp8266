#ifndef OTA_H
#define OTA_H

#include "Credentials.h"
#include <ArduinoOTA.h>

#define OTA_PORT 25483
#define OTA_HOSTNAME "mvhr-bypass"

class Ota
{
private:

public:
    void init();
    void handle();
};

#endif

