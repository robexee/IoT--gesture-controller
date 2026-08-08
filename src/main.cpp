#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include "Led.h"
#include "Buzzer.h"
#include "WifiCommunicator.h"
#include "ServerCommunicator.h"
#include "CommandHandler.h"

#define COMMAND_MAX_LEN 32

Led redLed(23);
Led yellowLed(22);
Buzzer buzzer(2);

WifiCommunicator wifi(
    "Orange-54A4",
    "tTQExQ4uSbS4b6ubfA"
);

ServerCommunicator server(
    "192.168.1.37", // IP-ul/hostname-ul unde ruleaza serverul Node.js
    8080
);

CommandHandler commandHandler(
    &redLed,
    &yellowLed,
    &buzzer
);

QueueHandle_t commandQueue;

void wifiTask(void* pvParameters) {
    bool serverStarted = false;

    for (;;) {
        if (wifi.connect() && !serverStarted)
        {
            server.begin();
            serverStarted = true;
        }

        if (serverStarted)
        {
            server.loop();

            const char* command = server.getCommand();
            if (strlen(command) > 0)
            {
                char buf[COMMAND_MAX_LEN];
                strncpy(buf, command, COMMAND_MAX_LEN - 1);
                buf[COMMAND_MAX_LEN - 1] = '\0';
                xQueueSend(commandQueue, buf, portMAX_DELAY);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void commandTask(void* pvParameters) {
    char buf[COMMAND_MAX_LEN];
    for (;;) {
        if (xQueueReceive(commandQueue, buf, portMAX_DELAY))
        {
            commandHandler.handleCommand(buf);
        }
    }
}

void setup() {
    Serial.begin(115200);
    buzzer.begin();
    buzzer.setActive(false);
    redLed.begin();
    yellowLed.begin();

    commandQueue = xQueueCreate(10, COMMAND_MAX_LEN);

    xTaskCreatePinnedToCore(wifiTask, "wifiTask", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(commandTask, "commandTask", 4096, NULL, 1, NULL, 1);
}

void loop() {
}
