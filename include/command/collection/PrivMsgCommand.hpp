
#pragma once

# include "Command.hpp"

class PrivMsg : public Command
{

public:
    PrivMsg();
    ~PrivMsg();
    PrivMsg(const PrivMsg& instance);
    PrivMsg& operator=(const PrivMsg& instance);

    void executeCommand(User *user, Data &data);
};
