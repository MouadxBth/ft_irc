
#pragma once

# include "Command.hpp"

class NoticeCommand : public Command
{

public:
    NoticeCommand();
    ~NoticeCommand();
    NoticeCommand(const NoticeCommand& instance);
    NoticeCommand& operator=(const NoticeCommand& instance);

    void executeCommand(User *user, Data &data);
};
