
#pragma once

# include "Command.hpp"

class ListCommand : public Command
{

public:
    ListCommand();
    ~ListCommand();
    ListCommand(const ListCommand& instance);
    ListCommand& operator=(const ListCommand& instance);

    void executeCommand(User *user, Data &data);
};
