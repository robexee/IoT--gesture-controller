#include "WifiCommunicator.h"

WifiCommunicator::WifiCommunicator(const char* id, const char* parola, unsigned int port)
{
    ssid = id;
    password = parola;
    localUdpPort = port;
}

bool WifiCommunicator::connect()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return true;
    }

    Serial.println("Connecting to WiFi: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    if (!waitForConnection(20))
    {
        Serial.println("\nError: Couldn't connect to WiFi");
        return false;
    }

    Serial.println("\nWiFi connected");
    Serial.print("Ip addres is: ");
    Serial.println(WiFi.localIP());

    beginUdpListener();
    return true;
}

const char* WifiCommunicator::getCommand()
{
    int packSize = udp.parsePacket();

    if (packSize)
    {
        int len = udp.read(incomingPacket, 255);
        if (len > 0 && len < 255)
        {
            incomingPacket[len] = '\0';
        }
        return incomingPacket;
    }
    return "";
}

bool WifiCommunicator::waitForConnection(int maxTries)
{
    int tries = 0;
    while (WiFi.status() != WL_CONNECTED && tries < maxTries)
    {
        delay(500);
        Serial.print('.');
        tries++;
    }
    return WiFi.status() == WL_CONNECTED;
}

void WifiCommunicator::beginUdpListener()
{
    udp.begin(localUdpPort);
}
