/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 02:19:11 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/25 20:55:13 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstring>
#include <algorithm>

#include "Server.hpp"
#include "User.hpp"
#include "CommandManager.hpp"
#include "Utilities.hpp"

Server *Server::_instance = NULL;

Server::Server()
	:_port(6667),
	_password("password"),
	_name("localhost"),
	_version("Mawi-1.0.0"),
	_creationDate(getCurrentDateTime()),
	_userModes("none"),
	_channelModes("itkol"),
	_motd(generateMotd())
{}

Server::Server(const Server& instance)
	: _port(instance.getPort()),
	_password(instance.getPassword()),
	_name(instance.getName()),
	_version(instance.getVersion()),
	_creationDate(instance.getVersion()),
	_userModes(instance.getUserModes()),
	_channelModes(instance.getChannelModes()),
	_motd(instance.getMotd())
{
	*this = instance;
}

Server::Server(const size_t port, const std::string password)
	:_port(port),
	_password(password),
	_name("localhost"),
	_version("Mawi-1.0.0"),
	_creationDate(getCurrentDateTime()),
	_userModes("none"),
	_channelModes("itkol"),
	_motd(generateMotd())
{}

Server& Server::operator=(const Server& instance)
{
	if (this != &instance)
	{
		_reservedNicknames.clear();
		_restrictedNicknames.clear();
		
		_connectedUsers.clear();
		_authenticatedUsers.clear();
		_channels.clear();

		_connectedUsers = instance._connectedUsers;
		_authenticatedUsers = instance._authenticatedUsers;
		_channels = instance._channels;

		_listener = instance._listener;
	}
	return (*this);
}

Server::~Server()
{
	this->disable();
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

bool	Server::isEnabled() const
{
	return (this->_enabled);
}

size_t		Server::getPort() const
{
    return (this->_port);
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

const std::set<std::string>&	Server::getReservedNicknames() const
{
	return (this->_reservedNicknames);
}

const std::set<std::string>&	Server::getRestrictedNicknames() const
{
	return (this->_restrictedNicknames);
}

const std::map<int, User *>&		Server::getConnectedUsers() const
{
    return (this->_connectedUsers);
}

const std::map<std::string, User *>&		Server::getAuthenticatedUsers() const
{
    return (this->_authenticatedUsers);
}

const std::map<std::string, Channel *>&    Server::getChannels() const
{
    return (this->_channels);
}

User *Server::getUser(int fd)
{
	std::map<int, User *>::iterator result = _connectedUsers.find(fd);

	if (result != _connectedUsers.end())
		return (result->second);
	
	for (std::map<std::string, User *>::iterator it = _authenticatedUsers.begin();
		it != _authenticatedUsers.end();
		it++)
	{
		if (it->second && it->second->getSocket().fd == fd)
			return (it->second);
	}

	return (NULL);
}

User *Server::getUser(const std::string& nickname)
{
	std::map<std::string, User *>::iterator result = _authenticatedUsers.find(nickname);

	if (result != _authenticatedUsers.end())
		return (result->second);

	return (NULL);
}

Channel *Server::getChannel(const std::string& name)
{
	std::map<std::string, Channel *>::iterator result = _channels.find(name);
	
	if (result != _channels.end())
		return (result->second);
	
	return (NULL);
}

bool	Server::addReservedNickname(const std::string& nickname)
{
	return (this->_reservedNicknames.insert(nickname).second);
}

bool	Server::removeReservedNickname(const std::string& nickname)
{
	return (this->_reservedNicknames.erase(nickname));
}

bool	Server::addRestrictedNickname(const std::string& nickname)
{
	return (this->_restrictedNicknames.insert(nickname).second);
}

bool	Server::removeRestrictedNickname(const std::string& nickname)
{
	return (this->_restrictedNicknames.erase(nickname));
}

bool	Server::authenticateUser(User *user)
{
	if (!user || user->getNickname().empty())
		return (false);
	if (!_connectedUsers.erase(user->getSocket().fd))
		return (false);
	_authenticatedUsers[user->getNickname()] = user;
	return (true);
}

bool	Server::removeUser(User *user)
{
	if (!user)
		return (false);
	
	_socketsToBeRemoved.push_back(user->getSocket());
	
	return (true);
}

void	Server::addChannel(Channel *channel)
{
	if (!channel)
		return ;
	this->_channels[channel->getName()] = channel;
}

bool	Server::removeChannel(const std::string& name)
{
	return (_channelsToBeRemoved.insert(name).second);
}

void	Server::cleanChannels()
{
	Channel *target;
    
	for (std::set<std::string>::iterator it = _channelsToBeRemoved.begin();
        it != _channelsToBeRemoved.end();
		it++)
	{
        target = getChannel(*it);
        if (target && !target->getUsers().size())
        {
			std::cout << "deleting channel: " << target->getName() << std::endl;
            _channels.erase(*it);
            delete target;
        }
	}

    _channelsToBeRemoved.clear();
}