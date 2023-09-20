/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/20 01:28:43 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ModeCommand.hpp"
#include "Server.hpp"
#include "Utilities.hpp"

//			ERR_NEEDMOREPARAMS              ERR_KEYSET

//          ERR_NOCHANMODES                 ERR_CHANOPRIVSNEEDED

//          ERR_USERNOTINCHANNEL            ERR_UNKNOWNMODE

//          RPL_CHANNELMODEIS
//          RPL_BANLIST                     RPL_ENDOFBANLIST
//          RPL_EXCEPTLIST                  RPL_ENDOFEXCEPTLIST
//          RPL_INVITELIST                  RPL_ENDOFINVITELIST
//          RPL_UNIQOPIS

ModeCommand::ModeCommand() : Command("MODE", true, false)
{}

ModeCommand::~ModeCommand()
{}

ModeCommand::ModeCommand(const ModeCommand& instance) : Command(instance)
{}

ModeCommand& ModeCommand::operator=(const ModeCommand& instance)
{
    if (this != &instance)
    {
        Command::operator=(instance);
    }
    return *this;
}

void ModeCommand::executeCommand(User *user, Data &data)
{   
    if (data.arguments.empty())
    {
        user->sendMessage(ERR_NEEDMOREPARAMS(user->getNickname(), data.command));
        return ;
    }

	std::string message;
	
	Channel *channel = Server::getInstance()->getChannel(data.arguments[0]);
	
	if (!channel)
		return ;

	// query channel modes
	if (data.arguments.size() == 1)
	{
		user->sendMessage(RPL_CHANNELMODEIS(user->getNickname(),
			channel->getName(),
			channel->getModes()));
		return ;
	}

	std::vector<ChannelMode> modes = parseModeArguments(data.arguments);

	if (!modes.size())
		return ;

	if (!channel->getUser(user->getNickname()).second.channelOperator)
	{
		user->sendMessage(ERR_CHANOPRIVSNEEDED(user->getNickname(), channel->getName()));
		return ;
	}

	std::pair<User *, Modes> channelUser;
	size_t value;
	std::string modeMessage;

	for (std::vector<ChannelMode>::const_iterator it = modes.begin();
		it != modes.end();
		it++)
	{
		message = ":" + user->getNickname() + "!" 
                + user->getUsername() + "@" 
                + user->getHostname() + " " 
                + data.command + " " + data.arguments[0] + " ";
				
		switch (it->mode)
		{
			case 'i':
				if (it->add && channel->isInviteOnly())
				{
					user->sendMessage(ERR_KEYSET(user->getNickname(), channel->getName()));
					break ;
				}
				
				message += it->add ? "+i" : "-i";

				channel->announce(message);
				channel->setInviteOnly(it->add);
				break ;
				
			case 't':
				if (it->add && channel->isTopicSettableByChannelOperatorOnly())
				{
					user->sendMessage(ERR_KEYSET(user->getNickname(), channel->getName()));
					break ;
				}

				message += it->add ? "+t" : "-t";

				channel->announce(message);
				channel->setTopicSettableByChannelOperatorOnly(it->add);
				break ;
				
			case 'k':
				if (it->add && channel->isChannelKeySet())
				{
					user->sendMessage(ERR_KEYSET(user->getNickname(), channel->getName()));
					break ;
				}
				
				if (it->add && it->parameter.empty())
				{
					message = data.command + " +k";
					
					user->sendMessage(ERR_NEEDMOREPARAMS(user->getNickname(), message));
					break ;
				}
					
				if (it->add)
					channel->setPassword(it->parameter);
				
				channel->setChannelKey(it->add);

				message += it->add ? "+k" : "-k";

				if (!it->parameter.empty())
					message += " " + it->parameter;

				channel->announce(message);
				break ;
				
			case 'o':
				if (it->parameter.empty())
				{
					message = data.command + " ";
					message += it->add ? "+o" : "-o";
					
					user->sendMessage(ERR_NEEDMOREPARAMS(user->getNickname(), message));
					break ;
				}
				
				channelUser = channel->getUser(it->parameter);
				
				if (!channelUser.first)
				{
					user->sendMessage(ERR_NOTONCHANNEL(user->getNickname(), channel->getName()));
					break ;
				}

				if ((it->add && channelUser.second.channelOperator)
					|| (!it->add && !channelUser.second.channelOperator))
				{
					user->sendMessage(ERR_KEYSET(user->getNickname(), channel->getName()));
					break ;
				}
				
				channelUser.second.channelOperator = it->add;

				message += it->add ? "+o " : "-o ";
				message += it->parameter;

				channel->announce(message);
				break ;
				
			case 'l':
				if (it->add && channel->isUserLimitSet())
				{
					user->sendMessage(ERR_KEYSET(user->getNickname(), channel->getName()));
					break ;
				}
				
				if (!it->add)
				{
					channel->setUserLimit(false);

					message += "-l";
					channel->announce(message);
					break ;
				}

				if (it->add && it->parameter.empty())
				{
					message = data.command + " ";
					message += "+l";
					user->sendMessage(ERR_NEEDMOREPARAMS(user->getNickname(), message));
					break ;
				}

				if (!isNumber(it->parameter.c_str()))
				{
					break ;
				}
				
				value = getNumber(it->parameter);
				
				channel->setUserLimit(it->add);
				channel->setMaximumCapacity(value);

				message += "+o ";
				message += it->parameter;

				channel->announce(message);
				break ;
				
			default:
				modeMessage += it->mode;
				user->sendMessage(ERR_UNKNOWNMODE(user->getNickname(), modeMessage, channel->getName()));
				break ;
		}
	}
	
}