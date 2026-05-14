#include <Arduino.h>
#include "Led.h"
#include "Buzzer.h"

Led redLed(23);
Led yellowLed(22);
Buzzer buzzer(2);

void setup() {
    buzzer.begin();
    redLed.begin();
    yellowLed.begin();
    buzzer.beep(3000);
}

void loop() {
    redLed.on();
    yellowLed.on();
}