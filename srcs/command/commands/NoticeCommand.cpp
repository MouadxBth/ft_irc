/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NoticeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/19 16:39:52 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NoticeCommand.hpp"
#include "Server.hpp"

NoticeCommand::NoticeCommand() : Command("NOTICE", true, false)
{}

NoticeCommand::~NoticeCommand()
{}

NoticeCommand::NoticeCommand(const NoticeCommand& instance) : Command(instance)
{}

NoticeCommand& NoticeCommand::operator=(const NoticeCommand& instance)
{
    if (this != &instance)
    {
        Command::operator=(instance);
    }
    return *this;
}

void NoticeCommand::executeCommand(User *user, Data &data)
{
    if (!data.arguments.size() || (data.arguments.size() < 2 && !data.trailPresent))
    {
        return ;
    }

    const User *userTarget = Server::getInstance()->getAuthenticatedUser(data.arguments[0]);

    std::string message = ":" + user->getNickname() + "!" 
            + user->getUsername() + "@" 
            + user->getHostname() + " " 
            + data.command + " " + data.arguments[0] + " :";

    message += data.arguments.size() == 1 ? data.trail : data.arguments[1];
    
    if (userTarget)
    {
        userTarget->sendMessage(message);
        return ;
    }

    const Channel *channelTarget = Server::getInstance()->getChannel(data.arguments[0]);

    if (!channelTarget)
    {
        return ;
    }
    
    channelTarget->broadcast(user->getNickname(), message);
}