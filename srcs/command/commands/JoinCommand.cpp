/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/20 16:30:30 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JoinCommand.hpp"
#include "Server.hpp"
#include "CommandManager.hpp"

//ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
//           ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
//           ERR_CHANNELISFULL               ERR_BADCHANMASK
//           ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
//           ERR_TOOMANYTARGETS              ERR_UNAVAILRESOURCE
//           RPL_TOPIC

JoinCommand::JoinCommand() : Command("JOIN", true, false)
{}

JoinCommand::~JoinCommand()
{}

JoinCommand::JoinCommand(const JoinCommand& instance) : Command(instance)
{}

JoinCommand& JoinCommand::operator=(const JoinCommand& instance)
{
    if (this != &instance)
        Command::operator=(instance);
    return *this;
}

void JoinCommand::executeCommand(User *user, Data &data)
{   
    if (data.arguments.empty())
    {
        user->sendMessage(ERR_NEEDMOREPARAMS(user->getNickname(), data.command));
        return ;
    }

    if (data.arguments.size() == 1 && data.arguments[0].size() == 1)
    {
        if (data.arguments[0] != "0")
            return ;
        
        Data partData = emptyCommandData();

        partData.command = "PART";

        std::vector<Data> collection;
        
        for (std::map<std::string, Channel *>::const_iterator it = Server::getInstance()->getChannels().begin();
            it != Server::getInstance()->getChannels().end();
            it++)
        {
            if (!it->second || !it->second->containsUser(user->getNickname()))
                continue;
            
            partData.arguments.clear();
            partData.arguments.push_back((it)->second->getName());

            collection.push_back(partData);  
        }

        for (std::vector<Data>::iterator it = collection.begin(); it != collection.end(); it++)
            CommandManager::getInstance()->executeCommand(user,  *it);

        Server::getInstance()->cleanChannels();
        return ;
    }

    std::vector<std::string> channels = split(data.arguments[0], ',');
    std::vector<std::string> passwords;

    if (data.arguments.size() == 2)
        passwords = split(data.arguments[1], ',');


    std::map<std::string, std::string> elements = getElements(channels, passwords);

    Data namesData;

    int current = 0;

    for (std::map<std::string, std::string>::const_iterator it = elements.begin();
        it != elements.end();
        it++)
    {
        if (current > 3)
        {
            user->sendMessage(ERR_TOOMANYTARGETS(user->getNickname(),
                it->first,
                "Reduce the number of channels please, skipping targets..."));
            return ;
        }
        
        Channel *target = Server::getInstance()->getChannel(it->first);

        if (!target)
        {
            Channel *newChannel = new Channel();

            newChannel->setName(it->first);
            newChannel->addUser(user, true, true, true);
            user->setJoinedChannelsCount(user->getJoinedChannelsCount() + 1);

            std::string message = ":" + user->getNickname() + "!" 
                + user->getUsername() + "@" 
                + user->getHostname() + " " 
                + data.command + " " + data.arguments[0];
                
            Server::getInstance()->getChannels()[it->first] = newChannel;
            
            newChannel->announce(message);

            namesData = emptyCommandData();
            namesData.command = "NAMES";
            namesData.arguments.push_back(newChannel->getName());

            CommandManager::getInstance()->executeCommand(user, namesData);
            current++;
            continue ;
        }
        
        if (target->containsUser(user->getNickname()))
            continue ;
        
        if (target->isUserBanned(user->getNickname()))
        {
            user->sendMessage(ERR_BANNEDFROMCHAN(user->getNickname(), target->getName()));
            continue;
        }
        if (target->isInviteOnly() && !target->isInvited(user->getNickname()))
        {
            user->sendMessage(ERR_INVITEONLYCHAN(user->getNickname(), target->getName()));
            continue;
        }
        if (target->isChannelKeySet() && target->getPassword() != it->second)
        {
            user->sendMessage(ERR_BADCHANNELKEY(user->getNickname(), target->getName()));
            continue;
        }
        if (target->isUserLimitSet() && target->getUsers().size() >= target->getMaximumCapacity())
        {
            user->sendMessage(ERR_CHANNELISFULL(user->getNickname(), target->getName()));
            continue ;
        }
        if (user->getJoinedChannelsCount() > 10)
        {
            user->sendMessage(ERR_TOOMANYCHANNELS(user->getNickname(), target->getName()));
            continue;
        }

        if (target->isInviteOnly())
            target->removeInvite(user->getNickname());
        
        target->addUser(user, false, false, false);

        user->setJoinedChannelsCount(user->getJoinedChannelsCount() + 1);
        
        std::string message = ":" + user->getNickname() + "!" 
                + user->getUsername() + "@" 
                + user->getHostname() + " " 
                + data.command + " " + target->getName();
        
        target->announce(message);
        
        namesData = emptyCommandData();
        namesData.command = "NAMES";
        namesData.arguments.push_back(target->getName());

        current++;

        CommandManager::getInstance()->executeCommand(user, namesData);
        //temporary
        //user->sendMessage("You joined!");
        // information about all commands his server receives affecting the channel.  This
        // includes JOIN, MODE, KICK, PART, QUIT and of course PRIVMSG/NOTICE
        // user receives a JOIN message as confirmation and is then sent the channel's 
        // topic (using RPL_TOPIC) and the list of users who are on the channel (using RPL_NAMREPLY), 
        // which MUST include the user joining
    }
}