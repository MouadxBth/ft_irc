/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 21:52:17 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/24 11:43:45 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Command::Command()
    : _name("unknown"),
    _authRequired(false),
    _requireTrail(false)
{}

Command::~Command()
{}

Command::Command(const Command& instance)
:   _name(instance._name),
    _authRequired(instance._authRequired),
    _requireTrail(instance._requireTrail)
{}

Command& Command::operator=(const Command& instance)
{
    if (this != &instance)
    {
        _name = instance._name;
        _authRequired = instance._authRequired;
        _requireTrail = instance._requireTrail;
    }
    return *this;
}

Command::Command(const std::string& name, bool authRequired, bool trail)
    : _name(name),
    _authRequired(authRequired),
    _requireTrail(trail)
{}

const std::string&  Command::getName() const
{
    return (this->_name);
}

bool  Command::isAuthRequired() const
{
    return (this->_authRequired);
}

bool Command::requiresTrail() const
{
    return (this->_requireTrail);
}

void    Command::setName(const std::string& name)
{
    this->_name = name;
}

void    Command::setAuthRequired(bool auth)
{
    this->_authRequired = auth;
}

void    Command::setRequireTrail(bool trail)
{
    this->_requireTrail = trail;
}