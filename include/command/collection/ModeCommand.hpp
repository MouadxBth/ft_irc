
#pragma once

# include "Command.hpp"

class ModeCommand : public Command
{

public:
    ModeCommand();
    ~ModeCommand();
    ModeCommand(const ModeCommand& instance);
    ModeCommand& operator=(const ModeCommand& instance);

    void executeCommand(User *user, Data &data);
};
