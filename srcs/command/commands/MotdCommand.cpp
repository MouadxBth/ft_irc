/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MotdCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/19 04:01:00 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>

#include "MotdCommand.hpp"
#include "Server.hpp"

//RPL_MOTDSTART                   RPL_MOTD
//           RPL_ENDOFMOTD                   ERR_NOMOTD

MotdCommand::MotdCommand() : Command("MOTD", true, false)
{}

MotdCommand::~MotdCommand()
{}

MotdCommand::MotdCommand(const MotdCommand& instance) : Command(instance)
{}

MotdCommand& MotdCommand::operator=(const MotdCommand& instance)
{
    if (this != &instance)
    {
        Command::operator=(instance);
    }
    return *this;
}

void MotdCommand::executeCommand(User *user, Data &data)
{
    if (!data.arguments.empty() && data.arguments[0] != Server::getInstance()->getName())
    {
        return ;
    }

    if (Server::getInstance()->getMotd().empty())
    {
        user->sendMessage(ERR_NOMOTD(user->getNickname()));
        return ;
    }

    user->sendMessage(RPL_MOTDSTART(user->getNickname(),
        Server::getInstance()->getName()));
    
    for (std::vector<std::string>::const_iterator it = Server::getInstance()->getMotd().begin();
        it != Server::getInstance()->getMotd().end();
        it++)
    {
        user->sendMessage(RPL_MOTD(user->getNickname(), *it));
    }

    user->sendMessage(RPL_ENDOFMOTD(user->getNickname()));
}