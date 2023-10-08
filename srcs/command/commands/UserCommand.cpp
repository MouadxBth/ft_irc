#include "UserCommand.hpp"
#include "Server.hpp"
#include "CommandManager.hpp"
#include "Utilities.hpp"

//ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
//USER guest 0 * :Ronnie Reagan 
//USER guest 8 * :Ronnie Reagan <-- invis

UserCommand::UserCommand()
    : Command("USER", false, false)
{}

UserCommand::~UserCommand()
{}

UserCommand::UserCommand(const UserCommand& instance) : Command(instance)
{}

UserCommand& UserCommand::operator=(const UserCommand& instance)
{
    if (this != &instance)
    {
        Command::operator=(instance);
    }
    return *this;
}

void UserCommand::executeCommand(User *user, Data &data)
{
    // user hasn't used nick command yet
    if (user->getNickname().empty())
        return ;
    
    if (user->isAuthenticated())
    {
        user->sendMessage(ERR_ALREADYREGISTERED(user->getNickname()));
        return ;
    }

    if (data.arguments.empty() || data.arguments.size() < 3)
    {
        user->sendMessage(ERR_NEEDMOREPARAMS(user->getNickname(), data.command));
        return ;
    }

    if (!isValidIRCUsername(data.arguments[0]))
    {
        return ;
    }
    
    user->setUsername(data.arguments[0]);
    
    if (data.arguments.size() == 3)
        user->setFullname(data.trail);
    else
        user->setFullname(data.arguments[3]);
    
    // set user mode

    user->setAuthenticated(true);
    Server::getInstance()->authenticateUser(user);

    if (!user->sendMessage(RPL_WELCOME(user->getNickname(),
        user->getUsername(),
        user->getHostname())))
        return ;
    
    if (!user->sendMessage(RPL_YOURHOST(user->getNickname(),
        Server::getInstance()->getName(),
        Server::getInstance()->getVersion())))
        return ;

    if (!user->sendMessage(RPL_CREATED(user->getNickname(),
        Server::getInstance()->getCreationDate())))
        return ;

    if (!user->sendMessage(RPL_MYINFO(user->getNickname(),
        Server::getInstance()->getName(),
        Server::getInstance()->getVersion(),
        Server::getInstance()->getUserModes(),
        Server::getInstance()->getChannelModes())))
        return ;

    Data motdData = emptyCommandData();

    motdData.command = "MOTD";

    CommandManager::getInstance()->executeCommand(user, motdData);

    std::cout << "=> AUTHENTICATED: " << user->getNickname() << std::endl;
}