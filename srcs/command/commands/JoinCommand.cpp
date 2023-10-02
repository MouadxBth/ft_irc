/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/26 15:46:48 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JoinCommand.hpp"
#include "Server.hpp"
#include "CommandManager.hpp"
#include "Utilities.hpp"

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

    std::string message;

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

        if (it->first[0] != '#')
        {
            user->sendMessage(ERR_NOSUCHCHANNEL(user->getNickname(), it->first));
            continue; 
        }
        
        Channel *target = Server::getInstance()->getChannel(it->first);

        ChannelUserModes modes = {
            .channelOwner = true,
            .channelOperator = true,
            .voice = true
        };

        if (!target)
        {
            if (it->first.size() > 50)
                continue ;
            Channel *newChannel = new Channel(it->first);

            newChannel->addUser(user, modes);
            user->setJoinedChannelsCount(user->getJoinedChannelsCount() + 1);

            message = ":" + user->getNickname() + "!" 
                + user->getUsername() + "@" 
                + user->getHostname() + " "     
                + data.command + " " + newChannel->getName();
                
            Server::getInstance()->addChannel(newChannel);
            
            newChannel->announce(message);

            namesData = emptyCommandData();
            namesData.command = "NAMES";
            namesData.arguments.push_back(newChannel->getName());

            CommandManager::getInstance()->executeCommand(user, namesData);

            std::cout << *newChannel << std::endl;
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
        if (target->isInviteOnly() && !target->isUserInvited(user->getNickname()))
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
            target->unInviteUser(user->getNickname());
        
        modes.channelOwner = false;
        modes.channelOperator = false;
        modes.voice = false;
        
        target->addUser(user, modes);

        user->setJoinedChannelsCount(user->getJoinedChannelsCount() + 1);
        
        message = ":" + user->getNickname() + "!" 
                + user->getUsername() + "@" 
                + user->getHostname() + " " 
                + data.command + " " + target->getName();
        
        target->announce(message);
        
        namesData = emptyCommandData();
        namesData.command = "NAMES";
        namesData.arguments.push_back(target->getName());

        current++;

        CommandManager::getInstance()->executeCommand(user, namesData);
    }
}