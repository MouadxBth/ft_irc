/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/16 16:59:48 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PartCommand.hpp"

//ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
//           ERR_NOTONCHANNEL

PartCommand::PartCommand()
    : Command("PART", "let's you pass n sht", -1, true, false)
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
    if (!getServer())
        return ;

    if (data.arguments.size() > 2 || (data.arguments.size() == 2 && data.trailPresent))
    {
        user->sendMessage(ERR_UNKNOWN_COMMAND(data.command, data.command));
        return ;
    }

    std::vector<std::string> channels = split(data.arguments[0], ',');
    
    std::string message;
    
    for (std::vector<std::string>::const_iterator it = channels.begin(); it != channels.end(); it++)
    {
        Channel *target = getServer()->getChannel(*it);

        if (!target)
        {
            user->sendMessage(ERR_NO_SUCH_CHANNEL(user->getNickname(), *it));
            continue ;
        }
        if (!target->containsUser(user->getNickname()))
        {
            user->sendMessage(ERR_NOT_ON_CHANNEL(user->getNickname(), *it));
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

        user->sendMessage(message);

        if (target->getUsers().size())
            target->announce(message);
    }

    getServer()->cleanChannels();
}