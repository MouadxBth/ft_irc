/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 12:04:38 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/16 16:02:50 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandManager.hpp"
#include "Command.hpp"

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
    if (!_server)
        return ;
    command->setServer(_server);
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
    std::ostringstream oss;

    oss << "User ";
    oss << user->getUserSocket().fd;
    
    nickname = user->getNickname().empty() 
        ? oss.str()
        : user->getNickname();

    std::map<std::string, Command *>::iterator it = _registeredCommands.find(data.command);
    
    if ((it == _registeredCommands.end() 
        || !it->second)
        || !isKnownCommand(data.command)
        || !data.valid)
    {
        if (!data.valid)
            std::cout << "INVALID COMMAND" << std::endl;
        user->sendMessage(ERR_UNKNOWN_COMMAND(nickname, data.command));
        return ;
    }
    
    if (!user->isAuthenticated() && it->second->isAuthRequired())
    {
        user->sendMessage(ERR_NOT_REGISTERED(nickname));
        return ;
    }

    if (data.arguments.size() > 15 || (data.trail.empty() && it->second->requiresTrail()))
    {
        user->sendMessage(ERR_UNKNOWN_COMMAND(nickname, data.command));
        return ;
    }

    if (it->second->getMaxArguments() >= 0 && 
        data.arguments.size() != static_cast<size_t>(it->second->getMaxArguments()))
    {
        user->sendMessage(ERR_UNKNOWN_COMMAND(nickname, data.command));
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

void    CommandManager::setServer(Server *server)
{
    _server = server;
}