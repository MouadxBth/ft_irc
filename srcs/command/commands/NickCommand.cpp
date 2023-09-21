/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/21 18:00:19 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NickCommand.hpp"
#include "Server.hpp"

//ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
//           ERR_NICKNAMEINUSE               ERR_NICKCOLLISION
//           ERR_UNAVAILRESOURCE             ERR_RESTRICTED

NickCommand::NickCommand() : Command("NICK", false, false)
{}

NickCommand::~NickCommand()
{}

NickCommand::NickCommand(const NickCommand& instance) : Command(instance)
{}

NickCommand& NickCommand::operator=(const NickCommand& instance)
{
    if (this != &instance)
    {
        Command::operator=(instance);
    }
    return *this;
}

void NickCommand::executeCommand(User *user, Data &data)
{
    std::string nickname = user->getNickname().empty() ? "*" : user->getNickname();

    
    if (!user->hasUsedPassword())
        return ;

    if (data.arguments.empty())
    {
        user->sendMessage(ERR_NONICKNAMEGIVEN(nickname));
        return ;
    }
    
    // invalid nickname
    if (!isValidIRCNickname(data.arguments[0]))
    {
        user->sendMessage(ERR_ERRONEUSNICKNAME(nickname, data.arguments[0]));
        return ;
    }

    const User *target = Server::getInstance()->getAuthenticatedUser(data.arguments[0]);
    
    // already used nickname
    if (target && target->getUserEPollEvent().data.fd != user->getUserEPollEvent().data.fd)
    {
        user->sendMessage(ERR_NICKNAMEINUSE(nickname, data.arguments[0]));
        return ;
    }

    // changing to reserved nickname
    if (containsString(Server::getInstance()->getReservedNicknames(), data.arguments[0]))
    {
        user->sendMessage(ERR_UNAVAILRESOURCE(nickname, data.arguments[0]));
        return ;
    }

    // restricted nickname
    if (containsString(Server::getInstance()->getRestrictedNicknames(), data.arguments[0]))
    {
        user->sendMessage(ERR_RESTRICTED(nickname));
        return ;
    }
    
    user->setNickname(data.arguments[0]);
}