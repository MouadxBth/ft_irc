
#pragma once

# include "Command.hpp"

class UserCommand : public Command
{

public:
    UserCommand();
    ~UserCommand();
    UserCommand(const UserCommand& instance);
    UserCommand& operator=(const UserCommand& instance);

    void executeCommand(User *user, Data &data);
};
