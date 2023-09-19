/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/19 01:44:27 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>

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
    if (!user->hasUsedPassword())
        return ;

    if (data.arguments.empty())
    {
        user->sendMessage(ERR_NO_NICKNAME_GIVEN);
        return ;
    }
    
    // invalid nickname
    if (!isValidIRCNickname(data.arguments[0]))
    {
        user->sendMessage(ERR_ERRONEUS_NICKNAME(data.arguments[0]));
        return ;
    }

    const User *target = Server::getInstance()->getAuthenticatedUser(data.arguments[0]);
    
    // already used nickname
    if (target && target->getUserSocket().fd != user->getUserSocket().fd)
    {
        user->sendMessage(ERR_NICKNAME_INUSE(data.arguments[0]));
        return ;
    }

    // changing to reserved nickname
    if (containsString(Server::getInstance()->getReservedNicknames(), data.arguments[0]))
    {
        user->sendMessage(ERR_UNAVAIL_RESOURCE(data.arguments[0]));
        return ;
    }

    // restricted nickname
    if (containsString(Server::getInstance()->getRestrictedNicknames(), data.arguments[0]))
    {
        user->sendMessage(ERR_RESTRICTED);
        return ;
    }
    
    user->setNickname(data.arguments[0]);
}