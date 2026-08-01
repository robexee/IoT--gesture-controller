#ifndef BUZZER_H
#define BUZZER_H

class Buzzer
{
private:
    int pin;
public:
    Buzzer(int gpioPin);

    void begin();
    void setActive(bool active);
};

#endif