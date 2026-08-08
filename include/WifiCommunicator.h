#ifndef WIFI_COMMUNICATOR_H
#define WIFI_COMMUNICATOR_H
#include <WiFi.h>

class WifiCommunicator
{
private:
    const char* ssid;
    const char* password;

    bool waitForConnection(int maxTries);

public:
    WifiCommunicator(const char* id, const char* parola);

    bool connect();
};


#endif
