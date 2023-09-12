/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/12 10:40:11 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCommand.hpp"

PassCommand::PassCommand()
    : Command("PASS", "let's you pass n sht", 0, true)
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
    if (user->isAuthenticated() || user->getState() == User::STAGE_THREE)
    {
        user->sendMessage(ERR_ALREADY_REGISTERED(user->getNickname()));
        return ;
    }
    // check password
    user->incrementState();

    if (user->getState() == User::STAGE_THREE)
    {
        // welcome
    }
}