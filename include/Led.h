#ifndef LED_H
#define LED_H


class Led
{
private:
    int pin;
    bool state;

public:
    Led(int gpioPin);

    void begin();
    void on();
    void off();
    void toggle();
    bool IsOn();

};

#endif