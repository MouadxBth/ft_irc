/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/19 01:44:20 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>

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
    std::ostringstream oss("User ");

    oss << user->getUserSocket().fd;
        
    if (user->isAuthenticated())
    {
        user->sendMessage(ERR_ALREADY_REGISTERED(user->getNickname()));
        return ;
    }

    if (data.arguments.empty())
    {
        user->sendMessage(ERR_NEED_MORE_PARAMS(oss.str(), data.command));
        return ;
    }
    
    // check password
    if (Server::getInstance()->getPassword() != data.arguments[0])
    {
        user->sendMessage(ERR_PASSWD_MISMATCH(oss.str()));
        return ;
    }

    user->setUsedPassword(true);
}