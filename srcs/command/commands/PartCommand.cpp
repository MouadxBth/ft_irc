#include "PartCommand.hpp"
#include "Server.hpp"
#include "Utilities.hpp"

//ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
//           ERR_NOTONCHANNEL

PartCommand::PartCommand() : Command("PART", true, false)
{}

PartCommand::~PartCommand()
{}

PartCommand::PartCommand(const PartCommand& instance) : Command(instance)
{}

PartCommand& PartCommand::operator=(const PartCommand& instance)
{
    if (this != &instance)
    {
        Command::operator=(instance);
    }
    return *this;
}


void PartCommand::executeCommand(User *user, Data &data)
{   
    if (data.arguments.empty())
    {
        user->sendMessage(ERR_NEEDMOREPARAMS(user->getNickname(), data.command));
        return ;
    }

    std::vector<std::string> channels = split(data.arguments[0], ',');
    
    std::string message;
    
    for (std::vector<std::string>::const_iterator it = channels.begin(); it != channels.end(); it++)
    {
        Channel *target = Server::getInstance()->getChannel(*it);

        if (!target)
        {
            if (!user->sendMessage(ERR_NOSUCHCHANNEL(user->getNickname(), *it)))
                return ;
            continue ;
        }
        
        if (!target->containsUser(user->getNickname()))
        {
            if (!user->sendMessage(ERR_NOTONCHANNEL(user->getNickname(), *it)))
                return ;
            continue ;
        }

        message = ":" + user->getNickname() + "!" 
                + user->getUsername() + "@" 
                + user->getHostname() + " " 
                + data.command + " " + *it + " :";
            
        if (data.trailPresent)
            message += data.trail;
        else if (data.arguments.size() == 2)
            message += data.arguments[1];
        else
            message += user->getNickname();

        target->removeUser(user->getNickname());
        
        if (!target->getUsers().size())
        {
            Server::getInstance()->removeChannel(target->getName());
        }
        
        user->setJoinedChannelsCount(user->getJoinedChannelsCount() - 1);

        user->sendMessage(message);

        if (target->getUsers().size())
            target->announce(message);
    }

    Server::getInstance()->cleanChannels();
}