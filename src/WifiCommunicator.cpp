#include "WifiCommunicator.h"

WifiCommunicator::WifiCommunicator(const char* id, const char* parola)
{
    ssid = id;
    password = parola;
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

    return true;
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
