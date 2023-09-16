/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/16 15:23:46 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "KickCommand.hpp"

//ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
//           ERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
//           ERR_USERNOTINCHANNEL            ERR_NOTONCHANNEL

KickCommand::KickCommand()
    : Command("KICK", "let's you pass n sht", 2, true, false)
{}

KickCommand::~KickCommand()
{}

KickCommand::KickCommand(const KickCommand& instance) : Command(instance)
{}

KickCommand& KickCommand::operator=(const KickCommand& instance)
{
    if (this != &instance)
    {
        Command::operator=(instance);
    }
    return *this;
}

bool    verify(User *user, Channel *channel, std::string& name)
{
    if (!channel)
    {
        user->sendMessage(ERR_NO_SUCH_CHANNEL(user->getNickname(), name));
        return (false);
    }

    if (!channel->containsUser(user->getNickname()))
    {
        user->sendMessage(ERR_NOT_ON_CHANNEL(user->getNickname(), channel->getName()));
        return (false);
    }
    
    std::pair<User *, Modes> channelUser = channel->getUser(user->getNickname());

    if (!channelUser.second.channelOperator)
    {
        user->sendMessage(ERR_CHAN_O_PRIVS_NEEDED(user->getNickname(), channel->getName()));
        return (false);
    }   

    return (true);
}

void KickCommand::executeCommand(User *user, Data &data)
{   
    if (!getServer())
        return ;

    if (data.arguments.empty())
    {
        user->sendMessage(ERR_NEED_MORE_PARAMS(user->getNickname(), data.command));
        return ;
    }
    
    std::vector<std::string> channels = split(data.arguments[0], ',');
    std::vector<std::string> users = split(data.arguments[1], ',');

    if (channels.size() > users.size())
    {
        user->sendMessage(ERR_UNKNOWN_COMMAND(data.command, data.command));
        return ;
    }
    
    std::vector<std::string> replies;

    std::string message;

    if (channels.size() == 1)
    {
        Channel *channelTarget = getServer()->getChannel(channels[0]);

        if (!verify(user, channelTarget, channels[0]))
            return ;

        for (std::vector<std::string>::const_iterator it = users.begin(); it != users.end(); it++)
        {
            if (!channelTarget->containsUser(*it))
            {
                user->sendMessage(ERR_USER_NOT_IN_CHANNEL(user->getNickname(), *it, channelTarget->getName()));
                continue ;
            }
            if (user->getNickname() == (*it))
                continue ;
                
            message = ":" + user->getNickname() + "!" 
                + user->getUsername() + "@" 
                + user->getHostname() + " " 
                + data.command + " " + channelTarget->getName() + " " + *it;
            
            if (data.trailPresent)
                message += " :" + data.trail;

            replies.push_back(message);
            
            channelTarget->getUser(*it).first->sendMessage(message);
            channelTarget->removeUser(*it);
        }
        channelTarget->announce(replies);
        return ;
    }

    for (size_t index = 0; index < channels.size() && index < users.size(); index++)
    {
        Channel *channelTarget = getServer()->getChannel(channels[index]);

        if (!verify(user, channelTarget, channels[index]))
            continue ;

        if (!channelTarget->containsUser(users[index]))
        {
            user->sendMessage(ERR_USER_NOT_IN_CHANNEL(user->getNickname(), users[index], channels[index]));
            continue ;
        }

        if (user->getNickname() == users[index])
                continue ;

        message = ":" + user->getNickname() + "!" 
                + user->getUsername() + "@" 
                + user->getHostname() + " " 
                + data.command + " " + channels[index] + " " + users[index];
            
        if (data.trailPresent)
            message += " :" + data.trail;
        
        channelTarget->getUser(users[index]).first->sendMessage(message);
        channelTarget->removeUser(users[index]);

        channelTarget->announce(message);
    }
    
}