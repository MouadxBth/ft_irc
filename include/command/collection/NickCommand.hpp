
#pragma once

# include "Command.hpp"

class NickCommand : public Command
{

public:
    NickCommand();
    ~NickCommand();
    NickCommand(const NickCommand& instance);
    NickCommand& operator=(const NickCommand& instance);

    void executeCommand(User *user, Data &data);
};
