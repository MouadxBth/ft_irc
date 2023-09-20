/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/20 21:36:40 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserCommand.hpp"
#include "Server.hpp"
#include "CommandManager.hpp"

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
    
    user->setUsername(data.arguments[0]);
    if (data.arguments.size() == 3)
        user->setFullname(data.trail);
    else
        user->setFullname(data.arguments[3]);
    // set user mode

    user->setAuthenticated(true);

    user->sendMessage(RPL_WELCOME(user->getNickname(),
        user->getUsername(),
        user->getHostname()));
    
    user->sendMessage(RPL_YOURHOST(user->getNickname(),
        Server::getInstance()->getName(),
        Server::getInstance()->getVersion()));

    user->sendMessage(RPL_CREATED(user->getNickname(),
        Server::getInstance()->getCreationDate()));

    user->sendMessage(RPL_MYINFO(user->getNickname(),
        Server::getInstance()->getName(),
        Server::getInstance()->getVersion(),
        Server::getInstance()->getUserModes(),
        Server::getInstance()->getChannelModes()));

    Data motdData = emptyCommandData();

    motdData.command = "MOTD";

    CommandManager::getInstance()->executeCommand(user, motdData);

    Server::getInstance()->getConnectedUsers().erase(user->getUserSocket().fd);
    Server::getInstance()->getAuthenticatedUsers()[user->getNickname()] = user;

    std::cout << "=> JOINED: " << user->getNickname() << std::endl;
    Server::getInstance()->setJoins(Server::getInstance()->getJoins() + 1);

    std::cout << Server::getInstance()->getJoins() << std::endl;

    //std::cout << "User: " << user->getNickname() << " has joined" << std::endl;

    //std::cout << *user << std::endl;
}