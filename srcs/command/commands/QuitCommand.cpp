/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/24 13:11:52 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "QuitCommand.hpp"
#include "Server.hpp"
#include "unistd.h"

QuitCommand::QuitCommand() : Command("QUIT", false, false)
{}

QuitCommand::~QuitCommand()
{}

QuitCommand::QuitCommand(const QuitCommand& instance) : Command(instance)
{}

QuitCommand& QuitCommand::operator=(const QuitCommand& instance)
{
    if (this != &instance)
        Command::operator=(instance);
    return *this;
}

void QuitCommand::executeCommand(User *user, Data &data)
{   
    std::string reply = ":" + user->getNickname() + "!" 
                + user->getUsername() + "@" 
                + user->getHostname() + " " 
                + data.command + " :" + data.command + ": ";
    
    if (data.arguments.size())
        reply += data.arguments[0];
    else if (data.trailPresent)
        reply += data.trail;
    else
        reply += user->getNickname();

    for (std::map<std::string, Channel *>::const_iterator it = Server::getInstance()->getChannels().begin();
        it != Server::getInstance()->getChannels().end();
        it++)
    {
        if (!it->second || !it->second->containsUser(user->getNickname()))
            continue ;

        it->second->broadcast(user->getNickname(), reply);
        it->second->removeUser(user->getNickname());
        
        if (!it->second->getUsers().size())
        {
            std::cout << "Staged channel: " << it->first << std::endl;
            Server::getInstance()->removeChannel(it->first);
        }
    }

    std::string message = "ERROR :Closing Link: ";
    
    message += user->getHostname();
    message += " (Quit: ";

    if (data.arguments.size())
        message += data.arguments[0];
    else if (data.trailPresent)
        message += data.trail;
    else
        message += user->getNickname().empty() ? "*" : user->getNickname();

    message += ")";

    user->sendMessage(message);

    close(user->getSocket());

    Server::getInstance()->removeUser(user);
	
    Server::getInstance()->cleanChannels();

    delete user;

}