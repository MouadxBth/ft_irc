/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:55:04 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/18 11:59:10 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ModeCommand.hpp"

//			ERR_NEEDMOREPARAMS              ERR_KEYSET
//          ERR_NOCHANMODES                 ERR_CHANOPRIVSNEEDED
//          ERR_USERNOTINCHANNEL            ERR_UNKNOWNMODE

//          RPL_CHANNELMODEIS
//          RPL_BANLIST                     RPL_ENDOFBANLIST
//          RPL_EXCEPTLIST                  RPL_ENDOFEXCEPTLIST
//          RPL_INVITELIST                  RPL_ENDOFINVITELIST
//          RPL_UNIQOPIS

ModeCommand::ModeCommand()
    : Command("MODE", "let's you pass n sht", -1, true, false)
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

bool takesParam(int c)
{
	return (c == 't' || c == 'k' || c == 'o' || c == 'l');
}

typedef struct s_mode
{
	int mode;
	bool add;
	std::string param;
}	Mode;

std::vector<Mode> transform(std::vector<std::string>& args)
{
	std::vector<Mode> result;

	bool takesb;
	
	for (size_t index = 0; index < args.size(); index++)
	{
		if (args[index].size() == 2)
		{
			Mode mode;

			mode.mode = args[index][1];
			mode.add = args[index][0] == '+';
			
			if (takesParam(args[index][1]))
			{
				if (index + 1 >= args.size())
					return ;
				mode.param = args[index + 1];
			}
			result.push_back(mode);
		}

		
		if (args[index].size() > 2)
		{
			std::vector<std::string> moreArgs;
			int takes = 0;

			if (index + 1 >= args.size())
				return ;

			moreArgs = split(args[index + 1], ',');

			for (size_t j = 1, temp = 0; j < args[index].size(); j++)
			{
				takesb = takesParam(args[index][j]);
				if (takesb)
					takes++;
				if (takes > moreArgs.size())
					return ;
				
				Mode riz;

				riz.mode = args[index][j];
				riz.add = args[index][0] == '+';
				
				if (takesb)
					riz.param = moreArgs[temp++];
			}
			index++;
		}
	}
}

void ModeCommand::executeCommand(User *user, Data &data)
{   
    if (data.arguments.empty())
    {
        user->sendMessage(ERR_NEED_MORE_PARAMS(user->getNickname(), data.command));
        return ;
    }

	// query channel modes
	if (data.arguments.size() == 1)
	{
		return ;
	}

	std::vector<std::string> modifications;

	for (size_t index = 1; index < data.arguments.size(); index++)
	{
		if (data.arguments[index][0] != '+' && data.arguments[index][0] != '-')
		{
			return ;
		}

		
		
		if (data.arguments[index][0] == '+' || data.arguments[index][0] == '-')
		{
			if (data.arguments[index].size() == 2)
			{
				if (takesParam(data.arguments[index][1]))
				{
					if ()
				}
			}	
		}
	}

	for (std::vector<std::string>::const_iterator it = data.arguments.begin();
		it != data.arguments.end();
		it++)
	{
		if ((*it)[0] == '+' || (*it)[0] == '-')
			
	}

	// no + or - found
	if (data.arguments[1][0] != '+' || data.arguments[1][0] != '-')
	{
		return ;
	}

	// used only + or -
	if (data.arguments[1].size() == 1)
	{
		return ;
	}

	// tkol
	bool add = data.arguments[1][0] == '+';

	for (size_t index = 1; index < data.arguments)
	
    
	
}