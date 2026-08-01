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
    if(strcmp(command, "/buzz") == 0)
    {
        buzzer ->beep(300);
    }

    else if(strcmp(command,"/red_on") == 0)
    {
        redLed ->on();
    }

    else if(strcmp(command,"/red_off") == 0)
    {
        redLed -> off();
    }

    else if(strcmp(command,"/yellow_on") == 0)
    {
        yellowLed -> on();
    }

    else if(strcmp(command,"/yellow_off") == 0)
    {
        yellowLed -> off();
    }
}