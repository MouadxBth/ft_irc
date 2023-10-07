
#pragma once

# include "Command.hpp"

class QuitCommand : public Command
{

public:
    QuitCommand();
    ~QuitCommand();
    QuitCommand(const QuitCommand& instance);
    QuitCommand& operator=(const QuitCommand& instance);

    void executeCommand(User *user, Data &data);
};
