/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 11:15:11 by mbouthai          #+#    #+#             */
/*   Updated: 2023/10/04 03:38:59 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <vector>
#include <cstring>
#include <cerrno>
#include <unistd.h>

#include "User.hpp"

User::User()
	: _authenticated(false),
	_away(false),
	_usedPassword(false),
	_joinedChannelsCount(0)
{}

User::~User()
{}

User::User(const User& instance)
	: _socket(instance._socket)
{
	*this = instance;
}

User& User::operator=(const User& instance)
{
	if (this != &instance)
	{
		setNickname(instance.getNickname());
		setUsername(instance.getUsername());
		setFullname(instance.getFullname());
		setHostname(instance.getHostname());

		setPartialMessage(instance.getPartialMessage());
		setAwayMessage(instance.getAwayMessage());
	
		setAuthenticated(instance.isAuthenticated());
		setAway(instance.isAway());

		setJoinedChannelsCount(instance.getJoinedChannelsCount());
		setJoinedChannelsCount(instance.getJoinedChannelsCount());
	}
	return (*this);
}

User::User(const pollfd& socket)
	: _socket(socket), 
	_authenticated(false),
	_away(false),
	_usedPassword(false),
	_joinedChannelsCount(0)
{}

const pollfd&          User::getSocket() const
{
	return (this->_socket);
}

const std::string&    User::getNickname() const
{
	return (this->_nickname);
}

const std::string&   User::getUsername() const
{
	return (this->_username);
}

const std::string&    User::getFullname() const
{
	return (this->_fullname);
}

const std::string&    User::getHostname() const
{
	return (this->_hostname);
}

const std::string&    User::getPartialMessage() const
{
	return (this->_partialMessage);
}

const std::string&	User::getAwayMessage() const
{
	return (this->_awayMessage);
}

bool            User::isAuthenticated() const
{
	return (this->_authenticated);
}

bool            User::isAway() const
{
	return (this->_away);
}

bool	User::hasUsedPassword() const
{
	return (this->_usedPassword);
}

int	User::getJoinedChannelsCount() const
{
	return (this->_joinedChannelsCount);
}

void   User::setNickname(const std::string& nickname)
{
	this->_nickname = nickname;
}

void   User::setUsername(const std::string& username)
{
	this->_username = username;
}

void    User::setFullname(const std::string& fullname)
{
	this->_fullname = fullname;
}

void    User::setHostname(const std::string& hostname)
{
	this->_hostname = hostname;
}

void    User::setAuthenticated(bool authenticated)
{
	this->_authenticated = authenticated;
}

void    User::setAway(bool _operator)
{
	this->_away = _operator;
}

void    User::setPartialMessage(const std::string& partialMessage)
{
	this->_partialMessage = partialMessage;
}

void    User::setAwayMessage(const std::string& message)
{
	this->_awayMessage = message;
}

void	User::setUsedPassword(bool b)
{
	this->_usedPassword = b;
}

void	User::setJoinedChannelsCount(int a)
{
	this->_joinedChannelsCount = a;
}

bool	User::sendMessage(const std::string& input) const
{
	std::string message;

	if (getSocket().fd < 0)
		return (false);

	message += input;

	if (input[input.size() - 2] != '\r' && input[input.size() - 1] != '\n')
		message += "\r\n";

	if (send(getSocket().fd, message.c_str(), message.size(), 0) < 0)
		return (std::cerr << "Couldn't send message to " << getNickname()
				<< " because: " << strerror(errno)
				<< std::endl, false);
	
	return (true);
}

std::ostream& operator<<(std::ostream& outputStream, const User& user)
{
	outputStream << "User Info:\n"
		<< "=> Socket: " << user.getSocket().fd
		<< "\n=> Nickname: " << user.getNickname()
		<< "\n=> Username: " << user.getUsername()
		<< "\n=> Fullname: " << user.getFullname()
		<< "\n=> Hostname: " << user.getHostname() 
		<< "\n=> Partial Message: " << user.getPartialMessage() 
		<< "\n=> Authenticated: " << (user.isAuthenticated() ? "Yes" : "No") 
		<< "\n=> Away: " << (user.isAway() ? "Yes" : "No") << " Message: " << user.getAwayMessage() 
		<< "\n=> Used Password: " << (user.hasUsedPassword() ? "Yes" : "No") 
		<< "\n=> Joined Channels Count: " << user.getJoinedChannelsCount();

    return outputStream;
}
