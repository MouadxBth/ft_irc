/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/13 19:16:28 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserCommand.hpp"

//ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
//USER guest 0 * :Ronnie Reagan 
//USER guest 8 * :Ronnie Reagan <-- invis

UserCommand::UserCommand()
    : Command("USER", "let's you pass n sht", -1, false, false)
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
    
    if (!getServer())
        return ;
        
    if (user->isAuthenticated())
    {
        user->sendMessage(ERR_ALREADY_REGISTERED(user->getNickname()));
        return ;
    }

    if (data.arguments.empty() || data.arguments.size() < 3)
    {
        user->sendMessage(ERR_NEED_MORE_PARAMS(user->getNickname(), data.command));
        return ;
    }

    if (data.arguments.size() > 3 && !data.trail.empty())
    {
        user->sendMessage(ERR_UNKNOWN_COMMAND(user->getNickname(), data.command));
        return ;
    }
    
    user->setUsername(data.arguments[0]);
    if (data.arguments.size() == 3)
        user->setFullname(data.trail);
    else
        user->setFullname(data.arguments[3]);
    // set user mode

    user->setAuthenticated(true);

    user->sendMessage(RPL_WELCOME(user->getNickname()));

    std::cout << *user << std::endl;
}