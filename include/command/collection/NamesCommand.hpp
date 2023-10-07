
#pragma once

# include "Command.hpp"

class NamesCommand : public Command
{

public:
    NamesCommand();
    ~NamesCommand();
    NamesCommand(const NamesCommand& instance);
    NamesCommand& operator=(const NamesCommand& instance);

    void executeCommand(User *user, Data &data);
};
