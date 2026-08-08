#include "ServerCommunicator.h"
#include <cstring>

ServerCommunicator::ServerCommunicator(const char* serverHost, uint16_t serverPort, const char* wsPath)
{
    host = serverHost;
    port = serverPort;
    path = wsPath;
    hasCommand = false;
    lastCommand[0] = '\0';
}

void ServerCommunicator::begin()
{
    webSocket.onEvent([this](WStype_t type, uint8_t* payload, size_t length) {
        handleEvent(type, payload, length);
    });
    webSocket.begin(host, port, path);
    webSocket.setReconnectInterval(5000);
}

void ServerCommunicator::loop()
{
    webSocket.loop();
}

void ServerCommunicator::handleEvent(WStype_t type, uint8_t* payload, size_t length)
{
    if (type == WStype_TEXT)
    {
        size_t len = length < SERVER_COMMAND_MAX_LEN - 1 ? length : SERVER_COMMAND_MAX_LEN - 1;
        memcpy(lastCommand, payload, len);
        lastCommand[len] = '\0';
        hasCommand = true;
    }
    else if (type == WStype_CONNECTED)
    {
        Serial.println("WebSocket connected to server");
    }
    else if (type == WStype_DISCONNECTED)
    {
        Serial.println("WebSocket disconnected from server");
    }
}

const char* ServerCommunicator::getCommand()
{
    if (hasCommand)
    {
        hasCommand = false;
        return lastCommand;
    }
    return "";
}
