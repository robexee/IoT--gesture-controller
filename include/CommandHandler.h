#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "Led.h"
#include "Buzzer.h"

class CommandHandler
{
private:
    Led* redLed;
    Led* yellowLed;

    Buzzer* buzzer;

public:
    CommandHandler(
        Led* red,
        Led* yellow,
        Buzzer* buzz
    );


    void handleCommand(const char* command);
};


#endif

