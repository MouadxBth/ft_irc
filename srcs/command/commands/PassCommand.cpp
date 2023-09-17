/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/17 16:01:03 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCommand.hpp"

//ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED

PassCommand::PassCommand()
    : Command("PASS", "let's you pass n sht", 1, false, false)
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
    std::string nickname;
    std::ostringstream oss;

    oss << "User ";
    oss << user->getUserSocket().fd;
    
    nickname = user->getNickname().empty() 
        ? oss.str()
        : user->getNickname();
    
    if (!getServer())
        return ;
        
    if (user->isAuthenticated())
    {
        user->sendMessage(ERR_ALREADY_REGISTERED(nickname));
        return ;
    }

    if (data.arguments.empty())
    {
        user->sendMessage(ERR_NEED_MORE_PARAMS(nickname, data.command));
        return ;
    }

    if (data.arguments.size() > 1 || !data.trail.empty())
    {
        user->sendMessage(ERR_UNKNOWN_COMMAND(data.command, data.command));
        return ;
    }
    
    // check password
    if (getServer()->getPassword() != data.arguments[0])
    {
        user->sendMessage(ERR_PASSWD_MISMATCH(nickname));
        return ;
    }

    user->setUsedPassword(true);
}