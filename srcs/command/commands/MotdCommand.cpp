/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MotdCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/21 18:37:28 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

    if (!user->sendMessage(RPL_MOTDSTART(user->getNickname(),
        Server::getInstance()->getName())))
        return ;
    
    for (std::vector<std::string>::const_iterator it = Server::getInstance()->getMotd().begin();
        it != Server::getInstance()->getMotd().end();
        it++)
    {
        if (!user->sendMessage(RPL_MOTD(user->getNickname(), *it)))
            return ;
    }

    user->sendMessage(RPL_ENDOFMOTD(user->getNickname()));
}