#include "CommandHandler.h"
#include <cstring>

CommandHandler::CommandHandler(
    Led* red,
    Led* yellow,
    Buzzer* buzz
){
    redLed = red;
    yellowLed = yellow;
    buzzer = buzz;
}

void CommandHandler::handleCommand(const char* command)
{
    if(strcmp(command, "/buzz_on") == 0)
    {
        handleBuzz(true);
    }

    else if(strcmp(command, "/buzz_off") == 0)
    {
        handleBuzz(false);
    }

    else if(strcmp(command, "/red_on") == 0)
    {
        handleRed(true);
    }

    else if(strcmp(command, "/red_off") == 0)
    {
        handleRed(false);
    }

    else if(strcmp(command, "/yellow_on") == 0)
    {
        handleYellow(true);
    }

    else if(strcmp(command, "/yellow_off") == 0)
    {
        handleYellow(false);
    }
}

void CommandHandler::handleRed(bool on)
{
    if (on) redLed->on(); else redLed->off();
}

void CommandHandler::handleYellow(bool on)
{
    if (on) yellowLed->on(); else yellowLed->off();
}

void CommandHandler::handleBuzz(bool on)
{
    buzzer->setActive(on);
}
