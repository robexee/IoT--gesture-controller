#include "Buzzer.h"
#include <Arduino.h>

Buzzer::Buzzer(int gpioPin)
{
   pin = gpioPin;
}

void Buzzer::begin()
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Buzzer::setActive(bool active)
{
    digitalWrite(pin, active ? HIGH : LOW);
}
