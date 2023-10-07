
#pragma once

# include "Command.hpp"

class TopicCommand : public Command
{

public:
    TopicCommand();
    ~TopicCommand();
    TopicCommand(const TopicCommand& instance);
    TopicCommand& operator=(const TopicCommand& instance);

    void executeCommand(User *user, Data &data);
};
