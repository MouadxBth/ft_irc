/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/10/03 21:40:24 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "ListCommand.hpp"
#include "Server.hpp"
#include "Utilities.hpp"

//ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED

ListCommand::ListCommand() : Command("LIST", true, false)
{}

ListCommand::~ListCommand()
{}

ListCommand::ListCommand(const ListCommand& instance) : Command(instance)
{}

ListCommand& ListCommand::operator=(const ListCommand& instance)
{
    if (this != &instance)
    {
        Command::operator=(instance);
    }
    return *this;
}

void ListCommand::executeCommand(User *user, Data &data)
{
    std::stringstream iss;
    
    if (!data.arguments.size())
    {    
        for (std::map<std::string, Channel *>::const_iterator it = Server::getInstance()->getChannels().begin();
            it != Server::getInstance()->getChannels().end();
            it++)
        {
            iss.str("");
            iss << it->second->getUsers().size();
            
            if (!user->sendMessage(RPL_LIST(user->getNickname(),
                it->first,
                iss.str(),
                it->second->getTopic())))
                return ;
        }
        user->sendMessage(RPL_LISTEND(user->getNickname()));
        return ;
    }

    std::vector<std::string> channels = split(data.arguments[0], ',');

    Channel *target;

    for (std::vector<std::string>::const_iterator it = channels.begin();
        it != channels.end();
        it++)
    {
        target = Server::getInstance()->getChannel(*it);
        
        if (!target)
            continue;
        
        iss.str("");
        iss << target->getUsers().size();
        
        if (!user->sendMessage(RPL_LIST(user->getNickname(),
            target->getName(),
            iss.str(),
            target->getTopic())))
            return ;
    }
    user->sendMessage(RPL_LISTEND(user->getNickname()));
}