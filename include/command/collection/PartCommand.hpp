
#pragma once

# include "Command.hpp"

class PartCommand : public Command
{

public:
    PartCommand();
    ~PartCommand();
    PartCommand(const PartCommand& instance);
    PartCommand& operator=(const PartCommand& instance);

    void executeCommand(User *user, Data &data);
};
