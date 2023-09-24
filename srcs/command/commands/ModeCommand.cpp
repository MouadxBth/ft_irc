/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/24 14:01:15 by mbouthai         ###   ########.fr       */
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
			channel->getChannelUserModes()));
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

	std::pair<User *, ChannelUserModes> channelUser;
	size_t value;
	std::string modeMessage;
	
	int current = 0;

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
				if (current > 2)
					break ;
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
				current++;
				break ;
				
			case 'o':
				if (current > 2)
					break ;
				if (it->parameter.empty())
				{
					message = data.command + " ";
					message += it->add ? "+o" : "-o";
					
					user->sendMessage(ERR_NEEDMOREPARAMS(user->getNickname(), message));
					break ;
				}
				
				if (!channel->containsUser(it->parameter))
				{
					user->sendMessage(ERR_USERNOTINCHANNEL(user->getNickname(),
						it->parameter,
						channel->getName()));
					break ;
				}
				
				channelUser = channel->getUser(it->parameter);

				if ((it->add && channelUser.second.channelOperator)
					|| (!it->add && !channelUser.second.channelOperator))
				{
					user->sendMessage(ERR_KEYSET(user->getNickname(), channel->getName()));
					break ;
				}
				
				channel->setOperator(it->parameter, it->add);

				message += it->add ? "+o " : "-o ";
				message += it->parameter;

				channel->announce(message);
				current++;
				break ;
				
			case 'l':
				if (current > 2)
					break ;
				if (it->add && channel->isUserLimitSet())
				{
					user->sendMessage(ERR_KEYSET(user->getNickname(), channel->getName()));
					break ;
				}
				
				if (!it->add)
				{
					std::cout << "REMOVED" << std::endl;
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

				if (value <= 0)
				{
					break ;
				}

				std::cout << "VALUE: " << value << std::endl;
				
				channel->setUserLimit(it->add);
				channel->setMaximumCapacity(value);

				message += "+l ";
				message += it->parameter;

				channel->announce(message);
				current++;
				break ;
				
			default:
				modeMessage += it->mode;
				user->sendMessage(ERR_UNKNOWNMODE(user->getNickname(), modeMessage, channel->getName()));
				break ;
		}
	}
	
}