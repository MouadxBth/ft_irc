/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/15 14:37:54 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TopicCommand.hpp"

//ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
//           RPL_NOTOPIC                     RPL_TOPIC
//           ERR_CHANOPRIVSNEEDED            ERR_NOCHANMODES

TopicCommand::TopicCommand()
    : Command("TOPIC", "let's you pass n sht", -1, true, false)
{}

TopicCommand::~TopicCommand()
{}

TopicCommand::TopicCommand(const TopicCommand& instance) : Command(instance)
{}

TopicCommand& TopicCommand::operator=(const TopicCommand& instance)
{
    if (this != &instance)
    {
        Command::operator=(instance);
    }
    return *this;
}


void TopicCommand::executeCommand(User *user, Data &data)
{   
    if (!getServer())
        return ;
        
    if (data.arguments.empty())
    {
        user->sendMessage(ERR_NEED_MORE_PARAMS(user->getNickname(), data.command));
        return ;
    }

    if (data.arguments.size() > 2 || (data.arguments.size() == 2 && data.trailPresent))
    {
        user->sendMessage(ERR_UNKNOWN_COMMAND(data.command, data.command));
        return ;
    }

    Channel *target = getServer()->getChannel(data.arguments[0]);
    
    if (!target)
        return ;
    
    if (!target->containsUser(user->getNickname()))
    {
        user->sendMessage(ERR_NOT_ON_CHANNEL(user->getNickname(), data.arguments[0]));
        return ;
    }

    if (data.arguments.size() == 1 && !data.trailPresent)
    {
        if (target->getTopic().empty())
            user->sendMessage(RPL_NOTOPIC(data.arguments[0]));
        else
            user->sendMessage(RPL_TOPIC(data.arguments[0], target->getTopic()));
        return ;
    }

    if (!target->getUser(user->getNickname()).second.channelOperator)
    {
        user->sendMessage(ERR_CHAN_O_PRIVS_NEEDED(user->getNickname(), data.arguments[0]));
        return ;
    }

    std::string topic;

    std::string message = ":" + user->getNickname() + "!" 
        + user->getUsername() + "@" 
        + user->getHostname() + " " 
        + data.command + " ";

    if (data.arguments.size() == 2)
    {
        topic = data.arguments[1];
        message += data.arguments[1];
    }
    else
    {
        topic = data.trail;
        message += ":" + data.trail;
    }
    
    target->broadcast(user->getNickname(), message);
}