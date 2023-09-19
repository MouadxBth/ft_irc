/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/19 16:34:21 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TopicCommand.hpp"
#include "Server.hpp"

//ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
//           RPL_NOTOPIC                     RPL_TOPIC
//           ERR_CHANOPRIVSNEEDED            ERR_NOCHANMODES

TopicCommand::TopicCommand() : Command("TOPIC", true, false)
{}

TopicCommand::~TopicCommand()
{}

TopicCommand::TopicCommand(const TopicCommand& instance) : Command(instance)
{}

TopicCommand& TopicCommand::operator=(const TopicCommand& instance)
{
    if (this != &instance)
        Command::operator=(instance);
    return *this;
}


void TopicCommand::executeCommand(User *user, Data &data)
{   
    if (data.arguments.empty())
    {
        user->sendMessage(ERR_NEEDMOREPARAMS(user->getNickname(), data.command));
        return ;
    }

    Channel *target = Server::getInstance()->getChannel(data.arguments[0]);
    
    if (!target)
        return ;
    
    if (!target->containsUser(user->getNickname()))
    {
        user->sendMessage(ERR_NOTONCHANNEL(user->getNickname(), data.arguments[0]));
        return ;
    }

    if (data.arguments.size() == 1 && !data.trailPresent)
    {
        if (target->getTopic().empty())
            user->sendMessage(RPL_NOTOPIC(user->getNickname(), data.arguments[0]));
        else
            user->sendMessage(RPL_TOPIC(user->getNickname(),
                data.arguments[0],
                target->getTopic()));
        return ;
    }

    if (target->isTopicSettableByChannelOperatorOnly()
        && !target->getUser(user->getNickname()).second.channelOperator)
    {
        user->sendMessage(ERR_CHANOPRIVSNEEDED(user->getNickname(), data.arguments[0]));
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

    target->setTopic(topic);
    
    target->announce(message);
}