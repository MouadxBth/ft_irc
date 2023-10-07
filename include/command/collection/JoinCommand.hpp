
#pragma once

# include "Command.hpp"

class JoinCommand : public Command
{

public:
    JoinCommand();
    ~JoinCommand();
    JoinCommand(const JoinCommand& instance);
    JoinCommand& operator=(const JoinCommand& instance);

    void executeCommand(User *user, Data &data);
};
