/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 02:19:11 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/20 21:36:02 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstring>
#include <algorithm>

#include "Server.hpp"
#include "User.hpp"
#include "CommandManager.hpp"

Server *Server::_instance = NULL;

Server::Server()
	:_port(6667),
	_password("password"),
	_name("localhost"),
	_version("Mawi-1.0.0"),
	_creationDate(getCurrentDateTime()),
	_userModes("none"),
	_channelModes("itkol"),
	_motd(generateMotd()),
	_joins(0)
{}

Server::Server(const Server& instance)
	: _port(instance.getPort()),
	_password(instance.getPassword()),
	_name(instance.getName()),
	_version(instance.getVersion()),
	_creationDate(instance.getVersion()),
	_userModes(instance.getUserModes()),
	_channelModes(instance.getChannelModes()),
	_motd(instance.getMotd()),
	_joins(instance._joins)
{
	*this = instance;
}

Server& Server::operator=(const Server& instance)
{
	if (this != &instance)
	{
		_connectedUsers.clear();
		_authenticatedUsers.clear();
		_channels.clear();

		_connectedUsers = instance._connectedUsers;
		_authenticatedUsers = instance._authenticatedUsers;
		_channels = instance._channels;

		_address = instance._address;
		_listener = instance._listener;
	}
	return (*this);
}

Server::Server(const size_t port, const std::string password)
	:_port(port),
	_password(password),
	_name("localhost"),
	_version("Mawi-1.0.0"),
	_creationDate(getCurrentDateTime()),
	_userModes("none"),
	_channelModes("itkol"),
	_motd(generateMotd()),
	_joins(0)
{}

Server::~Server()
{
	this->disable();
}

const std::string&    Server::getName() const
{
    return (this->_name);
}

const std::string&    Server::getPassword() const
{
    return (this->_password);
}

const std::string&    Server::getVersion() const
{
    return (this->_version);
}

const std::string&    Server::getCreationDate() const
{
    return (this->_creationDate);
}

const std::string&    Server::getUserModes() const
{
    return (this->_userModes);
}

const std::string&    Server::getChannelModes() const
{
    return (this->_channelModes);
}

const std::vector<std::string>&    Server::getMotd() const
{
    return (this->_motd);
}

size_t		Server::getPort() const
{
    return (this->_port);
}

std::map<int, User *>&		Server::getConnectedUsers()
{
    return (this->_connectedUsers);
}

std::map<std::string, User *>&		Server::getAuthenticatedUsers()
{
    return (this->_authenticatedUsers);
}

std::map<std::string, Channel *>&    Server::getChannels()
{
    return (this->_channels);
}

std::vector<std::string>&	Server::getReservedNicknames()
{
	return (this->_reservedNicknames);
}

std::vector<std::string>&	Server::getRestrictedNicknames()
{
	return (this->_restrictedNicknames);
}

User *Server::getConnectedUser(int fd)
{
	std::map<int, User *>::iterator result = _connectedUsers.find(fd);
	
	if (result != _connectedUsers.end())
		return (result->second);
	
	return (NULL);
}

User *Server::getAuthenticatedUser(const std::string& nickname)
{
	std::map<std::string, User *>::iterator result = _authenticatedUsers.find(nickname);
	
	if (result != _authenticatedUsers.end())
		return (result->second);
	
	return (NULL);
}

User *Server::getAuthenticatedUser(int fd)
{
	for (std::map<std::string, User *>::iterator it = _authenticatedUsers.begin();
		it != _authenticatedUsers.end();
		it++)
	{
		if (it->second && it->second->getUserSocket().fd == fd)
			return (it->second);
	}
	return (NULL);
}

User *Server::getUser(int fd)
{
	User *user = getConnectedUser(fd);

    if (!user)
	{
        user = getAuthenticatedUser(fd);
	}

	return (user);
}

Channel *Server::getChannel(const std::string& name)
{
	std::map<std::string, Channel *>::iterator result = _channels.find(name);
	
	if (result != _channels.end())
		return (result->second);
	
	return (NULL);
}

int	Server::getJoins() const
{
	return (_joins);
}

void	Server::setJoins(int joins)
{
	_joins = joins;
}

void	Server::removeUser(User *user)
{
	if (!user)
		return ;

	_connectedUsers.erase(user->getUserSocket().fd);
	_authenticatedUsers.erase(user->getNickname());

	_socketsToBeRemoved.push_back(user->getUserSocket());
}

void	Server::removeChannel(const std::string& name)
{
	if (std::find(_channelsToBeRemoved.begin(),
		_channelsToBeRemoved.end(),
		name) == _channelsToBeRemoved.end())
	{
		_channelsToBeRemoved.push_back(name);
	}
}

void	Server::cleanChannels()
{
	Channel *target;
    
	for (std::vector<std::string>::iterator it = _channelsToBeRemoved.begin();
        it != _channelsToBeRemoved.end(); it++)
	{
        target = getChannel(*it);
        if (target && !target->getUsers().size())
        {
            _channels.erase(*it);
            delete target;
        }
	}

    _channelsToBeRemoved.clear();
}

Server *Server::getInstance()
{
	if (!_instance)
	{
		_instance = new Server();
		_instance->configure();
	}
	return (_instance);
}

Server *Server::createInstance(size_t port, std::string& password)
{
	if (!_instance)
	{
		_instance = new Server(port, password);
		_instance->configure();
	}
	return (_instance);
}