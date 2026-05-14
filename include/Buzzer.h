#ifndef BUZZER_H
#define BUZZER_H

class Buzzer
{
private:
    int pin;
public:
    Buzzer(int gpioPin);

    void begin();
    void off();
    void on();
    void beep(int time);
};

#endif