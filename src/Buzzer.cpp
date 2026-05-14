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

void Buzzer::on()
{
    digitalWrite(pin, HIGH);
}

void Buzzer::off()
{
    digitalWrite(pin, LOW);
}

void Buzzer::beep(int time)
{
    on();
    delay(time);
    off();
}