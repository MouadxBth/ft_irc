
#pragma once

# include "Command.hpp"

class MotdCommand : public Command
{

public:
    MotdCommand();
    ~MotdCommand();
    MotdCommand(const MotdCommand& instance);
    MotdCommand& operator=(const MotdCommand& instance);

    void executeCommand(User *user, Data &data);
};
