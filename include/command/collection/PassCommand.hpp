
#pragma once

# include "Command.hpp"

class PassCommand : public Command
{

public:
    PassCommand();
    ~PassCommand();
    PassCommand(const PassCommand& instance);
    PassCommand& operator=(const PassCommand& instance);

    void executeCommand(User *user, Data &data);
};
