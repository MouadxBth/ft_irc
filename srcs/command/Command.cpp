/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 21:52:17 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/15 12:29:30 by mbouthai         ###   ########.fr       */
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
    _maxArguments(instance._maxArguments), 
    _authRequired(instance._authRequired),
    _requireTrail(instance._requireTrail)
{}

Command& Command::operator=(const Command& instance)
{
    if (this != &instance)
    {
        _name = instance._name;
        _description = instance._description;
        _maxArguments = instance._maxArguments;
        _authRequired = instance._authRequired;
        _requireTrail = instance._requireTrail;
    }
    return *this;
}

Command::Command(const std::string& name, const std::string& description, int maxArguments, bool authRequired, bool trail)
    : _name(name),
    _description(description),
    _maxArguments(maxArguments),
    _authRequired(authRequired),
    _requireTrail(trail)
{}

void    printAsci(std::string& str)
{
    for (size_t index = 0; index < str.size(); index++)
    {
        std::cout << ((int)str[index]) << " ";
    }
    std::cout << std::endl;
}

const std::string&  Command::getName() const
{
    return (this->_name);
}

const std::string&  Command::getDescription() const
{
    return (this->_description);
}

int  Command::getMaxArguments() const
{
    return (this->_maxArguments);
}

bool  Command::isAuthRequired() const
{
    return (this->_authRequired);
}

bool Command::requiresTrail() const
{
    return (this->_requireTrail);
}

Server  *Command::getServer()
{
    return (_server);
}

void    Command::setName(const std::string& name)
{
    _name = name;
}

void    Command::setDescription(const std::string& description)
{
    _description = description;
}

void    Command::setMaxArguments(int maxArguments)
{
    _maxArguments = maxArguments;
}

void    Command::setAuthRequired(bool auth)
{
    _authRequired = auth;
}

void    Command::setRequireTrail(bool trail)
{
    _requireTrail = trail;
}

void    Command::setServer(Server *server)
{
    _server = server;
}
