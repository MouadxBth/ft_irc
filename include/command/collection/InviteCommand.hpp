
#pragma once

# include "Command.hpp"

class InviteCommand : public Command
{

public:
    InviteCommand();
    ~InviteCommand();
    InviteCommand(const InviteCommand& instance);
    InviteCommand& operator=(const InviteCommand& instance);

    void executeCommand(User *user, Data &data);
};
