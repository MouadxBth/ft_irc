/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 12:04:38 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/19 17:19:19 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandManager.hpp"
#include "Command.hpp"
#include "CommandReplies.hpp"

# include "PassCommand.hpp"
# include "UserCommand.hpp"
# include "NickCommand.hpp"
# include "PrivMsgCommand.hpp"
# include "InviteCommand.hpp"
# include "JoinCommand.hpp"
# include "TopicCommand.hpp"
# include "KickCommand.hpp"
# include "PartCommand.hpp"
# include "QuitCommand.hpp"
# include "MotdCommand.hpp"
# include "NamesCommand.hpp"
# include "NoticeCommand.hpp"

CommandManager *CommandManager::_instance = NULL;

CommandManager::CommandManager()
{}

CommandManager::~CommandManager()
{}

CommandManager::CommandManager(const CommandManager& instance)
{
    (void) instance;
}

CommandManager& CommandManager::operator=(const CommandManager& instance)
{
    (void) instance;
    return (*this);
}

const std::map<std::string, Command *>& CommandManager::getRegisteredCommands() const
{
    return (_registeredCommands);
}

void    CommandManager::registerCommand(Command *command)
{
    if (!command 
    || command->getName().empty())
        return ;
        
    _registeredCommands[command->getName()] = command;
}

void    CommandManager::unregisterCommand(const std::string& command)
{
    if (command.empty())
        return ;
    _registeredCommands.erase(command);
}

void    CommandManager::executeCommand(User *user, Data &data)
{
    if (!user)
        return ;
    
    std::string nickname;
    
    nickname = user->getNickname().empty() 
        ? "*"
        : user->getNickname();

    std::map<std::string, Command *>::iterator it = _registeredCommands.find(data.command);
    
    if ((it == _registeredCommands.end() 
        || !it->second)
        || !isKnownCommand(data.command))
    {
        user->sendMessage(ERR_UNKNOWNCOMMAND(nickname, data.command));
        return ;
    }

    if (!data.valid)
    {
        return ;
    }
    
    if (!user->isAuthenticated() && it->second->isAuthRequired())
    {
        user->sendMessage(ERR_NOTREGISTERED(nickname));
        return ;
    }

    if (data.arguments.size() > 15 || (data.trail.empty() && it->second->requiresTrail()))
    {
        user->sendMessage(ERR_UNKNOWNCOMMAND(nickname, data.command));
        return ;
    }

    it->second->executeCommand(user, data);
}

void    CommandManager::registerCommands(Command *command, ...)
{
    va_list args;
    
    va_start(args, command);

    Command *current = command;

    while (current)
    {
        registerCommand(current);
        current = va_arg(args, Command *);
    }
        
    va_end(args);
}

void    CommandManager::unregisterCommands(std::string *command, ...)
{
    va_list args;
    va_start(args, command);

    std::string *current = command;
    
    while (current && !current->empty())
    {
        unregisterCommand(*current);
        current = va_arg(args, std::string *);
    }

    va_end(args);
}

void    CommandManager::cleanUp()
{
    for (std::map<std::string, Command *>::const_iterator it = _registeredCommands.begin();
		it != _registeredCommands.end();
		it++)
	{
		if (it->second)
			delete it->second;
	}
}

CommandManager *CommandManager::getInstance()
{
    if (!_instance)
    {
        _instance = new CommandManager();
        
        _instance->registerCommands(new PassCommand(),
            new NickCommand(),
            new UserCommand(),
            new PrivMsg(),
            new InviteCommand(),
            new JoinCommand(),
            new TopicCommand(),
            new KickCommand(),
            new PartCommand(),
            new QuitCommand(),
            new MotdCommand(),
            new NamesCommand(),
            new NoticeCommand(),
            NULL);
    }
    return (_instance);
}