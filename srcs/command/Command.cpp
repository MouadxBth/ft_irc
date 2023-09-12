/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 21:52:17 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/12 10:34:51 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Command::Command()
{}

Command::~Command()
{}

Command::Command(const Command& instance)
:   _name(instance._name),
    _description(instance._description),
    _code(instance._code), 
    _authRequired(instance._authRequired)
{}

Command& Command::operator=(const Command& instance)
{
    if (this != &instance)
    {
        _name = instance._name;
        _description = instance._description;
        _code = instance._code;
        _authRequired = instance._authRequired;    
    }
    return *this;
}

Command::Command(const std::string& name, const std::string& description, size_t code, bool authRequired)
    : _name(name),
    _description(description),
    _code(code),
    _authRequired(authRequired)
{}

void    Command::execute(User *user, Data &data)
{
    if (!user)
        return ;
    
    if (!isKnownCommand(data.command))
    {
        user->sendMessage(ERR_UNKNOWN_COMMAND(user->getNickname(), data.command));
        return ;
    }
    
    if (!user->isAuthenticated() && _authRequired)
    {
        user->sendMessage(ERR_NOT_REGISTERED(user->getNickname()));
        return ;
    }

    executeCommand(user, data);
}

const std::string&  Command::getName() const
{
    return (this->_name);
}

const std::string&  Command::getDescription() const
{
    return (this->_description);
}

size_t  Command::getCode() const
{
    return (this->_code);
}

bool  Command::isAuthRequired() const
{
    return (this->_authRequired);
}


void    Command::setName(const std::string& name)
{
    _name = name;
}

void    Command::setDescription(const std::string& description)
{
    _description = description;
}

void    Command::setCode(size_t code)
{
    _code = code;
}

void    Command::setAuthRequired(bool auth)
{
    _authRequired = auth;
}
