#include "Led.h"
#include <Arduino.h>

Led::Led(int gpioPin)
{
    pin = gpioPin;
    state = false;
}

void Led::begin()
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Led::on()
{
    digitalWrite(pin, HIGH);
    state = true;
}

void Led::off()
{
    digitalWrite(pin, LOW);
    state = false;
}

bool Led::IsOn()
{
    return state;
}

void Led::toggle()
{
    state = !state;
    digitalWrite(pin, state);
}