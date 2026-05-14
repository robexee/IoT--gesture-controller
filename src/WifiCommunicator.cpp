#include "WifiCommunicator.h"

WifiCommunicator::WifiCommunicator(const char* id, const char* parola, unsigned int port)
{
    ssid = id;
    password = parola;
    localUdpPort = port;
}

bool WifiCommunicator::connect()
{
    Serial.println("Connecting to WiFi: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    int tries = 0;
    while (WiFi.status() != WL_CONNECTED && tries < 20)
    {
        delay(500);
        Serial.print('.');
        tries++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("\nWiFi connected");
        Serial.print("Ip addres is: ");
        Serial.println(WiFi.localIP());

        udp.begin(localUdpPort);
        return true;
    } else {
        Serial.println("\nError: Couldn't connect to WiFi");
        return false;
    }
}

void WifiCommunicator::reconnect()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Connection lost! Trying to reconnect...");
        WiFi.disconnect();
        connect();
    }
}

const char* WifiCommunicator::getCommand()
{
    int packSize = udp.parsePacket();

    if (packSize) 
    {
        int len = udp.read(incomingPacket, 255);
        if (len > 0)
        {
            incomingPacket[len] = '\0';
        }
        return incomingPacket;
    }
    return "";
}