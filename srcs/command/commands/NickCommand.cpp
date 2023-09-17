/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/17 15:55:38 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NickCommand.hpp"
#include <algorithm>

//ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
//           ERR_NICKNAMEINUSE               ERR_NICKCOLLISION
//           ERR_UNAVAILRESOURCE             ERR_RESTRICTED

NickCommand::NickCommand()
    : Command("NICK", "let's you pass n sht", 1, false, false)
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

bool isValidIRCNickname(const std::string& nickname)
{
    // Check if the nickname is empty or too long
    // Check if the nickname starts with a letter
    if (nickname.empty() 
        || nickname.length() > 50
        || !std::isalpha(nickname[0]))
        return (false);

    // Check if the nickname contains only valid characters
    for (size_t index = 0; index < nickname.length(); ++index)
    {   
        if (!std::isalnum(nickname[index])
            && nickname[index] != '-' 
            && nickname[index] != '_' 
            && nickname[index] != '[' 
            && nickname[index] != ']' 
            && nickname[index] != '\\' 
            && nickname[index] != '`' 
            && nickname[index] != '^')
            return (false);
    }

    return (true);
}

bool isNicknameReserved(const std::string& nickname, const std::vector<std::string>& reserved)
{
    // Convert the provided nickname to lowercase for case-insensitive comparison
    std::string lowercaseNickname = nickname;
    
    std::transform(lowercaseNickname.begin(),
        lowercaseNickname.end(),
        lowercaseNickname.begin(), ::tolower);

    // Check if the lowercase nickname is in the list of reserved nicknames
    for (std::vector<std::string>::const_iterator it = reserved.begin();
        it != reserved.end();
        it++)
    {
        if (lowercaseNickname == *it)
            return (true);
    }
    return false;
}


void NickCommand::executeCommand(User *user, Data &data)
{   
    if (!user->hasUsedPassword())
        return ;

    // shouldn't happen
    if (!getServer())
        return ;
        
    if (data.arguments.empty())
    {
        user->sendMessage(ERR_NO_NICKNAME_GIVEN);
        return ;
    }

    if (data.arguments.size() > 1 || !data.trail.empty())
    {
        user->sendMessage(ERR_UNKNOWN_COMMAND(data.command, data.command));
        return ;
    }
    
    // invalid nickname
    if (!isValidIRCNickname(data.arguments[0]))
    {
        user->sendMessage(ERR_ERRONEUS_NICKNAME(data.arguments[0]));
        return ;
    }

    const User *target = getServer()->getUser(data.arguments[0]);
    
    // already used nickname
    if (target && target->getUserSocket().fd != user->getUserSocket().fd)
    {
        user->sendMessage(ERR_NICKNAME_INUSE(data.arguments[0]));
        return ;
    }

    // changing to same nickname simultaneously

    // changing to reserved nickname
    if (isNicknameReserved(data.arguments[0], getServer()->getReservedNicknames()))
    {
        user->sendMessage(ERR_UNAVAIL_RESOURCE(data.arguments[0]));
        return ;
    }

    // restricted nickname
    if (isNicknameReserved(data.arguments[0], getServer()->getRestrictedNicknames()))
    {
        user->sendMessage(ERR_RESTRICTED);
        return ;
    }
    
    user->setNickname(data.arguments[0]);
}