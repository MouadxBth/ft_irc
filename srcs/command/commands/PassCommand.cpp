/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/10/03 16:25:04 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCommand.hpp"
#include "Server.hpp"

//ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED

PassCommand::PassCommand() : Command("PASS", false, false)
{}

PassCommand::~PassCommand()
{}

PassCommand::PassCommand(const PassCommand& instance) : Command(instance)
{}

PassCommand& PassCommand::operator=(const PassCommand& instance)
{
    if (this != &instance)
    {
        Command::operator=(instance);
    }
    return *this;
}

void PassCommand::executeCommand(User *user, Data &data)
{
    const std::string nickname = user->getNickname().empty() ? "*" : user->getNickname();
    if (user->isAuthenticated())
    {
        user->sendMessage(ERR_ALREADYREGISTERED(nickname));
        return ;
    }

    if (data.arguments.empty())
    {
        user->sendMessage(ERR_NEEDMOREPARAMS(nickname, data.command));
        return ;
    }
    
    // check password
    if (Server::getInstance()->getPassword() != data.arguments[0])
    {
        user->sendMessage(ERR_PASSWDMISMATCH(nickname));
        return ;
    }

    user->setUsedPassword(true);
}