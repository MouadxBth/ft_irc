#include "NickCommand.hpp"
#include "Server.hpp"
#include "Utilities.hpp"

//ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
//           ERR_NICKNAMEINUSE               ERR_NICKCOLLISION
//           ERR_UNAVAILRESOURCE             ERR_RESTRICTED

NickCommand::NickCommand() : Command("NICK", false, false)
{}

NickCommand::~NickCommand()
{}

NickCommand::NickCommand(const NickCommand& instance) : Command(instance)
{}

NickCommand& NickCommand::operator=(const NickCommand& instance)
{
    if (this != &instance)
    {
        Command::operator=(instance);
    }
    return *this;
}

void NickCommand::executeCommand(User *user, Data &data)
{
    std::string nickname = user->getNickname().empty() ? "*" : user->getNickname();

    if (!user->hasUsedPassword())
        return ;

    if (data.arguments.empty())
    {
        user->sendMessage(ERR_NONICKNAMEGIVEN(nickname));
        return ;
    }
    
    // invalid nickname
    if (!isValidIRCNickname(data.arguments[0]))
    {
        user->sendMessage(ERR_ERRONEUSNICKNAME(nickname, data.arguments[0]));
        return ;
    }

    const User *target = Server::getInstance()->getUser(data.arguments[0]);
    
    // nickname already taken
    if (target && target->getSocket().fd != user->getSocket().fd)
    {
        user->sendMessage(ERR_NICKNAMEINUSE(nickname, data.arguments[0]));
        return ;
    }

    // changing to reserved nickname
    if (containsString(Server::getInstance()->getReservedNicknames(), data.arguments[0]))
    {
        user->sendMessage(ERR_UNAVAILRESOURCE(nickname, data.arguments[0]));
        return ;
    }

    // restricted nickname
    if (containsString(Server::getInstance()->getRestrictedNicknames(), data.arguments[0]))
    {
        user->sendMessage(ERR_RESTRICTED(nickname));
        return ;
    }

    if (data.arguments[0].size() > 9)
        return ;
    
    user->setNickname(data.arguments[0]);
}