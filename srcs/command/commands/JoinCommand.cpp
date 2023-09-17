/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/17 16:04:34 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JoinCommand.hpp"
#include "CommandManager.hpp"

//ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
//           ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
//           ERR_CHANNELISFULL               ERR_BADCHANMASK
//           ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
//           ERR_TOOMANYTARGETS              ERR_UNAVAILRESOURCE
//           RPL_TOPIC

JoinCommand::JoinCommand()
    : Command("JOIN", "let's you pass n sht", -1, true, false)
{}

JoinCommand::~JoinCommand()
{}

JoinCommand::JoinCommand(const JoinCommand& instance) : Command(instance)
{}

JoinCommand& JoinCommand::operator=(const JoinCommand& instance)
{
    if (this != &instance)
    {
        Command::operator=(instance);
    }
    return *this;
}

std::map<std::string, std::string> getElements(std::vector<std::string>& channels, std::vector<std::string>& passwords)
{
	std::map<std::string, std::string> elements;

    size_t index = 0;

	for (; index < passwords.size(); index++)
	{
		elements[channels[index]] = passwords[index];
	}

    while (index < channels.size())
    {
        elements[channels[index++]] = "";
    }

	return (elements);
}

void printDatas(Data &data)
{
	std::cout << "=>Command:" << "\n\tPrefix: " << data.prefix << " " << data.prefix.size()
		<< "\n\tCommand: " << data.command << " " << data.command.size()
		<< "\n\tArguments: ";
	
	for (std::vector<std::string>::iterator it = data.arguments.begin();
		it != data.arguments.end();
		it++)
	{
		std::cout << *it << " ";
	}
	
	std::cout << "\n\tArgument Size: " << data.arguments.size()
		<< "\n\tTrail: " << data.trail << " " << data.trail.size() << std::endl;
}

void JoinCommand::executeCommand(User *user, Data &data)
{   
    if (!getServer())
        return ;
    
    if (data.arguments.empty())
    {
        user->sendMessage(ERR_NEED_MORE_PARAMS(user->getNickname(), data.command));
        return ;
    }

    if (data.arguments.size() > 2 || (data.arguments.size() == 2 && data.trailPresent))
    {
        user->sendMessage(ERR_UNKNOWN_COMMAND(data.command, data.command));
        return ;
    }

    if (data.arguments.size() == 1 && data.arguments[0].size() == 1)
    {
        if (data.arguments[0] != "0")
            user->sendMessage(ERR_UNKNOWN_COMMAND(data.command, data.command));
        else
        {
            std::vector<std::string> empty;

            Data partData = {
                .prefix = "",
                .command = "PART",
                .arguments = empty,
                .trail = "",
                .valid = true,
                .trailPresent = false
            };

            std::vector<Data> collection;
            
            for (std::vector<Channel *>::const_iterator it = getServer()->getChannels().begin();
                it != getServer()->getChannels().end();
                it++)
            {
                if (!(*it) 
                    || !(*it)->containsUser(user->getNickname()))
                    continue;
                
                partData.arguments.clear();
                partData.arguments.push_back((*it)->getName());

                //printDatas(partData);

                collection.push_back(partData);  
            }

            for (std::vector<Data>::iterator it = collection.begin(); it != collection.end(); it++)
            {
                getServer()->getCommandManager()->executeCommand(user,  *it);
            }

            getServer()->cleanChannels();
        }
        return ;
    }

    std::vector<std::string> channels = split(data.arguments[0], ',');
    std::vector<std::string> passwords;

    if (data.arguments.size() == 2)
        passwords = split(data.arguments[1], ',');

    if (passwords.size() > channels.size())
    {
        user->sendMessage(ERR_UNKNOWN_COMMAND(data.command, data.command));
        return ;
    }

    if (channels.size() > 3)
    {
        user->sendMessage(ERR_TOOMANYTARGETS(data.arguments[0],
            "ERR_TOOMANYTARGETS",
            "Reduce the number of channels please."));
        return ;
    }

    std::map<std::string, std::string> elements = getElements(channels, passwords);

    int joinedChannels = 0;

    for (std::vector<Channel *>::const_iterator it = getServer()->getChannels().begin();
        it != getServer()->getChannels().end();
        it++)
    {
        if ((*it)->containsUser(user->getNickname()))
            joinedChannels++;
    }

    for (std::map<std::string, std::string>::const_iterator it = elements.begin();
        it != elements.end();
        it++)
    {
        Channel *target = getServer()->getChannel(it->first);

        if (!target)
        {
            Channel *newChannel = new Channel();

            newChannel->setName(it->first);
            newChannel->addUser(user, true, true);
            if (!it->second.empty())
            {
                newChannel->setPassword(it->second);
                newChannel->setChannelKey(true);
            }
            getServer()->getChannels().push_back(newChannel);
            user->sendMessage("You created a new channel!");
            continue ;
            // i dont think this is valid
            //user->sendMessage(ERR_NO_SUCH_CHANNEL(user->getNickname(), it->first));
            //continue;
        }
        if (target->containsUser(user->getNickname()))
            continue ;
        if (target->isUserBanned(user->getNickname()))
        {
            user->sendMessage(ERR_BANNED_FROM_CHAN(target->getName()));
            continue;
        }
        if (target->isInviteOnly() && !target->isInvited(user->getNickname()))
        {
            user->sendMessage(ERR_INVITE_ONLY_CHAN(target->getName()));
            continue;
        }
        if (target->isChannelKeySet() && target->getPassword() != it->second)
        {
            user->sendMessage(ERR_BAD_CHANNEL_KEY(target->getName()));
            continue;
        }
        if (joinedChannels > 10)
        {
            user->sendMessage(ERR_TOO_MANY_CHANNELS(target->getName()));
            continue;
        }

        if (target->isInviteOnly())
            target->removeInvite(user->getNickname());
        
        target->addUser(user, false, false);
        //temporary
        user->sendMessage("You joined!");
        // information about all commands his server receives affecting the channel.  This
        // includes JOIN, MODE, KICK, PART, QUIT and of course PRIVMSG/NOTICE
        // user receives a JOIN message as confirmation and is then sent the channel's 
        // topic (using RPL_TOPIC) and the list of users who are on the channel (using RPL_NAMREPLY), 
        // which MUST include the user joining
    }
}