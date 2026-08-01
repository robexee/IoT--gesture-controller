#include <Arduino.h>
#include "Led.h"
#include "Buzzer.h"
#include "WifiCommunicator.h"
#include "CommandHandler.h"

Led redLed(23);
Led yellowLed(22);
Buzzer buzzer(2);

WifiCommunicator wifi(
    "Orange-54A4",
    "tTQExQ4uSbS4b6ubfA",
    4210
);

CommandHandler commandHandler(
    &redLed,
    &yellowLed,
    &buzzer
);
void setup() {
    Serial.begin(115200);
    buzzer.begin();
    buzzer.setActive(false);
    redLed.begin();
    yellowLed.begin();
    wifi.connect();
}

void loop() {
    wifi.connect();

    const char* command = wifi.getCommand();
    if(strlen(command) > 0)
    {
        commandHandler.handleCommand(command);
    }
}