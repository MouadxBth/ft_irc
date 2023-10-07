
#pragma once

# include "Command.hpp"

class KickCommand : public Command
{

public:
    KickCommand();
    ~KickCommand();
    KickCommand(const KickCommand& instance);
    KickCommand& operator=(const KickCommand& instance);

    void executeCommand(User *user, Data &data);
};
