#ifndef WIFI_COMMUNICATOR_H
#define WIFI_COMMUNICATOR_H
#include <WiFi.h>
#include <WiFiUdp.h>

class WifiCommunicator
{
private:
    const char* ssid;
    const char* password;
    unsigned int localUdpPort;
    WiFiUDP udp;
    char incomingPacket[255];

public:
    WifiCommunicator(const char* id, const char* parola, unsigned int port);

    bool connect();
    void reconnect();
    const char* getCommand();
};


#endif