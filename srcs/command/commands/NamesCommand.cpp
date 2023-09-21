/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NamesCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/19 01:44:20 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NamesCommand.hpp"
#include "Server.hpp"

//ERR_TOOMANYMATCHES              ERR_NOSUCHSERVER
//           RPL_NAMREPLY                    RPL_ENDOFNAMES

NamesCommand::NamesCommand() : Command("NAMES", true, false)
{}

NamesCommand::~NamesCommand()
{}

NamesCommand::NamesCommand(const NamesCommand& instance) : Command(instance)
{}

NamesCommand& NamesCommand::operator=(const NamesCommand& instance)
{
    if (this != &instance)
        Command::operator=(instance);
    return *this;
}

void NamesCommand::executeCommand(User *user, Data &data)
{
    if (data.arguments.size())
    {
        std::vector<std::string> channels = split(data.arguments[0], ',');

        Channel *current;

        for (std::vector<std::string>::iterator it = channels.begin();
            it != channels.end();
            it++)
        {
            current = Server::getInstance()->getChannel(*it);

            if (!current)
            {
                user->sendMessage(RPL_ENDOFNAMES(user->getNickname(),
                    *it));
                continue;
            }
            
            if (!user->sendMessage(RPL_NAMREPLY(user->getNickname(),
                current->getName(),
                current->getUserNicknames())))
                return ;

            if (!user->sendMessage(RPL_ENDOFNAMES(user->getNickname(),
                current->getName())))
                return ;
        }
        return ;
    }

    for (std::map<std::string, Channel *>::iterator it = Server::getInstance()->getChannels().begin();
            it != Server::getInstance()->getChannels().end();
            it++)
    {
        if (!it->second)
        {
            if (!user->sendMessage(RPL_ENDOFNAMES(user->getNickname(),
                it->first)))
                return ;
            continue;
        }
        
        if (!user->sendMessage(RPL_NAMREPLY(user->getNickname(),
            it->first,
            it->second->getUserNicknames())))
            return ;

        if (!user->sendMessage(RPL_ENDOFNAMES(user->getNickname(),
            it->first)))
            return ;
    }

    std::vector<std::string> users;

    std::string nicknames;

    for (std::map<std::string, User *>::const_iterator it = Server::getInstance()->getAuthenticatedUsers().begin();
        it != Server::getInstance()->getAuthenticatedUsers().end();
        it++)
    {
        if (it->second && !it->second->getJoinedChannelsCount())
            users.push_back(it->first);
    }

    for (size_t index = 0; index < users.size(); index++)
	{
		if (!nicknames.empty())
			nicknames += " ";
		nicknames += users[index];
	}

    if (!user->sendMessage(RPL_NAMREPLY(user->getNickname(),
            "*",
            nicknames)))
        return ;

    user->sendMessage(RPL_ENDOFNAMES(user->getNickname(),
        nicknames));
}