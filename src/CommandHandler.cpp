#include "CommandHandler.h"

CommandHandler::CommandHandler(
    Led* red,
    Led* yellow,
    Buzzer* buzz
){
    red = redLed;
    yellow = yellowLed;
    buzzer = buzz;
}

void CommandHandler::handleCommand(const char* command)
{
    if(command == "/buzz")
    {
        buzzer ->beep(300);
    }

    else if(command == "/red_on")
    {
        redLed ->on();
    }

    else if(command == "/red_off")
    {
        redLed -> off();
    }

    else if(command == "/yellow_on")
    {
        yellowLed -> on();
    }

    else if(command == "/yellow_off")
    {
        yellowLed -> off();
    }
}