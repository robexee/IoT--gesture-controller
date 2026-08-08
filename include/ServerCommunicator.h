#ifndef SERVER_COMMUNICATOR_H
#define SERVER_COMMUNICATOR_H

#include <Arduino.h>
#include <WebSocketsClient.h>

#define SERVER_COMMAND_MAX_LEN 32

class ServerCommunicator
{
private:
    const char* host;
    uint16_t port;
    const char* path;

    WebSocketsClient webSocket;
    char lastCommand[SERVER_COMMAND_MAX_LEN];
    volatile bool hasCommand;

    void handleEvent(WStype_t type, uint8_t* payload, size_t length);

public:
    ServerCommunicator(const char* serverHost, uint16_t serverPort, const char* wsPath = "/?role=esp32");

    void begin();
    void loop();
    const char* getCommand();
};

#endif
